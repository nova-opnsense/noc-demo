#include "unistd.h"
#include "stdio.h"
#include "syslog.h"
#include "config.h"
#include "time.h"
#include "pthread.h"
#include "api/mqtt/mqtt_handler.h"
#include "core/mesh/setting/mesh_setting.h"
#include "core/mesh/core/emt.h"
#include <cjson/cJSON.h>
#include "utils/log/log.h"
#include "utils/queue/queue.h"
#include "utils/timer/timer.h"
#include "utils/cmd/cmd.h"
#include "utils/file/file.h"
#include "utils/dotenv/dotenv.h"
#include "utils/uci/app_uci.h"
#include "config/service_info/service_info.h"
#include "config/mqtt_bridge/mqtt_bridge.h"
#include "config/service_controller/service_controller.h"
#include "utils/app_mdns/app_mdns.h"

Queue_t message_queue;
Queue_t thread_queue;
static int log_level = -1;
static char *config_file_path = NULL;
static char *appname;
static int work_mode;

static void print_usage(void)
{
    fprintf(stderr,
            "Usage: %s [<options>] [<arguments>]\n\n"
            "Options:\n"
            "\t-c <path>  set the search path for config files (default: /data/config/ezm/ezm.cnf)\n"
            "\t-l <str>   set log_level {alert,crit,err,warn,notice,info,debug}\n"
            "\t-v         ezm version \n"
            "\t-h         help\n"
            "\n",
            appname);
}

static void parse_option(int argc, char **argv)
{
    int c;
    FILE *fp;
    while ((c = getopt(argc, argv, "c:l:hv")) != -1)
    {
        int this_option_optind = optind ? optind : 1;
        switch (c)
        {
        case 'c':
            fp = fopen(optarg, "r");
            if (fp != NULL)
            {
                config_file_path = optarg;
            }
            break;
        case 'l':
            if (!strcmp(optarg, "emerg"))
            {
                log_level = LOG_EMERG;
            }
            else if (!strcmp(optarg, "alert"))
            {
                log_level = LOG_ALERT;
            }
            else if (!strcmp(optarg, "crit"))
            {
                log_level = LOG_CRIT;
            }
            else if (!strcmp(optarg, "err"))
            {
                log_level = LOG_ERR;
            }
            else if (!strcmp(optarg, "warn"))
            {
                log_level = LOG_WARNING;
            }
            else if (!strcmp(optarg, "notice"))
            {
                log_level = LOG_NOTICE;
            }
            else if (!strcmp(optarg, "info"))
            {
                log_level = LOG_INFO;
            }
            else if (!strcmp(optarg, "debug"))
            {
                log_level = LOG_DEBUG;
            }
            else
            {
                fprintf(stderr, "Invalid Log Param\n");
                exit(1);
            }
            break;
        case 'v':
            fprintf(stderr, "Version : %s\n", EZM_VERSION_DEFAULT);
            exit(0);
            break;
        case 'h':
            print_usage();
            exit(0);
            break;
        default:
            fprintf("Invalid Option: %c\n", c);
            print_usage();
            exit(1);
            break;
        }
    }
    if (optind < argc)
    {
        fprintf(stderr, "Invalid Argument: ");
        while (optind < argc)
        {
            fprintf(stderr, "%s  ", argv[optind++]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }
}

void init()
{
    q_init(&message_queue, sizeof(struct mosquitto_message), 1000, FIFO, true);
    q_init(&thread_queue, sizeof(pthread_t), 1000, FIFO, true);
    init_service_info();
    init_error_table();
    initialize_timer();

    log_info("Waiting for all config done");
    sleep(5);

    log_info("mdns_query_device_list >>> 1 begin");
    device_list_t *device_list = mdns_query_device_list("_noc._tcp.local.", 1);
    log_debug("%p", device_list);
    log_debug("%p", device_list->device_list);
    log_debug("%d", device_list->nb_device);
    log_info("mdns_query_device_list >>> 2 end");

    // Init network config and restart network
    mqtt_connect(get_config().mosquitto_host, get_config().mosquitto_port, 60);
    mqtt_loop();
    // start_timer(50, thread_garbage, TIMER_PERIODIC, NULL);
}

int main(int argc, const char *argv[])
{
    appname = argv[0];
    parse_option(argc, argv);

    if (!config_file_path)
    {
        config_file_path = CONFIG_FILE_PATH_DEFAULT;
    }
    log_debug("config_file_path: %s", config_file_path);
    load_config(config_file_path);

    // if(log_level != -1){
    //     log_init(appname , log_level);
    // }
    // else{
    //     log_init(appname , LOG_LEVEL_DEFAULT);
    // }

    // Sleep for initialize syslog-ng
    sleep(1);
    init();

    while (1)
    {
        sleep(1);
    }

    mqtt_clean();

    return 0;
}
