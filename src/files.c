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
            free(input);
            return NULL;

        }

        input[inputLength] = c;
        inputLength++;

    }

    return input;

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


int find_user(char* input, char* users, char** pass, char** salt, char** vect) {
    char delim[] = "|";
    FILE* file = fopen(users, "r");
    if (file == NULL) {
        printf("archivo no encontrado: %s\n", users);
        return 0;
    }

    char line[1450]; // Assuming each line in the file has at most 1450 characters

    while (fgets(line, sizeof(line), file) != NULL) {
        char* line_copy = strdup(line); // Make a copy of the line

        char* u = strtok(line_copy, delim);
        if (strcmp(input, u) == 0) {
            fclose(file);

            char* temp_pass = strtok(NULL, delim);
            *pass = strdup(temp_pass); // Allocate memory for password and copy it
            char* temp_salt = strtok(NULL, delim);
            *salt = strdup(temp_salt); // Allocate memory for salt and copy it
            char* temp_vect = strtok(NULL, delim);
            *vect = strdup(temp_vect); // Allocate memory for vect and copy it

            free(line_copy); // Free the dynamically allocated line_copy

            return 1; // Found the input string in the file
        }

        free(line_copy); // Free the dynamically allocated line_copy
    }

    fclose(file);
    *pass = NULL;
    *salt = NULL;
    *vect = NULL;

    return 0; // Input string not found in the file
}

