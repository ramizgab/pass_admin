 #include "../include/menus.h"

void menu_principal() {
    int numero = 0;

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
            printf("Buenisimo, bienvenido al menu login\n");
            break;  // Exit the loop
        }
        if (numero == 2) {
            printf("Buenisimo, bienvenido al menu de registro\n");
            break;  // Exit the loop
        }
        if (numero == 3) {
            printf("Buenisimo, Adios!\n");
            break;  // Exit the loop
        }

        printf("Opcion no valida. Por favor, intente nuevamente.\n\n");
    }
}


int login() { // da 1 si el login es correcto y 0 si mama el login
    char* pass = malloc(sizeof(char) * 100);

    while (1)
    {

        // Entrar el User
        //char input[100]; // Assuming the input has at most 100 characters
        char users[] = "data/users.txt";

        if (!file_exists(users)) {
            fprintf(stderr, "Error: archivo no existe\n");
            return 0;
        }

        printf("Entre su user: \n");
        char* input = get_string_50();

        // Remove the trailing newline character, if any
        size_t input_length = strlen(input);
        if (input[input_length - 1] == '\n')
            input[input_length - 1] = '\0';

        if (find_user(input, users, &pass))
        {
            size_t pass_length = strlen(pass);
            if (pass[pass_length - 1] != '\0')
                pass[pass_length - 1] = '\0';

            printf("%s\n", pass);
            // probar contrasena
            while (1)
            {
                printf("Entre su contrasena: \n");
                input = get_string_50();
                // Remove the trailing newline character, if any

                printf("input: %s\n", input);
                if (strcmp(input, pass) == 0)
                { // si la contrasena es la misma que pass.
                    printf("login exitoso\n");
                    return 1;
                }
                else
                {
                    printf("Contrasena incorrecta, intente de nuevo.\n");
                }
            }
        }
        else
        {
            printf("Error: User no encontrado. Intente de nuevo.\n");
        }
    }
}

int registrar() {

    char users[] = "../data/users.txt";
    char* buffer = malloc(sizeof(char)*50);
    // cosas que determina el usuariok, input
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

        if (find_user(new_user, users, &buffer) == 0) {
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

                    if (end_character(users) != '\n' || end_character(users) != '\r') {
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
            free(buffer);
            free(new_user);
            printf("El usuario ya existe. Intente con un nuevo usuario. \n");
        }
    } 
}