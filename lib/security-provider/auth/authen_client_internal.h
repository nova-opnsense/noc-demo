#ifndef AUTHEN_CLIENT_INTERNAL_H
#define AUTHEN_CLIENT_INTERNAL_H

#include <curl/curl.h>
#include <stdlib.h>

struct _data_struct {
  char *memory;
  size_t size;
};

/**
 * Data callback
 *
 * @param contents data contents
 * @param size data size
 * @param memb memory in bytes
 * @param u_data user data
 *
 * @return data received in bytes
 */
static inline size_t data_callback(void *contents, size_t size, size_t nmemb,
                                   void *u_data) {
  // init data
  size_t real_size = size * nmemb;
  struct _data_struct *mem = (struct _data_struct *)u_data;
  char *ptr = realloc(mem->memory, mem->size + real_size + 1);

  // check pointer
  if (ptr != NULL) {
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;
  }

  return real_size;
}

/**
 * Send http post request
 *
 * @param url http server url
 * @param data post data
 * @param data_len post data length
 * @param response response data
 *
 * @return http status
 */
static inline long _send_post_request(char *url, const char *data,
                                      const size_t data_len, char **response) {
  // init status
  long status = 0;
  CURL *p_curl = NULL;
  struct _data_struct chunk;

  // init global
  curl_global_init(CURL_GLOBAL_ALL);

  // init curl
  p_curl = curl_easy_init();

  // init chunk
  chunk.size = 0;
  chunk.memory = malloc(1);

  // check curl
  if (p_curl != NULL) {
    // set tls options
    curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // set data options
    curl_easy_setopt(p_curl, CURLOPT_URL, url);
    curl_easy_setopt(p_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(p_curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(p_curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(p_curl, CURLOPT_POSTFIELDSIZE, data_len);
    curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, data_callback);

    // perform request
    if (curl_easy_perform(p_curl) == CURLE_OK) {
      // get status
      curl_easy_getinfo(p_curl, CURLINFO_RESPONSE_CODE, &status);
    }

    // copy data
    *response = chunk.memory;

    // clean up curl
    curl_easy_cleanup(p_curl);
  }

  // clean up global
  curl_global_cleanup();

  return status;
}

/**
 * Send http get request
 *
 * @param url http server url
 * @param token jwt token
 * @param response response data
 *
 * @return http status
 */
static inline long _send_get_request(char *url, const char *token,
                                     char **response) {
  // init status
  long status = 0;
  CURL *p_curl = NULL;
  struct _data_struct chunk;
  struct curl_slist *p_headers = NULL;

  // init global
  curl_global_init(CURL_GLOBAL_ALL);

  // init curl
  p_curl = curl_easy_init();

  // init chunk
  chunk.size = 0;
  chunk.memory = malloc(1);

  // check curl
  if (p_curl != NULL) {
    // get token len
    size_t access_token_len = snprintf(NULL, 0, "AccessToken: %s", token) + 1;

    // build access token
    char access_token[access_token_len];
    snprintf(access_token, access_token_len, "AccessToken: %s", token);

    // build headers
    p_headers = curl_slist_append(p_headers, access_token);

    // set headers
    curl_easy_setopt(p_curl, CURLOPT_HTTPHEADER, p_headers);

    // set tls options
    curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(p_curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // set data options
    curl_easy_setopt(p_curl, CURLOPT_URL, url);
    curl_easy_setopt(p_curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(p_curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(p_curl, CURLOPT_WRITEFUNCTION, data_callback);

    // perform request
    if (curl_easy_perform(p_curl) == CURLE_OK) {
      // get status
      curl_easy_getinfo(p_curl, CURLINFO_RESPONSE_CODE, &status);
    }

    // copy data
    *response = chunk.memory;

    // clean up headers
    curl_slist_free_all(p_headers);

    // clean up curl
    curl_easy_cleanup(p_curl);
  }

  // clean up global
  curl_global_cleanup();

  return status;
}

#endif  // AUTHEN_CLIENT_INTERNAL_H