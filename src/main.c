#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/files.h"
#include "../include/menus.h"
#include "../include/encryption.h"


int main() {
    char users[] = "../data/users.txt";
    char* buffer;
    // cosas que determina el usuario
    char* new_user;
    char* new_password;
    char* new_password2;
    // cosas que se ocupan para nuevo usuario y que se generan con input del usuario
    unsigned char encrypted_password[1024 + EVP_MAX_BLOCK_LENGTH];
    unsigned char key[32];  // AES-256 key length in bytes (32)
    unsigned char iv[EVP_MAX_IV_LENGTH]; // length in bytes (16)
    unsigned char salt[8]; // Generate a random salt (8)

    int iv_length = EVP_CIPHER_iv_length(EVP_aes_256_cbc());
    int salt_length = sizeof(salt);
    // para registro en .txt
    char* base64_encryptedPassword;
    char* base64_salt;
    char* base64_iv;

    generateRandomSalt(salt, salt_length);
    generateRandomIV(iv, iv_length);

    while (1) {
        printf("----------------------------macua.com---------------------------- \n");
        printf("|                                                               | \n");
        printf("|         Bienvenido al administrador de contrasenas            | \n");
        printf("|          Siga las instrucciones para su registro!             | \n");
        printf("|                                                               | \n");
        printf("----------------------------------------------------------------- \n");
        printf("Ingrese su nombre de usuario (menos de 50 caracteres): ");
        new_user = get_string_50();

        if (!find_user(new_user, users, &buffer)) {
            while (1) {
                printf("Ingrese su contrasena (menos de 50 caracteres): ");
                new_password = get_string_50();
                printf("Confirme su contrasena: ");
                new_password2 = get_string_50();

                if (strcmp(new_password, new_password2) == 0) {
                    // contrasenas coinciden, proceso de encriptacion y registro
                    free(new_password2);

                    // Derive key from the master password
                    if (!deriveKey(new_password, key, salt)) {
                        fprintf(stderr, "Key derivation failed.\n");
                        return 1;
                    }

                    int encrypted_len = encryptAES256(new_password, strlen(new_password), key, encrypted_password, iv);

                    size_t output_size;

                    base64_encryptedPassword = base64_encode(encrypted_password, encrypted_len, &output_size);
                    base64_salt = base64_encode(salt, salt_length, &output_size);
                    base64_iv = base64_encode(iv, iv_length, &output_size);

                    new_line(users);

                    register_string(new_user, users);
                    doubledots(users);
                    register_string(base64_encryptedPassword, users);
                    doubledots(users);
                    register_string(base64_salt, users);
                    doubledots(users);
                    register_string(base64_iv, users);
                    

                    free(new_user);
                    free(new_password);
                    return 0;
                }
                else {
                    free(new_password);
                    free(new_password2);
                    printf("Las contrasenas no coinciden. Intente de nuevo. \n");
                }
            }
        }
        else {
            free(new_user);
            printf("El usuario ya existe. Intente con un nuevo usuario. \n");
        }
    }


    return 0;
}