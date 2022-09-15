#ifndef RSA_KEY_PAIR_INTERNAL_H
#define RSA_KEY_PAIR_INTERNAL_H

#include <openssl/pem.h>
#include <openssl/x509.h>
#include <unistd.h>

/**
 * Verify RSA private key
 *
 * @param key_path  The key path
 *
 * @return status code based on openssl library
 */
static inline int _validate_rsa_private_key(const char* key_path) {
  // init result
  int ret = 0;

  // check file exist
  if (access(key_path, F_OK) == 0) {
    // TODO: real validate here

    // set okay
    ret = 1;
  }

  return ret;
}

/**
 * Verify RSA public key
 *
 * @param key_path  The key path
 *
 * @return status code based on openssl library
 */
static inline int _validate_rsa_public_key(const char* key_path) {
  // init result
  int ret = 0;

  // check file exist
  if (access(key_path, F_OK) == 0) {
    // TODO: real validate here

    // set okay
    ret = 1;
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
static inline int _validate_x509_certificate(const char* certificate_path) {
  // init result
  int ret = 0;

  // check file exist
  if (access(certificate_path, F_OK) == 0) {
    // TODO: real validate here

    //  set okay
    ret = 1;
  }

  return ret;
}

/**
 * Load RSA key from private key
 *
 * @param key_path  The RSA key
 *
 * @return RSA key or NULL
 */
static inline EVP_PKEY* _load_rsa_private_key(const char* key_path) {
  // init data
  FILE* p_file = NULL;
  EVP_PKEY* p_key = NULL;

  // open file
  p_file = fopen(key_path, "rb");

  // check file
  if (p_file != NULL) {
    // read private key
    p_key = PEM_read_PrivateKey(p_file, &p_key, NULL, NULL);

    // close
    fclose(p_file);
  }

  return p_key;
}

/**
 * Load x509 certificate from file
 *
 * @param cert_path  The certificate path
 *
 * @return x509 certificate or NULL
 */
static inline X509* _load_x509_certificate(const char* cert_path) {
  // init data
  FILE* p_file = NULL;
  X509* p_cert = NULL;

  // open file
  p_file = fopen(cert_path, "rb");

  // check file
  if (p_file != NULL) {
    // read private key
    p_cert = PEM_read_X509(p_file, &p_cert, NULL, NULL);

    // close
    fclose(p_file);
  }

  return p_cert;
}

/**
 * Write x509 certificate to file
 *
 * @param cert  The x509 certificate
 * @param certificate_path  The certificate path
 *
 * @return status code based on openssl library
 */
static inline int _write_x509_certificate(X509* cert,
                                          const char* certificate_path) {
  // init status
  int status = 0;

  // init file
  FILE* cert_file = NULL;

  // open cert file
  cert_file = fopen(certificate_path, "wb");

  // check cert file
  if (cert_file != NULL) {
    // write cert
    status = PEM_write_X509(cert_file, cert);

    // close
    fclose(cert_file);
  }

  return status;
}

/**
 * Write RSA key pair to files
 *
 * @param key  The RSA key pair
 * @param private_key_path  The private key path
 * @param public_key_path  The public key path
 *
 * @return status code based on openssl library
 */
static inline int _write_rsa_key_pair(EVP_PKEY* key,
                                      const char* private_key_path,
                                      const char* public_key_path) {
  // init status
  int status = 0;

  // check input
  if (key != NULL && private_key_path != NULL && public_key_path != NULL) {
    // init file
    FILE* private_key_file = NULL;

    // open private key file
    private_key_file = fopen(private_key_path, "wb");

    // check private key file
    if (private_key_file != NULL) {
      // write private key
      status = PEM_write_PrivateKey(private_key_file, key, NULL, NULL, 0, NULL,
                                    NULL);

      // close
      fclose(private_key_file);
    }

    // check status
    if (status > 0) {
      // init file
      FILE* public_key_file = NULL;

      // open public key file
      public_key_file = fopen(public_key_path, "wb");

      // check public key file
      if (public_key_file != NULL) {
        // write public key
        status = PEM_write_PUBKEY(public_key_file, key);

        // close
        fclose(public_key_file);
      }
    }
  }

  return status;
}

/**
 * Generate RSA key pair
 *
 * @param key_size  The RSA key size
 *
 * @return RSA key pair or NULL
 */
static inline EVP_PKEY* _generate_rsa_key_pair(const int key_size) {
  // init data
  EVP_PKEY* p_key = NULL;

  // check input
  if (key_size > 0) {
    // init data
    int status = 0;
    RSA* p_rsa = NULL;
    BIGNUM* p_big_num = NULL;

    // create RSA
    p_rsa = RSA_new();

    // create key exponent
    p_big_num = BN_new();

    // create key
    p_key = EVP_PKEY_new();

    // check components
    if (p_key != NULL && p_rsa != NULL && p_big_num != NULL) {
      // set word
      BN_set_word(p_big_num, RSA_F4);

      // generate the RSA key
      status = RSA_generate_key_ex(p_rsa, key_size, p_big_num, NULL);

      // check status
      if (status > 0) {
        // assign key
        status = EVP_PKEY_assign_RSA(p_key, p_rsa);
      }

      // free data
      BN_free(p_big_num);
    }
  }

  return p_key;
}

/**
 * Generate x509 certificate
 *
 * @param cert The x509 certificate
 * @param key  The RSA key pair
 * @param expire_time  The expire time base on seconds
 * @param country  Country in CSR
 * @param state  State in CSR
 * @param organization  Organization in CSR
 * @param common_name  Common Name in CSR
 *
 * @return status code based on openssl library
 */
static inline int _generate_x509_certificate(
    X509* cert, EVP_PKEY* key, const long expire_time, const char* country,
    const char* state, const char* organization, const char* common_name) {
  // init status
  int status = 0;
  X509_NAME* p_name = NULL;

  // check input
  if (cert != NULL && key != NULL && expire_time > 0 && country != NULL &&
      state != NULL && organization != NULL && common_name != NULL) {
    // set the serial number
    ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);

    // Set expire time
    X509_gmtime_adj(X509_get_notBefore(cert), 0);
    X509_gmtime_adj(X509_get_notAfter(cert), expire_time);

    // set the public key
    X509_set_pubkey(cert, key);

    // copy name
    p_name = X509_get_subject_name(cert);

    // Set the country code and common name.
    X509_NAME_add_entry_by_txt(p_name, "C", MBSTRING_ASC,
                               (unsigned char*)country, -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "ST", MBSTRING_ASC,
                               (unsigned char*)state, -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "O", MBSTRING_ASC,
                               (unsigned char*)organization, -1, -1, 0);
    X509_NAME_add_entry_by_txt(p_name, "CN", MBSTRING_ASC,
                               (unsigned char*)common_name, -1, -1, 0);

    // Now set the issuer name
    X509_set_issuer_name(cert, p_name);

    // Sign the certificate with provided key.
    status = X509_sign(cert, key, EVP_sha256());
  }

  return status;
}

/**
 * Generate x509 certificate sign request
 *
 * @param cert_request  The certificate sign request
 * @param key  The RSA key pair
 * @param country  The country for certificate request
 * @param state  The state for certificate request
 * @param organization  The organization for certificate request
 * @param common_name  The common name for certificate request
 *
 * @return status code based on openssl library
 */
static inline int _generate_x509_certificate_request(
    X509_REQ* cert_request, EVP_PKEY* key, const char* country,
    const char* state, const char* organization, const char* common_name) {
  // init data
  int ret = 0;
  X509_NAME* p_name = NULL;

  // check input
  if (cert_request != NULL && key != NULL && country != NULL && state != NULL &&
      organization != NULL && common_name != NULL) {
    // set public key
    ret = X509_REQ_set_pubkey(cert_request, key);

    // check result
    if (ret > 0) {
      // get name
      p_name = X509_REQ_get_subject_name(cert_request);

      // Set the country code and common name.
      X509_NAME_add_entry_by_txt(p_name, "C", MBSTRING_ASC,
                                 (unsigned char*)country, -1, -1, 0);
      X509_NAME_add_entry_by_txt(p_name, "ST", MBSTRING_ASC,
                                 (unsigned char*)state, -1, -1, 0);
      X509_NAME_add_entry_by_txt(p_name, "O", MBSTRING_ASC,
                                 (unsigned char*)organization, -1, -1, 0);
      X509_NAME_add_entry_by_txt(p_name, "CN", MBSTRING_ASC,
                                 (unsigned char*)common_name, -1, -1, 0);

      // set the issuer name.
      X509_REQ_set_subject_name(cert_request, p_name);

      // sign request
      ret = X509_REQ_sign(cert_request, key, EVP_sha256());
    }
  }

  return ret;
}

/**
 * Generate x509 certificate with request
 *
 * @param key  The RSA key pair
 * @param cert  The x509 certificate
 * @param cert_request  The certificate sign request
 * @param ca_key  The RSA private key of CA
 * @param ca_cert  The x509 certificate of CA
 * @param expire_time  The certificate expire time
 *
 * @return status code based on openssl library
 */
static inline int _generate_x509_certificate_with_request(
    EVP_PKEY* key, X509* cert, X509_REQ* cert_request, EVP_PKEY* ca_key,
    X509* ca_cert, long expire_time) {
  // init data
  int ret = 0;

  // check input
  if (key != NULL && cert != NULL && cert_request != NULL && ca_key != NULL &&
      ca_cert != NULL && expire_time > 0) {
    // set serial
    ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);

    // set cert info
    X509_set_issuer_name(cert, X509_REQ_get_subject_name(cert_request));
    X509_set_subject_name(cert, X509_REQ_get_subject_name(cert_request));

    // set expire time
    X509_gmtime_adj(X509_get_notBefore(cert), 0);
    X509_gmtime_adj(X509_get_notAfter(cert), expire_time);

    // set key
    ret = X509_set_pubkey(cert, key);

    // check result
    if (ret > 0) {
      // set issue name
      X509_set_issuer_name(cert, X509_get_subject_name(ca_cert));

      // sign cert
      ret = X509_sign(cert, ca_key, EVP_sha256());
    }
  }

  return ret;
}

#endif  // RSA_KEY_PAIR_INTERNAL_H