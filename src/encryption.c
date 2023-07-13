#include "../include/encryption.h"


int deriveKey(const char* password, unsigned char* key, unsigned char* salt) {
    const int iterations = 10000;  // Number of iterations
    const int key_length = 32;     // AES-256 key length in bytes (256 bits)

    if (PKCS5_PBKDF2_HMAC((const char*)password, strlen((const char*)password), salt, sizeof(salt), iterations, EVP_sha256(), key_length, key) == 0) {
        fprintf(stderr, "Key derivation failed.\n");
        return 0;
    }

    return 1;
}


int encryptAES256(const char* plaintext, int plaintext_len, const unsigned char* key, unsigned char* ciphertext, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx;

    int len;
    int ciphertext_len;

    /* Create and initialize the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return 0;
    }

    /* Initialize the encryption operation */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    /* Enable padding and set the padding mode */
    EVP_CIPHER_CTX_set_padding(ctx, 1); // Enable padding
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7); // Set padding mode to PKCS7

    /* Provide the message to be encrypted, and obtain the encrypted output */
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (const unsigned char*)plaintext, plaintext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    ciphertext_len = len;

    /* Finalize the encryption */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


// Function to decrypt the encrypted password
int decryptAES256(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, char* plaintext, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx;

    int len;
    int plaintext_len;

    /* Create and initialize the context */
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return 0;
    }

    /* Initialize the decryption operation */
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    /* Enable padding and set the padding mode */
    EVP_CIPHER_CTX_set_padding(ctx, 1); // Enable padding
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7); // Set padding mode to PKCS7

    /* Provide the ciphertext to be decrypted, and obtain the decrypted output */
    if (1 != EVP_DecryptUpdate(ctx, (unsigned char*)plaintext, &len, ciphertext, ciphertext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    plaintext_len = len;

    /* Finalize the decryption */
    if (1 != EVP_DecryptFinal_ex(ctx, (unsigned char*)plaintext + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    plaintext_len += len;

    /* Null-terminate the decrypted plaintext */
    plaintext[plaintext_len] = '\0';

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}



void generateRandomIV(unsigned char* iv, int iv_length) {
    // Generate a random IV
    RAND_bytes(iv, iv_length);
}

void generateRandomSalt(unsigned char* salt, int salt_length) {
    // Generate a random Salt
    RAND_bytes(salt, salt_length);
}

char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, data, input_length);
    BIO_flush(bio);

    BUF_MEM* buffer_ptr;
    BIO_get_mem_ptr(bio, &buffer_ptr);
    *output_length = buffer_ptr->length;

    char* encoded_data = (char*)malloc(*output_length + 1); // Add space for the null terminator
    if (encoded_data == NULL) {
        BIO_free_all(bio);
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    memcpy(encoded_data, buffer_ptr->data, *output_length);
    encoded_data[*output_length] = '\0'; // Add null terminator

    BIO_free_all(bio);
    return encoded_data;
}

unsigned char* base64_decode(const char* encoded_data, size_t input_length, size_t* output_length) {
    BIO* bio, * b64;

    // Create a BIO object with a memory buffer
    bio = BIO_new_mem_buf(encoded_data, input_length);
    if (bio == NULL) {
        fprintf(stderr, "Error creating BIO object.\n");
        return NULL;
    }

    // Create a Base64 BIO
    b64 = BIO_new(BIO_f_base64());
    if (b64 == NULL) {
        BIO_free_all(bio);
        fprintf(stderr, "Error creating Base64 BIO object.\n");
        return NULL;
    }
    bio = BIO_push(b64, bio);

    // Ignore newlines
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    // Determine the maximum possible size of the decoded data
    size_t max_output_length = input_length * 3 / 4;

    // Allocate memory for the decoded data
    unsigned char* decoded_data = (unsigned char*)malloc(max_output_length);
    if (decoded_data == NULL) {
        BIO_free_all(bio);
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    // Perform the Base64 decoding
    int length = BIO_read(bio, decoded_data, max_output_length);
    if (length < 0) {
        free(decoded_data);
        BIO_free_all(bio);
        fprintf(stderr, "Error performing Base64 decoding.\n");
        return NULL;
    }

    // Set the actual output length
    *output_length = (size_t)length;

    // Clean up and return the decoded data
    BIO_free_all(bio);
    return decoded_data;
}


int compare_binary_keys(const unsigned char* key1, size_t key1_length, const unsigned char* key2, size_t key2_length) {
    if (key1_length != key2_length) {
        return 0; // Keys have different lengths
    }

    for (size_t i = 0; i < key1_length; i++) {
        if (key1[i] != key2[i]) {
            return 0; // Keys are different
        }
    }

    return 1; // Keys are the same
}