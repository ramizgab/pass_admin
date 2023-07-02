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
