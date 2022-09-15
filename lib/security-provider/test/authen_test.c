#include <assert.h>
#include <auth/authen_client.h>

void test_client_register() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *data =
      "{\"id\":\"test-id\",\"name\":\"test\",\"software_info\":{},\"hardware_"
      "info\":{}}";
  const size_t data_len = strlen(data);

  // register client
  status = register_client(host, port, data, data_len, &response);

  assert(status == 200);

  // free data
  free(response);
}

void test_client_login() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *data =
      "{\"id\":\"test-id\", \"secret\":\"2cbd5f902bb09d2c80a5fe49c71a5e67\"}";
  const size_t data_len = strlen(data);

  // register client
  status = login_client(host, port, data, data_len, &response);

  assert(status == 200);

  // free data
  free(response);
}

void test_client_authorize() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *data =
      "{\"id\":\"test-id\", \"secret\":\"2cbd5f902bb09d2c80a5fe49c71a5e67\", "
      "\"claims\":{\"user_id\":\"admin@gmail.com\"}}";
  const size_t data_len = strlen(data);

  // register client
  status = authorize_client(host, port, data, data_len, &response);

  assert(status == 200);

  // free data
  free(response);
}

void test_client_validate() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *data =
      "{\"id\":\"test-id\", \"secret\":\"2cbd5f902bb09d2c80a5fe49c71a5e67\"}";
  const size_t data_len = strlen(data);

  // register client
  status = validate_client(host, port, data, data_len, &response);

  assert(status == 200);

  // free data
  free(response);
}

void test_client_recover() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *data =
      "{\"id\":\"test-id\",\"name\":\"test\",\"software_info\":{},\"hardware_"
      "info\":{}}";
  const size_t data_len = strlen(data);

  // register client
  status = recover_client(host, port, data, data_len, &response);

  assert(status == 200);

  // free data
  free(response);
}

void test_get_broker_info() {
  // define data
  int port = 8080;
  long status = 0;
  char *response = NULL;
  const char *host = "127.0.0.1";  //"192.168.3.72";
  const char *token =
      "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9."
      "eyJleHAiOjE2NTc2NDQyMDYsImlhdCI6MTY1NzU1NzgwNiwiaXNzIjoiTm92YUluVGVjaHMi"
      "LCJzZXJ2aWNlX2lkIjoidGVzdC1pZCIsInVzZXJfaWQiOiJhZG1pbkBnbWFpbC5jb20ifQ."
      "2pxl9K5v-eJ7nQM0wdGU2B_e7PmlbADBXlTTuZAKlCLpFpEOFIBXV6qhRl0I-uTjd_5n-"
      "2fGCcKWLpOQLEi0u9UBHqNhqBhRadG-Luw7m2g78jzb2DGQXossg_"
      "n1MFmCqwbRJDgGvY1BT6lHBlnNHO_"
      "ghQwIZYFT35j1JzrhwphmJDEuhZqNbLVFsVSUMWYUwmEbnrTUHzBAnFOVPc3-XtUV_"
      "VwnPGC9kQEtYrfubYfF4b96UtGYpJKE31rD6rscZWBWk4fvRgocTx5r-"
      "GQDeyuzvpeWhRxOnZx_bdynv3Knar3Bk9CVYnNDAd-w-irmV40al0KQYHYcPJOIEoAp_w";

  // register client
  status = request_broker_info(host, port, token, &response);

  assert(status == 200);

  // free data
  free(response);
}

int main() {
  // test register
  test_client_register();

  printf("Test register client is passed.\n");

  // test login
  test_client_login();

  printf("Test login client is passed.\n");

  // test authorize
  test_client_authorize();

  printf("Test authorize client is passed.\n");

  // test validate
  test_client_validate();

  printf("Test validate client is passed.\n");

  // test recover
  test_client_recover();

  printf("Test recover client is passed.\n");

  // test get broker
  test_get_broker_info();

  printf("Test get broker info is passed.\n");

  return 0;
}