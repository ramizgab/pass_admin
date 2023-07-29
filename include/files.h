#ifndef FILES_H
#define FILES_H

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER 50

typedef struct {
    size_t data_count;
}FileInfo;

typedef struct {
    char* title;
    char* username;
    char* url;
    unsigned char* encrypted_password;
}UserData;

// 1 si existe el archivo 0 si no 
int file_exists(const char* name);
// returna string inferior a 50 caracteres, NULL si > 50
char* get_string_50();
// 1 si el regitro se hizo 0 si hubo un problema 
int register_string(const char *string, const char *file);
// va a la siguiente linea, 1 si lo logro 0 si no
int new_line(const char* file);
//pone un separador espacio, 1 si lo logro 0 is no 
int space(const char* file);
// retorna 1 si encuentra el usuario, aloca y guarda contrasena encriptada, salt y vector inicial y retorna 0 si no encuentra el usuario y pass NULL en las tres variables
int find_user(char* input, char* users, char** pass, char **salt, char **vect);
// retorna 1 si encuentra el usuario
int check_user(char* input, char* users);
// retorna -1 si hubo un error de algun tipo, retorna int relacionado con el ultimo caracter en el archivo
int end_character(const char* file);
//
void init_bifile(const char* filename);
//
void adddata_bifile(const char* filename, const UserData data);
//
int search_name(const char* filename, const char* name);
//
void del_data(const char* filename, const char* name);
//
void print_all_bidata(const char* filename);
//
int bifile_exists(const char* filename);
//
char* file_path(const char* filename);

#endif