#include "../include/menus.h"

void menu_principal() {
    int numero = 0;
    int skip = 0;

    while (1) {
        printf("----------------------------macua.com---------------------------- \n");
        printf("|   1.Login                                                     | \n");
        printf("|   2.Registrarse                                               | \n");
        printf("|   3.Salir                                                     | \n");
        printf("----------------------------------------------------------------- \n");
        printf("Ingrese el numero deseado: ");

        if (scanf("%d", &numero) != 1) {
            printf("Error: numero no valido. Por favor, intente nuevamente.\n");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;  // Ask for input again
        }
        printf("\n");

        if (numero == 1) {
            char * archivo_user;
            if(!login(&archivo_user)){
                printf("login fracasado\n");
            }
            else{
                if(archivo_user!=NULL){
                    printf("se abre el archivo %s.bin\n", archivo_user);
                }
                printf("login exitoso\n");
            }
            printf("Buenisimo, bienvenido al menu login\n");
            break;  // Exit the loop
        }
        if (numero == 2) {
            if (!registrar()){
                printf("Hubo un error con el registro. \n");
            }
            else {
                printf("El registro fue todo un exito!\n");
            }
            skip = 1;
            printf("\n");
        }
        if (numero == 3) {
            printf("Buenisimo, Adios!\n");
            break;  // Exit the loop
        }

        if (!skip) {
            printf("Opcion no valida. Por favor, intente nuevamente.\n\n");
        }
        skip = 0;
    }
}


int login(char **archivo_user) { // da 1 si el login es correcto y 0 si mama el login
    char* pass;
    char* salt;
    char* iv;

    while (1) {

        char users[] = "../data/users.txt";

        if (!file_exists(users)) {
            fprintf(stderr, "Error: archivo no existe\n");
            return 0;
        }

        printf("Entre su user: \n");
        char* user_input = get_string_50();

        if (find_user(user_input, users, &pass, &salt, &iv))
        {
            size_t pass_length = strlen(pass);
            /*if (pass[pass_length - 1] != '\0')
                pass[pass_length - 1] = '\0';*/

            printf("Contrasena encriptada base64: %s, salt base64: %s, iv base64: %s \n", pass, salt, iv);
            // probar contrasena
            while (1)
            {
                printf("Entre su contrasena: \n");
                char* pass_input = get_string_50();
                // Remove the trailing newline character, if any

                printf("input: %s\n", pass_input);

                size_t encrypted_password_blen;
                unsigned char* encrypted_password_b = base64_decode(pass, strlen(pass), &encrypted_password_blen);

                size_t salt_blen;
                unsigned char* salt_b = base64_decode(salt, strlen(salt), &salt_blen);

                size_t iv_blen;
                unsigned char* iv_b = base64_decode(iv, strlen(iv), &iv_blen);

                unsigned char key[32];

                if (!deriveKey(pass_input, key, salt_b)) {
                    fprintf(stderr, "Key derivation failed.\n");
                    return 0;
                }

                char output[50];

                if (output == NULL) {
                    printf("Error allocating memory\n");
                    return 1;
                }

                int output_len = encryptAES256(pass_input, strlen(pass_input), key, output, iv_b);

                printf("Despues de encriptar da: %s\n", output);
                printf("contrasena encriptada binaria del archivo: %s\n", encrypted_password_b);

                if (compare_binary_keys(output, output_len, encrypted_password_b, encrypted_password_blen))//(strcmp(pass_input, output) == 0)
                { // si la contrasena es la misma que pass.
                    printf("login exitoso\n");

                    *archivo_user=strdup(user_input);
                    
                    free(user_input);
                    free(pass_input);
                   /* 
                    free(pass);
                    free(salt);
                    free(iv);
*/
                    free(encrypted_password_b);
                    free(salt_b);
                    free(iv_b);

                    return 1;
                }
                else {
                    free(user_input);
                    free(pass_input);
                   /* 
                    free(pass);
                    free(salt);
                    free(iv);*/

                    free(encrypted_password_b);
                    free(salt_b);
                    free(iv_b);
                    printf("Contrasena incorrecta, intente de nuevo.\n");
                }
            }
        }
        else {
            free(user_input);
            printf("Error: User no encontrado. Intente de nuevo.\n");
        }
    }
}

int registrar() {

    char users[] = "../data/users.txt";

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

                    int encrypted_len = encryptAES256(new_password, strlen(new_password), key, encrypted_password, iv);

                    size_t output_size;

                    base64_encryptedPassword = base64_encode(encrypted_password, encrypted_len, &output_size);
                    base64_salt = base64_encode(salt, salt_length, &output_size);
                    base64_iv = base64_encode(iv, iv_length, &output_size);

                    if ((end_character(users) != 0) && (end_character(users) != '\n')) {
                        new_line(users);
                    }

                    register_string(new_user, users);
                    space(users);
                    register_string(base64_encryptedPassword, users);
                    space(users);
                    register_string(base64_salt, users);
                    space(users);
                    register_string(base64_iv, users);

                    free(base64_encryptedPassword);
                    free(base64_salt);
                    free(base64_iv);

                    char* path = file_path(new_user);

                    init_bifile(path);

                    free(path);
                    free(new_user);
                    free(new_password);
                    return 1;
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
}