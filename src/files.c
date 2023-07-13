#include "../include/files.h"


int file_exists(const char* name) {

    FILE* file = fopen(name, "r");

    if (file != NULL) {
        fclose(file);
        return 1;
    }

    return 0;
}

char* get_string_50() {

    char* input = malloc(sizeof(char) * INITIAL_BUFFER);

    if (input == NULL) { // error malloc
        fprintf(stderr, "Error allocating memory");
        exit(1);
    }

    size_t buffer_size = INITIAL_BUFFER;
    size_t inputLength = 0;
    int c;

    while (1) {

        c = getchar();

        if (inputLength == 0 && (c == '\n' || c == '\r'))
            continue;

        if (c == '\n' || c == EOF) {
            input[inputLength] = '\0';
            break;
        }

        if (inputLength == INITIAL_BUFFER - 1) {
            fprintf(stderr, "String muy largo xd, > 50\n");
            return NULL;

        }

        input[inputLength] = c;
        inputLength++;

    }

    return input;

}

void print_str(char* string) {
    int i = 0;
    while (string[i] != '\0') {
        printf("%c", string[i]);
        i++;
    }
    printf("\n");
}

int register_string(const char* string, const char* file) {

    if (!file_exists(file)) {
        fprintf(stderr, "Error: archivo \"%s\" no existe", file);
        return 0;
    }

    FILE* archivo = fopen(file, "a");

    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", file);
        return 0;
    }

    fputs(string, archivo);

    fclose(archivo);

    return 1;

}

int new_line(const char* file) {
    if (!file_exists(file)) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", file);
        return 0;
    }

    FILE* archivo = fopen(file, "a");

    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", file);
        return 0;
    }

    fputc('\n',archivo);

    fclose(archivo);

    return 1;
}

int space(const char* file) {
    if (!file_exists(file)) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", file);
        return 0;
    }

    FILE* archivo = fopen(file, "a");

    if (archivo == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", file);
        return 0;
    }

    fputc('|', archivo);

    fclose(archivo);

    return 1;
}


int find_user(char* input, char* users, char** pass, char **salt, char **vect) {

    char delim[] = "|";
    FILE* file = fopen(users, "r");
    if (file == NULL)
    {
        printf("archivo no encontrado: %s\n", users);
        return 0;
    }

    char line[1450]; // Assuming each line in the file has at most 100 characters

    while (fgets(line, sizeof(line), file) != NULL)
    {

        char* u =strtok(line, delim);
        if (strcmp(input, u) == 0)
        {
            fclose(file);


            char* temp_pass = strtok(NULL, delim);
            *pass = malloc(strlen(temp_pass) + 1); // Allocate memory for password (+1 for null terminator)
            strcpy(*pass, temp_pass);

            char* temp_salt = strtok(NULL, delim);
            *salt = malloc(strlen(temp_salt) + 1);
            strcpy(*salt, temp_salt);

            char* temp_vect = strtok(NULL, delim);
            *vect = malloc(strlen(temp_vect) + 1);
            strcpy(*vect, temp_vect);

            return 1; // Found the input string in the file
        }
    }

    *pass = NULL;
    *salt= NULL;
    *vect = NULL;

    fclose(file);
    return 0; // Input string not found in the file
}

int check_user(char* input, char* users) {

    char delim[] = "|";
    FILE* file = fopen(users, "r");
    if (file == NULL)
    {
        printf("archivo no encontrado: %s\n", users);
        return 0;
    }

    char line[1450]; // Assuming each line in the file has at most 100 characters

    while (fgets(line, sizeof(line), file) != NULL)
    {

        char* u =strtok(line, delim);
        if (strcmp(input, u) == 0)
        {
            fclose(file);

            return 1; // Found the input string in the file
        }
    }
    fclose(file);
    return 0; // Input string not found in the file
}

int end_character(const char* file) {

    FILE* archivo = fopen(file, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Failed to open the file \"%s\".\n", file);
        return -1;
    }

    fseek(archivo, 0, SEEK_END);
    long file_size = ftell(archivo);

    if (file_size == 0) {
        fclose(archivo);
        return 0; // File is empty
    }

    fseek(archivo, -1, SEEK_END);
    int last_char = fgetc(archivo);

    fclose(archivo);
    return last_char;
}