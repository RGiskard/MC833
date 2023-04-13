#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define DATABASE "database.db"
extern sqlite3 *db;
int read_config_file();



// Estructuras
typedef struct {
    char email[50];
    char nome[50];
    char sobrenome[50];
    char residencia[50];
    char formacao_academica[50];
    int ano_formatura;
} Usuario;

typedef struct {
    char email[50];
    char habilidade[50];
} Habilidade;

typedef struct {
    char email[50];
    char nome[50];
    char imagem[2000];
} Imagem;


int insert_usuario(Usuario *usuario);
int update_usuario(Usuario *usuario);
int delete_usuario(char *email);
int select_usuario(char *email);
int select_all_usuarios();
void db_error(sqlite3 *db);
void create_tables(sqlite3 *db);
int conect();
void desconect();
#endif /* SERVER_H */