#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>

// Function to derive the key from the master password using PBKDF2
int deriveKey(const char* password, unsigned char* key, unsigned char* salt);
// Function to encrypt text using AES 256 CBC protocol
int encryptAES256(const char* plaintext, int plaintext_len, const unsigned char* key, unsigned char* ciphertext, const unsigned char* iv);
// Function to decrypt text in AES 256 CBC protocol
int decryptAES256(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, char* plaintext, const unsigned char* iv);
// Generates random initializing vector of the desired length
void generateRandomIV(unsigned char* iv, int iv_length);
// Generates random salt of the desired length
void generateRandomSalt(unsigned char* salt, int salt_length);
// Encodes binary arrays to base64 
char* base64_encode(const unsigned char* data, size_t input_length, size_t* output_length);
// Decodes base64 arrays to binary
unsigned char* base64_decode(const char* encoded_data, size_t input_length, size_t* output_length);
// Compares to binary arrays
int compare_binary_keys(const unsigned char* key1, size_t key1_length, const unsigned char* key2, size_t key2_length);