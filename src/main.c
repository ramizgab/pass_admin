
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/files.h"
#include "../include/menus.h"
#include "../include/encryption.h"


int main() {

    char users[] = "../data/users.txt";
    char* buffer = malloc(sizeof(char)*50);
    // cosas que determina el usuario, input
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

    int encrypted_len;

    generateRandomSalt(salt, salt_length);
    generateRandomIV(iv, iv_length);

    int out = 0;

    while (1) {
        printf("----------------------------macua.com---------------------------- \n");
        printf("|                                                               | \n");
        printf("|         Bienvenido al administrador de contrasenas            | \n");
        printf("|          Siga las instrucciones para su registro!             | \n");
        printf("|                                                               | \n");
        printf("----------------------------------------------------------------- \n");
        printf("Ingrese su nombre de usuario (menos de 50 caracteres): ");
        new_user = get_string_50();


        if (!check_user(new_user, users)) {
            while (1) {
                // new_password input and confirmation
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
                        return 0;
                    }

                    encrypted_len = encryptAES256(new_password, strlen(new_password), key, encrypted_password, iv);

                    printf("The length of the encrypted cipher is: %d\n", encrypted_len);

                    size_t output_size;

                    base64_encryptedPassword = base64_encode(encrypted_password, encrypted_len, &output_size);
                    printf("The size of the base64 encode is: %zu\n", output_size);
                    base64_salt = base64_encode(salt, salt_length, &output_size);
                    printf("The size of the base64 encode is: %zu\n", output_size);
                    base64_iv = base64_encode(iv, iv_length, &output_size);
                    printf("The size of the base64 encode is: %zu\n", output_size);

                    printf("Contrasena base 64: %s\n",base64_encryptedPassword);
                    printf("Salt base 64: %s\n",base64_salt);
                    printf("IV base 64: %s \n",base64_iv);

                    int character = end_character(users);

                    if ((character != 0) && (character != '\n')) {
                        new_line(users);
                    }


                    register_string(new_user, users);
                    space(users);
                    register_string(base64_encryptedPassword, users);
                    space(users);
                    register_string(base64_salt, users);
                    space(users);
                    register_string(base64_iv, users);


                    free(new_user);
                    free(new_password);

                    out = 1;

                }
                else {
                    free(new_password);
                    free(new_password2);
                    printf("Las contrasenas no coinciden. Intente de nuevo. \n");
                }
                if (out) {
                    break;
                }
            }

        }
        else {
            free(new_user);
            printf("El usuario ya existe. Intente con un nuevo usuario. \n");
        }
        if (out) {
            break;
        }
    }

    printf("User: ");
    char* input = get_string_50();
    char* password_b64 = malloc(sizeof(char)*100);
    if (password_b64 == NULL){
        printf("Erreur d'allocation");
        return 1;
    }
    char* salt_b64 = malloc(sizeof(char)*8);
    if (salt_b64 == NULL){
        printf("Erreur d'allocation");
        return 1;
    }
    char* iv_b64 = malloc(sizeof(char)*16);
    if (iv_b64 == NULL){
        printf("Erreur d'allocation");
        return 1;
    }

    char delim[] = "|";
    FILE* file = fopen("../data/users.txt", "r");
    if (file == NULL)
    {
        printf(" \"%s\" no encontrado\n ", "../data/users.txt");
        return 0;
    }

    char line[1000]; // Assuming each line in the file has at most 100 characters

    while (fgets(line, sizeof(line), file) != NULL)
    {

        char* u =strtok(line, delim);
        if (strcmp(input, u) == 0)
        {
            fclose(file);

            strcpy(password_b64, strtok(NULL, delim));
            strcpy(salt_b64, strtok(NULL, delim));
            strcpy(iv_b64, strtok(NULL, delim));

            break;

        }
    }

    printf("Password encriptado: %s \n",password_b64);
    printf("Salt: %s \n",salt_b64);
    printf("iv: %s \n",iv_b64);

    printf("Password: ");
    char* pass = get_string_50();

    size_t password_blen;
    unsigned char* password_b = base64_decode(password_b64, strlen(password_b64), &password_blen);
    printf("The size of the base64 encode is: %zu\n", password_blen);

    size_t salt_blen;
    unsigned char* salt_b = base64_decode(salt_b64, strlen(salt_b64), &salt_blen);
    printf("The size of the base64 encode is: %zu\n", salt_blen);

    size_t iv_blen;
    unsigned char* iv_b= base64_decode(iv_b64, strlen(iv_b64), &iv_blen);
    printf("The size of the base64 encode is: %zu\n", iv_blen);

    unsigned char keyt[32];

    if (!deriveKey(pass, keyt, salt_b)) {
        fprintf(stderr, "Key derivation failed.\n");
        return 0;
    }

    if(!compare_binary_keys(keyt, 32, key, 32)) {
        printf("No son iguales!\n");

    }
    else {
        printf("Son iguales!\n");
    }

    if (!compare_binary_keys(password_b, password_blen, encrypted_password, encrypted_len)) {
        printf("Contrasena encriptadas no son iguales\n");
    }
    else {
        printf("Contrasenas encriptadas son iguales \n");
    }
    if (!compare_binary_keys(salt_b, salt_blen, salt, salt_length)) {
        printf("Salt no son iguales\n");
    }
    else {
        printf("Salt son iguales \n");
    }
    if (!compare_binary_keys(iv_b, iv_blen, iv, iv_length)) {
        printf("iv no son iguales\n");
    }
    else {
        printf("iv son iguales \n");
    }

    char* output = malloc(sizeof(char)*50);

    if (output == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }

    int output_len = decryptAES256(password_b, password_blen, keyt, output, iv_b);

    printf("La contrasena desencriptada da %s\n", output);

    if (strcmp(output, pass) == 0) {
        printf("access granted!\n");
    }
    else {
        printf("Access denied");
    }

    return 0;

}