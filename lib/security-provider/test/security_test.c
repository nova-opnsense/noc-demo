#include <assert.h>
#include <rsa/key_pair.h>

// cert info
#define STATE "HCM"
#define COUNTRY "VN"
#define EXPIRE_TIME 300000
#define ORGANIZATION "Nova"
#define COMMON_NAME "security-test"
#define CA_COMMON_NAME "NovaIntechs"

// key info
#define KEY_SIZE 2048
#define CERT_PATH "/tmp/cert.crt"
#define CA_CERT_PATH "/tmp/ca.crt"
#define PUBLIC_KEY_PATH "/tmp/public.key"
#define PRIVATE_KEY_PATH "/tmp/private.key"
#define CA_PUBLIC_KEY_PATH "/tmp/ca_public.key"
#define CA_PRIVATE_KEY_PATH "/tmp/ca_private.key"

void test_generate_rsa_key_pair() {
  // init
  int ret = 0;

  // generate key pair
  ret = generate_rsa_key_pair(KEY_SIZE, PRIVATE_KEY_PATH, PUBLIC_KEY_PATH);

  // assert
  assert(ret > 0);

  // validate key pair
  ret = validate_rsa_key_pair(PRIVATE_KEY_PATH, PUBLIC_KEY_PATH);

  // assert
  assert(ret > 0);

  // clean up
  remove(PUBLIC_KEY_PATH);
  remove(PRIVATE_KEY_PATH);
}

void test_generate_rsa_key_pair_with_cert() {
  // init
  int ret = 0;

  // generate key pair
  ret = generate_rsa_key_pair_with_cert(
      CERT_PATH, PRIVATE_KEY_PATH, PUBLIC_KEY_PATH, KEY_SIZE, EXPIRE_TIME,
      COUNTRY, STATE, ORGANIZATION, COMMON_NAME);

  // assert
  assert(ret > 0);

  // validate key pair
  ret = validate_rsa_key_pair(PRIVATE_KEY_PATH, PUBLIC_KEY_PATH);

  // assert
  assert(ret > 0);

  // validate cert
  ret = validate_x509_certificate(CERT_PATH);

  // assert
  assert(ret > 0);

  // clean up
  remove(CERT_PATH);
  remove(PUBLIC_KEY_PATH);
  remove(PRIVATE_KEY_PATH);
}

void test_generate_rsa_key_and_cert_with_sign_request() {
  // init
  int ret = 0;

  // generate key pair
  ret = generate_rsa_key_pair_with_cert(
      CA_CERT_PATH, CA_PRIVATE_KEY_PATH, CA_PUBLIC_KEY_PATH, KEY_SIZE,
      EXPIRE_TIME, COUNTRY, STATE, ORGANIZATION, CA_COMMON_NAME);

  // assert
  assert(ret > 0);

  // validate key pair
  ret = validate_rsa_key_pair(CA_PRIVATE_KEY_PATH, CA_PUBLIC_KEY_PATH);

  // assert
  assert(ret > 0);

  // validate cert
  ret = validate_x509_certificate(CA_CERT_PATH);

  // assert
  assert(ret > 0);

  // generate key & cert with sign request
  ret = generate_rsa_key_pair_and_cert_with_ca_sign(
      CA_PRIVATE_KEY_PATH, CA_CERT_PATH, PRIVATE_KEY_PATH, PUBLIC_KEY_PATH,
      CERT_PATH, KEY_SIZE, EXPIRE_TIME, COUNTRY, STATE, ORGANIZATION,
      COMMON_NAME);

  // assert
  assert(ret > 0);

  // validate key pair
  ret = validate_rsa_key_pair(PRIVATE_KEY_PATH, PUBLIC_KEY_PATH);

  // assert
  assert(ret > 0);

  // validate cert
  ret = validate_x509_certificate(CERT_PATH);

  // assert
  assert(ret > 0);

  // clean up
  remove(CERT_PATH);
  remove(CA_CERT_PATH);
  remove(PUBLIC_KEY_PATH);
  remove(PRIVATE_KEY_PATH);
  remove(CA_PUBLIC_KEY_PATH);
  remove(CA_PRIVATE_KEY_PATH);
}

int main() {
  // test gen key
  test_generate_rsa_key_pair();

  printf("Test generate RSA key pair is passed.\n");

  // test gen key with cert
  test_generate_rsa_key_pair_with_cert();

  printf("Test generate RSA key pair and certificate is passed.\n");

  // test gen key & cert with sign request
  test_generate_rsa_key_and_cert_with_sign_request();

  printf(
      "Test generate RSA key pair and certificate with sign request is "
      "passed.\n");

  return 0;
}