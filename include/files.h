#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER 50

// 1 si existe el archivo 0 si no 
int file_exists(const char* name);
// returna string inferior a 50 caracteres, NULL si > 50
char* get_string_50();
// print string
void print_str(char *string);
// 1 si el regitro se hizo 0 si hubo un problema 
int register_string(const char *string, const char *file);
// va a la siguiente linea, 1 si lo logro 0 si no
int new_line(const char* file);
//pone un separador ':', 1 si lo logro 0 is no 
int doubledots(const char* file);
// retorna 1 si encuentra el usuario, guarda la contrasena asociada en la variable pass y retorna 0 si no encuentra el usuario y pass NULL
int find_user(char* input, char* users, char** pass);