#include "app_mdns.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stdio.h>

#include <errno.h>
#include <signal.h>

#include "utils/string/app_string.h"
#include "utils/log/log.h"
#include "utils/file/file.h"
#include "utils/cmd/cmd.h"
#include "config/service_controller/service_controller.h"
#include "config/service_info/service_info.h"
#include "core/mesh/setting/mesh_setting.h"
#include "config.h"

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/time.h>
#include <sys/select.h>
#endif

// Alias some things to simulate recieving data to fuzz library
#if defined(MDNS_FUZZING)
#define recvfrom(sock, buffer, capacity, flags, src_addr, addrlen) ((mdns_ssize_t)capacity)
#define printf
#endif

#if defined(MDNS_FUZZING)
#undef recvfrom
#endif

static char addrbuffer[64];
static char entrybuffer[256];
static char namebuffer[256];
static char sendbuffer[1024];
static mdns_record_txt_t txtbuffer[128];

static struct sockaddr_in service_address_ipv4;
static struct sockaddr_in6 service_address_ipv6;

static int has_ipv4;
static int has_ipv6;

volatile sig_atomic_t running = 1;

// Data for our service including the mDNS records
typedef struct
{
	mdns_string_t service;
	mdns_string_t hostname;
	mdns_string_t service_instance;
	mdns_string_t hostname_qualified;
	struct sockaddr_in address_ipv4;
	struct sockaddr_in6 address_ipv6;
	int port;
	mdns_record_t record_ptr;
	mdns_record_t record_srv;
	mdns_record_t record_a;
	mdns_record_t record_aaaa;
	mdns_record_t txt_record[2];
} service_t;

