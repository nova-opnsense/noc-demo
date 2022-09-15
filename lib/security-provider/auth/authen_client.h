#ifndef AUTHEN_CLIENT_H
#define AUTHEN_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "authen_client_internal.h"

/**
 * Register Authen Client
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param data 	client data
 * @param data_len 	client data length
 * @param response data response
 *
 * @return http status code
 */
long register_client(const char* host, int port, const char* data,
                     const size_t data_len, char** response) {
  // get url length
  size_t url_len = snprintf(NULL, 0, "https://%s:%d/register", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/register", host, port);

  // register client
  return _send_post_request(url, data, data_len, response);
}

/**
 * Login Authen Client
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param data 	client login data
 * @param data_len 	client login data length
 * @param response data response
 *
 * @return http status code
 */
long login_client(const char* host, int port, const char* data,
                  const size_t data_len, char** response) {
  // get url length
  size_t url_len = snprintf(NULL, 0, "https://%s:%d/login", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/login", host, port);

  // login client
  return _send_post_request(url, data, data_len, response);
}

/**
 * Authorize Authen Client
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param data 	authorize data
 * @param data_len 	authorize data length
 * @param response data response
 *
 * @return http status code
 */
long authorize_client(const char* host, int port, const char* data,
                      const size_t data_len, char** response) {
  // get url length
  size_t url_len = snprintf(NULL, 0, "https://%s:%d/authorize", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/authorize", host, port);

  // authorize client
  return _send_post_request(url, data, data_len, response);
}

/**
 * Validate Authen Client
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param data 	validate data
 * @param data_len 	validate data length
 * @param response data response
 *
 * @return http status code
 */
long validate_client(const char* host, int port, const char* data,
                     const size_t data_len, char** response) {
  // get url length
  size_t url_len = snprintf(NULL, 0, "https://%s:%d/validate", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/validate", host, port);

  // validate client
  return _send_post_request(url, data, data_len, response);
}

/**
 * Recover Authen Client
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param data 	recover data
 * @param data_len 	recover data length
 * @param response data response
 *
 * @return http status code
 */
long recover_client(const char* host, int port, const char* data,
                    const size_t data_len, char** response) {
  // get url length
  size_t url_len = snprintf(NULL, 0, "https://%s:%d/recover", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/recover", host, port);

  // recover client
  return _send_post_request(url, data, data_len, response);
}

/**
 * Request Broker Info
 *
 * @param host	authen service host
 * @param port 	authen service port
 * @param token jwt token
 * @param response data response
 *
 * @return http status code
 */
long request_broker_info(const char* host, int port, const char* token,
                         char** response) {
  // get url length
  size_t url_len =
      snprintf(NULL, 0, "https://%s:%d/auth/broker", host, port) + 1;

  // format url
  char url[url_len];
  snprintf(url, url_len, "https://%s:%d/auth/broker", host, port);

  // get broker info
  return _send_get_request(url, token, response);
}

#ifdef __cplusplus
}
#endif

#endif  // AUTHEN_CLIENT_H