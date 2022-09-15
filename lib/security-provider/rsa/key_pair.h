#ifndef RSA_KEY_PAIR_H
#define RSA_KEY_PAIR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "key_pair_internal.h"

/**
 * Generate RSA key pair
 *
 * @param key_size  The RSA key size
 * @param private_key_path  The private key path
 * @param public_key_path  The public key path
 *
 * @return status code based on openssl library
 */
int generate_rsa_key_pair(const int key_size, const char* private_key_path,
                          const char* public_key_path) {
  // init status
  int status = 0;
  EVP_PKEY* p_key = NULL;

  // generate key
  p_key = _generate_rsa_key_pair(key_size);

  // check key
  if (p_key != NULL) {
    // write key files
    status = _write_rsa_key_pair(p_key, private_key_path, public_key_path);

    // free key
    EVP_PKEY_free(p_key);
  }

  return status;
}

/**
 * Generate RSA key pair with certificate
 *
 * @param cert_path  The certificate path
 * @param private_key_path  The private key path
 * @param public_key_path  The public key path
 * @param key_size  The RSA key size
 * @param expire_time  The certificate expire time
 * @param country  The country for certificate
 * @param state  The state for certificate
 * @param organization  The organization for certificate
 * @param common name  The common name for certificate
 *
 * @return status code based on openssl library
 */
int generate_rsa_key_pair_with_cert(const char* cert_path,
                                    const char* private_key_path,
                                    const char* public_key_path,
                                    const int key_size, const long expire_time,
                                    const char* country, const char* state,
                                    const char* org, const char* common_name) {
  // init data
  int ret = 0;
  X509* p_cert = NULL;
  EVP_PKEY* p_key = NULL;

  // generate rsa key
  p_key = _generate_rsa_key_pair(key_size);

  // check key
  if (p_key != NULL) {
    // write rsa key
    ret = _write_rsa_key_pair(p_key, private_key_path, public_key_path);

    // check result
    if (ret > 0) {
      // create cert
      p_cert = X509_new();

      // check cert
      if (p_cert != NULL) {
        // generate ceritifcate
        ret = _generate_x509_certificate(p_cert, p_key, expire_time, country,
                                         state, org, common_name);

        // check result
        if (ret > 0) {
          // write certificate
          ret = _write_x509_certificate(p_cert, cert_path);
        }

        // free data
        X509_free(p_cert);
      }
    }

    // free data
    EVP_PKEY_free(p_key);
  }

  return ret;
}

/**
 * Generate RSA key pair and certificate with CA Sign
 *
 * @param ca_private_key_path  The CA private key path
 * @param ca_cert_path  The CA certificate path
 * @param private_key_path  The private key path
 * @param public_key_path  The public key path
 * @param cert_path  The certificate path
 * @param key_size  The RSA key size
 * @param expire_time  The certificate expire time
 * @param country  The country for certificate
 * @param state  The state for certificate
 * @param organization  The organization for certificate
 * @param common name  The common name for certificate
 *
 * @return status code based on openssl library
 */
int generate_rsa_key_pair_and_cert_with_ca_sign(
    const char* ca_private_key_path, const char* ca_cert_path,
    const char* private_key_path, const char* public_key_path,
    const char* cert_path, const int key_size, const long expire_time,
    const char* country, const char* state, const char* org,
    const char* common_name) {
  // init data
  int ret = 0;
  X509* p_ca_cert = NULL;
  EVP_PKEY* p_ca_key = NULL;

  // load ca key
  p_ca_key = _load_rsa_private_key(ca_private_key_path);

  // load ca cert
  p_ca_cert = _load_x509_certificate(ca_cert_path);

  // check cert
  if (p_ca_key != NULL && p_ca_cert != NULL) {
    // init tls files
    X509* p_cert = NULL;
    EVP_PKEY* p_key = NULL;
    X509_REQ* p_cert_request = NULL;

    // generate rsa key
    p_key = _generate_rsa_key_pair(key_size);

    // check key
    if (p_key != NULL) {
      // write rsa key
      ret = _write_rsa_key_pair(p_key, private_key_path, public_key_path);

      // check result
      if (ret > 0) {
        // create cert request
        p_cert_request = X509_REQ_new();

        // check request
        if (p_cert_request != NULL) {
          // generate ceritifcate request
          ret = _generate_x509_certificate_request(
              p_cert_request, p_key, country, state, org, common_name);

          // check result
          if (ret > 0) {
            // create cert
            p_cert = X509_new();

            // check cert
            if (p_cert != NULL) {
              // generate ceritifcate
              ret = _generate_x509_certificate_with_request(
                  p_key, p_cert, p_cert_request, p_ca_key, p_ca_cert,
                  expire_time);

              // check result
              if (ret > 0) {
                // write certificate
                ret = _write_x509_certificate(p_cert, cert_path);
              }

              // free data
              X509_free(p_cert);
            }
          }

          // free data
          X509_REQ_free(p_cert_request);
        }
      }

      // free data
      EVP_PKEY_free(p_key);
    }
  }

  if (p_ca_cert != NULL) {
    // free data
    X509_free(p_ca_cert);
  }

  if (p_ca_key != NULL) {
    // free data
    EVP_PKEY_free(p_ca_key);
  }

  return ret;
}

/**
 * Verify RSA key pair
 *
 * @param private_key_path  The private key path
 * @param public_key_path  The public key path
 *
 * @return status code based on openssl library
 */
int validate_rsa_key_pair(const char* private_key_path,
                          const char* public_key_path) {
  // init result
  int ret = 0;

  // validate private key
  ret = _validate_rsa_private_key(private_key_path);

  // check result
  if (ret > 0) {
    // check public key
    ret = _validate_rsa_public_key(public_key_path);
  }

  return ret;
}

/**
 * Verify x509 certificate
 *
 * @param certificate_path  The certificate path
 *
 * @return status code based on openssl library
 */
int validate_x509_certificate(const char* certificate_path) {
  // call internal verifier
  return _validate_x509_certificate(certificate_path);
}

#ifdef __cplusplus
}
#endif

#endif  // RSA_KEY_PAIR_H