static mdns_string_t
ipv4_address_to_string(char *buffer, size_t capacity, const struct sockaddr_in *addr,
					   size_t addrlen)
{
	char host[NI_MAXHOST] = {0};
	char service[NI_MAXSERV] = {0};
	int ret = getnameinfo((const struct sockaddr *)addr, (socklen_t)addrlen, host, NI_MAXHOST,
						  service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;
	if (ret == 0)
	{
		if (addr->sin_port != 0)
			len = snprintf(buffer, capacity, "%s:%s", host, service);
		else
			len = snprintf(buffer, capacity, "%s", host);
	}
	if (len >= (int)capacity)
		len = (int)capacity - 1;
	mdns_string_t str;
	str.str = buffer;
	str.length = len;
	return str;
}

static mdns_string_t
ipv6_address_to_string(char *buffer, size_t capacity, const struct sockaddr_in6 *addr,
					   size_t addrlen)
{
	char host[NI_MAXHOST] = {0};
	char service[NI_MAXSERV] = {0};
	int ret = getnameinfo((const struct sockaddr *)addr, (socklen_t)addrlen, host, NI_MAXHOST,
						  service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
	int len = 0;
	if (ret == 0)
	{
		if (addr->sin6_port != 0)
			len = snprintf(buffer, capacity, "[%s]:%s", host, service);
		else
			len = snprintf(buffer, capacity, "%s", host);
	}
	if (len >= (int)capacity)
		len = (int)capacity - 1;
	mdns_string_t str;
	str.str = buffer;
	str.length = len;
	return str;
}

static mdns_string_t
ip_address_to_string(char *buffer, size_t capacity, const struct sockaddr *addr, size_t addrlen)
{
	if (addr->sa_family == AF_INET6)
		return ipv6_address_to_string(buffer, capacity, (const struct sockaddr_in6 *)addr, addrlen);
	return ipv4_address_to_string(buffer, capacity, (const struct sockaddr_in *)addr, addrlen);
}

// Callback handling parsing answers to queries sent
static int
query_callback(int sock, const struct sockaddr *from, size_t addrlen, mdns_entry_type_t entry,
			   uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void *data,
			   size_t size, size_t name_offset, size_t name_length, size_t record_offset,
			   size_t record_length, void *user_data)
{
	(void)sizeof(sock);
	(void)sizeof(query_id);
	(void)sizeof(name_length);
	device_list_t *device_list = (device_list_t *)user_data;

	mdns_string_t fromaddrstr = ip_address_to_string(addrbuffer, sizeof(addrbuffer), from, addrlen);
	const char *entrytype = (entry == MDNS_ENTRYTYPE_ANSWER) ? "answer" : ((entry == MDNS_ENTRYTYPE_AUTHORITY) ? "authority" : "additional");
	mdns_string_t entrystr =
		mdns_string_extract(data, size, &name_offset, entrybuffer, sizeof(entrybuffer));
	if (rtype == MDNS_RECORDTYPE_PTR)
	{
		mdns_string_t namestr = mdns_record_parse_ptr(data, size, record_offset, record_length,
													  namebuffer, sizeof(namebuffer));
		log_debug("%.*s : %s %.*s PTR %.*s rclass 0x%x ttl %u length %d\n",
				  MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
				  MDNS_STRING_FORMAT(namestr), rclass, ttl, (int)record_length);
	}
	else if (rtype == MDNS_RECORDTYPE_SRV)
	{
		mdns_record_srv_t srv = mdns_record_parse_srv(data, size, record_offset, record_length,
													  namebuffer, sizeof(namebuffer));
		log_debug("%.*s : %s %.*s SRV %.*s priority %d weight %d port %d\n",
				  MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
				  MDNS_STRING_FORMAT(srv.name), srv.priority, srv.weight, srv.port);
	}
	else if (rtype == MDNS_RECORDTYPE_A)
	{
		struct sockaddr_in addr;
		mdns_record_parse_a(data, size, record_offset, record_length, &addr);
		mdns_string_t addrstr =
			ipv4_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
		log_debug("%.*s : %s %.*s A %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
				  MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
	}
	else if (rtype == MDNS_RECORDTYPE_AAAA)
	{
		struct sockaddr_in6 addr;
		mdns_record_parse_aaaa(data, size, record_offset, record_length, &addr);
		mdns_string_t addrstr =
			ipv6_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
		log_debug("%.*s : %s %.*s AAAA %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
				  MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
	}
	else if (rtype == MDNS_RECORDTYPE_TXT)
	{
		size_t parsed = mdns_record_parse_txt(data, size, record_offset, record_length, txtbuffer,
											  sizeof(txtbuffer) / sizeof(mdns_record_txt_t));
		device_list->nb_device++;
		device_list->device_list = realloc(device_list->device_list, device_list->nb_device * sizeof(device_info_t));
		memset(&device_list->device_list[device_list->nb_device - 1], 0, sizeof(device_info_t));
		for (size_t itxt = 0; itxt < parsed; ++itxt)
		{
			// Check Role before -> Just get Standalone mode
			if (!strncmp(txtbuffer[itxt].key.str, "role", strlen("role")))
			{
				// If diff standalone -> Remove device
				if (!strncmp(txtbuffer[itxt].value.str, "standalone", strlen("standalone")))
				{
					log_debug("Standalone Mode");
					device_list->device_list[device_list->nb_device - 1].role = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
				}
				else
				{
					device_list->nb_device--;
					break;
				}
			}
			if (!strncmp(txtbuffer[itxt].key.str, "hub_id", strlen("hub_id")))
			{
				device_list->device_list[device_list->nb_device - 1].hub_id = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
			else if (!strncmp(txtbuffer[itxt].key.str, "ezm_id", strlen("ezm_id")))
			{
				device_list->device_list[device_list->nb_device - 1].ezm_id = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
			else if (!strncmp(txtbuffer[itxt].key.str, "ezm_version", strlen("ezm_version")))
			{
				device_list->device_list[device_list->nb_device - 1].ezm_version = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
			else if (!strncmp(txtbuffer[itxt].key.str, "ezm_name", strlen("ezm_name")))
			{
				device_list->device_list[device_list->nb_device - 1].ezm_name = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
			else if (!strncmp(txtbuffer[itxt].key.str, "mac", strlen("mac")))
			{
				device_list->device_list[device_list->nb_device - 1].mac = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
			else if (!strncmp(txtbuffer[itxt].key.str, "ip", strlen("ip")))
			{
				device_list->device_list[device_list->nb_device - 1].ip = app_strndup(txtbuffer[itxt].value.str, txtbuffer[itxt].value.length);
			}
		}
	}
	else
	{
		log_debug("%.*s : %s %.*s type %u rclass 0x%x ttl %u length %d\n",
				  MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr), rtype,
				  rclass, ttl, (int)record_length);
	}
	return 0;
}

// Open sockets for sending one-shot multicast queries from an ephemeral port
static int
open_client_sockets(int *sockets, int max_sockets, int port)
{
	// When sending, each socket can only send to one network interface
	// Thus we need to open one socket for each interface and address family
	int num_sockets = 0;

#ifdef _WIN32

	IP_ADAPTER_ADDRESSES *adapter_address = 0;
	ULONG address_size = 8000;
	unsigned int ret;
	unsigned int num_retries = 4;
	do
	{
		adapter_address = (IP_ADAPTER_ADDRESSES *)malloc(address_size);
		ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_ANYCAST, 0,
								   adapter_address, &address_size);
		if (ret == ERROR_BUFFER_OVERFLOW)
		{
			free(adapter_address);
			adapter_address = 0;
			address_size *= 2;
		}
		else
		{
			break;
		}
	} while (num_retries-- > 0);

	if (!adapter_address || (ret != NO_ERROR))
	{
		free(adapter_address);
		printf("Failed to get network adapter addresses\n");
		return num_sockets;
	}

	int first_ipv4 = 1;
	int first_ipv6 = 1;
	for (PIP_ADAPTER_ADDRESSES adapter = adapter_address; adapter; adapter = adapter->Next)
	{
		if (adapter->TunnelType == TUNNEL_TYPE_TEREDO)
			continue;
		if (adapter->OperStatus != IfOperStatusUp)
			continue;

		for (IP_ADAPTER_UNICAST_ADDRESS *unicast = adapter->FirstUnicastAddress; unicast;
			 unicast = unicast->Next)
		{
			if (unicast->Address.lpSockaddr->sa_family == AF_INET)
			{
				struct sockaddr_in *saddr = (struct sockaddr_in *)unicast->Address.lpSockaddr;
				if ((saddr->sin_addr.S_un.S_un_b.s_b1 != 127) ||
					(saddr->sin_addr.S_un.S_un_b.s_b2 != 0) ||
					(saddr->sin_addr.S_un.S_un_b.s_b3 != 0) ||
					(saddr->sin_addr.S_un.S_un_b.s_b4 != 1))
				{
					int log_addr = 0;
					if (first_ipv4)
					{
						service_address_ipv4 = *saddr;
						first_ipv4 = 0;
						log_addr = 1;
					}
					has_ipv4 = 1;
					if (num_sockets < max_sockets)
					{
						saddr->sin_port = htons((unsigned short)port);
						int sock = mdns_socket_open_ipv4(saddr);
						if (sock >= 0)
						{
							sockets[num_sockets++] = sock;
							log_addr = 1;
						}
						else
						{
							log_addr = 0;
						}
					}
					if (log_addr)
					{
						char buffer[128];
						mdns_string_t addr = ipv4_address_to_string(buffer, sizeof(buffer), saddr,
																	sizeof(struct sockaddr_in));
						printf("Local IPv4 address: %.*s\n", MDNS_STRING_FORMAT(addr));
					}
				}
			}
			else if (unicast->Address.lpSockaddr->sa_family == AF_INET6)
			{
				struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)unicast->Address.lpSockaddr;
				// Ignore link-local addresses
				if (saddr->sin6_scope_id)
					continue;
				static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
														  0, 0, 0, 0, 0, 0, 0, 1};
				static const unsigned char localhost_mapped[] = {0, 0, 0, 0, 0, 0, 0, 0,
																 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
				if ((unicast->DadState == NldsPreferred) &&
					memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
					memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
				{
					int log_addr = 0;
					if (first_ipv6)
					{
						service_address_ipv6 = *saddr;
						first_ipv6 = 0;
						log_addr = 1;
					}
					has_ipv6 = 1;
					if (num_sockets < max_sockets)
					{
						saddr->sin6_port = htons((unsigned short)port);
						int sock = mdns_socket_open_ipv6(saddr);
						if (sock >= 0)
						{
							sockets[num_sockets++] = sock;
							log_addr = 1;
						}
						else
						{
							log_addr = 0;
						}
					}
					if (log_addr)
					{
						char buffer[128];
						mdns_string_t addr = ipv6_address_to_string(buffer, sizeof(buffer), saddr,
																	sizeof(struct sockaddr_in6));
						printf("Local IPv6 address: %.*s\n", MDNS_STRING_FORMAT(addr));
					}
				}
			}
		}
	}

	free(adapter_address);

#else

	struct ifaddrs *ifaddr = 0;
	struct ifaddrs *ifa = 0;

	if (getifaddrs(&ifaddr) < 0)
		log_error("Unable to get interface addresses\n");

	int first_ipv4 = 1;
	int first_ipv6 = 1;
	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr)
			continue;
		if (!(ifa->ifa_flags & IFF_UP) || !(ifa->ifa_flags & IFF_MULTICAST))
			continue;
		if ((ifa->ifa_flags & IFF_LOOPBACK) || (ifa->ifa_flags & IFF_POINTOPOINT))
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET)
		{
			struct sockaddr_in *saddr = (struct sockaddr_in *)ifa->ifa_addr;
			if (saddr->sin_addr.s_addr != htonl(INADDR_LOOPBACK))
			{
				int log_addr = 0;
				if (first_ipv4)
				{
					service_address_ipv4 = *saddr;
					first_ipv4 = 0;
					log_addr = 1;
				}
				has_ipv4 = 1;
				if (num_sockets < max_sockets)
				{
					saddr->sin_port = htons(port);
					int sock = mdns_socket_open_ipv4(saddr);
					if (sock >= 0)
					{
						sockets[num_sockets++] = sock;
						log_addr = 1;
					}
					else
					{
						log_addr = 0;
					}
				}
				if (log_addr)
				{
					char buffer[128];
					mdns_string_t addr = ipv4_address_to_string(buffer, sizeof(buffer), saddr,
																sizeof(struct sockaddr_in));
					log_debug("Local IPv4 address: %.*s\n", MDNS_STRING_FORMAT(addr));
				}
			}
		}
		else if (ifa->ifa_addr->sa_family == AF_INET6)
		{
			struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)ifa->ifa_addr;
			// Ignore link-local addresses
			if (saddr->sin6_scope_id)
				continue;
			static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
													  0, 0, 0, 0, 0, 0, 0, 1};
			static const unsigned char localhost_mapped[] = {0, 0, 0, 0, 0, 0, 0, 0,
															 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
			if (memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
				memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
			{
				int log_addr = 0;
				if (first_ipv6)
				{
					service_address_ipv6 = *saddr;
					first_ipv6 = 0;
					log_addr = 1;
				}
				has_ipv6 = 1;
				if (num_sockets < max_sockets)
				{
					saddr->sin6_port = htons(port);
					int sock = mdns_socket_open_ipv6(saddr);
					if (sock >= 0)
					{
						sockets[num_sockets++] = sock;
						log_addr = 1;
					}
					else
					{
						log_addr = 0;
					}
				}
				if (log_addr)
				{
					char buffer[128];
					mdns_string_t addr = ipv6_address_to_string(buffer, sizeof(buffer), saddr,
																sizeof(struct sockaddr_in6));
					log_debug("Local IPv6 address: %.*s\n", MDNS_STRING_FORMAT(addr));
				}
			}
		}
	}

	freeifaddrs(ifaddr);

#endif

	return num_sockets;
}

// Send a mDNS query
device_list_t *mdns_query_device_list(char *name, int timeout_second)
{
	mdns_query_t query;
	query.name = name;
	query.length = strlen(name);
	query.type = MDNS_RECORDTYPE_PTR;
	int count = 1;

	int sockets[32];
	int query_id[32];
	int num_sockets = open_client_sockets(sockets, sizeof(sockets) / sizeof(sockets[0]), 0);
	if (num_sockets <= 0)
	{
		log_error("Failed to open any client sockets\n");
		return NULL;
	}
	log_debug("Opened %d socket%s for mDNS query\n", num_sockets, num_sockets ? "s" : "");

	size_t capacity = 2048;
	void *buffer = malloc(capacity);
	device_list_t *device_list = calloc(1, sizeof(device_list_t));

	log_debug("Sending mDNS query");
	for (int isock = 0; isock < num_sockets; ++isock)
	{
		query_id[isock] =
			mdns_multiquery_send(sockets[isock], &query, count, buffer, capacity, 0);
		if (query_id[isock] < 0)
			log_error("Failed to send mDNS query: %s\n", strerror(errno));
	}

	// This is a simple implementation that loops for 5 seconds or as long as we get replies
	int res;
	log_debug("Reading mDNS query replies\n");
	int records = 0;
	do
	{
		struct timeval timeout;
		timeout.tv_sec = timeout_second;
		timeout.tv_usec = 0;

		int nfds = 0;
		fd_set readfs;
		FD_ZERO(&readfs);
		for (int isock = 0; isock < num_sockets; ++isock)
		{
			if (sockets[isock] >= nfds)
				nfds = sockets[isock] + 1;
			FD_SET(sockets[isock], &readfs);
		}

		res = select(nfds, &readfs, 0, 0, &timeout);
		if (res > 0)
		{
			for (int isock = 0; isock < num_sockets; ++isock)
			{
				if (FD_ISSET(sockets[isock], &readfs))
				{
					int rec = mdns_query_recv(sockets[isock], buffer, capacity, query_callback, device_list, query_id[isock]);
					if (rec > 0)
						records += rec;
				}
				FD_SET(sockets[isock], &readfs);
			}
		}
	} while (res > 0);

	log_debug("Read %d records\n", records);

	free(buffer);

	for (int isock = 0; isock < num_sockets; ++isock)
		mdns_socket_close(sockets[isock]);
	log_debug("Closed socket%s\n", num_sockets ? "s" : "");

	return device_list;
}

void mdns_create_publisher()
{
	char avahi_data[4096];
	device_info_t *device = get_current_device_info();
	sprintf(avahi_data,
			"<?xml version=\"v1.0.0\" standalone='no'?><!--*-nxml-*-->\n"
			"<!DOCTYPE service-group SYSTEM \"avahi-service.dtd\">\n"
			"<service-group>\n"
			"<name>Ezm Service</name>\n"
			"<service>\n"
			"<type>_ezm._tcp</type>\n"
			"<txt-record>hub_id=%s</txt-record>\n"
			"<txt-record>ezm_id=%s</txt-record>\n"
			"<txt-record>ezm_version=%s</txt-record>\n"
			"<txt-record>ezm_name=%s</txt-record>\n"
			"<txt-record>role=%s</txt-record>\n"
			"<txt-record>mac=%s</txt-record>\n"
			"<txt-record>ip=%s</txt-record>\n"
			"</service>\n"
			"</service-group>",
			device->hub_id,		 // hub_id
			device->ezm_id,		 // ezm_id
			device->ezm_version, // ezm_version
			device->ezm_name,	 // Ezm_name
			device->role,		 // Role
			device->mac,		 // mac
			device->ip			 // ip
	);
	write_data(avahi_data, strlen(avahi_data), AVAHI_EZM_SERVICE_FILE, TRUNCATE);
	sleep(1);
	restart_service("avahi-deamon");
}
void mdns_remove_publisher(char *service_file)
{
	remove(service_file);
	sleep(1);
	restart_service("avahi-deamon");
}