int check_user(char* input, char* users) {
    char delim[] = "|";
    FILE* file = fopen(users, "r");
    if (file == NULL) {
        printf("archivo no encontrado: %s\n", users);
        return 0;
    }

    char line[1450]; // Assuming each line in the file has at most 1450 characters

    while (fgets(line, sizeof(line), file) != NULL) {
        char* line_copy = strdup(line); // Make a copy of the line

        char* u = strtok(line_copy, delim);
        if (strcmp(input, u) == 0) {
            fclose(file);

            free(line_copy); // Free the dynamically allocated line_copy

            return 1; // Found the input string in the file
        }

        free(line_copy); // Free the dynamically allocated line_copy
    }

    fclose(file);

    return 0; // Input string not found in the file
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

void init_bifile(const char* filename) {
    FileInfo counter;

    counter.data_count = 0;

    FILE* file = fopen(filename, "wb");
    if ( file ==  NULL) {
        printf("Failed initializing binary file %s \n", filename);
        return;
    }

    if (fwrite(&counter, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed inputting the data to the binary file.\n");
    }

    fclose(file);
}

void adddata_bifile(const char* filename, const UserData data) {
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Failed opening binary file: %s \n", filename);
        return;
    }

    FileInfo info;
    if (fread(&info, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed to read file %s information.\n", filename);
        fclose(file);
        return;
    }

    info.data_count++;

    fseek(file, 0, SEEK_SET);
    if (fwrite(&info, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed to write in file %s information.\n", filename);
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_END);
    if (fwrite(&data, sizeof(UserData), 1, file) != 1) {
        printf("Failed to write in file %s information.\n", filename);
        fclose(file);
        return;
    }
    fclose(file);

}

int search_name(const char* filename, const char* name) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed opening binary file: %s \n", filename);
        return -1;
    }

    FileInfo info;
     if (fread(&info, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed to read file %s information.\n", filename);
        fclose(file);
        return -1;
    }

    UserData* data = malloc(sizeof(UserData) * info.data_count);
    if (data == NULL) {
        printf("Failed allocating memory\n");
        return -1;
    }

    fseek(file, sizeof(FileInfo), SEEK_SET);

    if (fread(data, sizeof(UserData), info.data_count, file) != info.data_count) {
        printf("Failed to read in file %s information.\n", filename);
        fclose(file);
        free(data);
        return -1;
    }

    for (size_t i = 0; i < info.data_count; i++) {
        if ( strcmp(data[i].title, name) == 0) {
            printf("Name: %s\n", data[i].title);
            printf("Site: %s\n", data[i].url);
            printf("Username: %s\n", data[i].username);
            printf("Encrypted Password: ");
            for (size_t j = 0; j < 16; j++) {
                printf("%02X ", data[i].encrypted_password[i]);
            }
            printf("\n");
            return i;
        }
            
    }

    fclose(file);
    free(data);
    return -1;
}

void del_data(const char* filename, const char* name) {
    
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Failed opening binary file: %s \n", filename);
        return;
    }

    FileInfo info;
    if (fread(&info, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed to read file %s information.\n", filename);
        fclose(file);
        return;
    }

    UserData data;
    int found = 0;
    size_t indicator = 0;

    for (size_t i = 0; i < info.data_count; i++) {
        if (fread(&data, sizeof(UserData), 1, file) != 1) {
            printf("Failed to read in file %s information.\n", filename);
            fclose(file);
            return;
        }

        if ( strcmp(data.title, name) == 0) {
            found = 1;
            indicator = i;
            break;
        }
    }

    if (!found) {
        printf("Data with title %s is not found in file", name);
        fclose(file);
        return;
    }

    if (indicator == info.data_count - 1) {
        info.data_count--;
        fseek(file, 0, SEEK_SET);
        if (fwrite(&info, sizeof(FileInfo), 1, file) != 1) {
            printf("Failed to write file information.\n");
            fclose(file);
            return;
        }

        fclose(file);
    }
    else {
        
        for (size_t i = indicator; i < info.data_count -1; i++) {
            //leer
            if (fread(&data, sizeof(UserData), 1, file) != 1) {
            printf("Failed to read in file %s information.\n", filename);
            fclose(file);
            return;
            }
            // -2
            fseek(file, -2 * sizeof(UserData), SEEK_CUR);
            // escribir
            if (fwrite(&data, sizeof(UserData), 1, file) != 1) {
            printf("Failed to write in file %s information.\n", filename);
            fclose(file);
            return;
            }
            // +1
            fseek(file, sizeof(UserData), SEEK_CUR);
        }

        info.data_count--;
        fseek(file, 0, SEEK_SET);
        if (fwrite(&info, sizeof(FileInfo), 1, file) != 1) {
            printf("Failed to write file information.\n");
            fclose(file);
            return;
        }
        fclose(file);
    }

}

void print_all_bidata(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed opening binary file: %s\n", filename);
        return;
    }

    FileInfo info;
    if (fread(&info, sizeof(FileInfo), 1, file) != 1) {
        printf("Failed to read file %s information.\n", filename);
        fclose(file);
        return;
    }

    UserData* data = malloc(sizeof(UserData) * info.data_count);
    if (data == NULL) {
        printf("Failed allocating memory.\n");
        fclose(file);
        return;
    }

    fseek(file, sizeof(FileInfo), SEEK_SET);

    if (fread(data, sizeof(UserData), info.data_count, file) != info.data_count) {
        printf("Failed to read in file %s information.\n", filename);
        fclose(file);
        free(data);
        return;
    }

    for (size_t i = 0; i < info.data_count; i++) {
        printf("Data %zu:\n", i + 1);
        printf("Title: %s\n", data[i].title);
        printf("Username: %s\n", data[i].username);
        printf("URL: %s\n", data[i].url);
        printf("Encrypted Password: ");
        for (size_t j = 0; j < 16; j++) {
            printf("%02X ", data[i].encrypted_password[j]);
        }
        printf("\n\n");
    }

    fclose(file);
    free(data);
}

int bifile_exists(const char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file != NULL){
        fclose(file);
        return 1;
    }

    return 0;
}

char* file_path(const char* filename) {
    char* res = malloc(sizeof(char) * 100);
    strcpy(res,"../data/");
    strcat(res, filename);
    strcat(res,".bin");
    return res;
}