#include "utils.h"

// Definición en algún archivo fuente
sqlite3 *db = NULL;
// Función para leer el archivo de configuración y obtener el valor de database
int read_config_file() {
    FILE *fp;
    char buffer[256];
    int database = 0;

    fp = fopen("conf.txt", "r");
    if (fp == NULL) {
        printf("Error al abrir el archivo de configuración.\n");
        exit(1);
    }

    // Leer línea por línea
    while (fgets(buffer, sizeof(buffer), fp)) {
        char *param = strtok(buffer, " \t\n");
        char *value = strtok(NULL, " \t\n");

        if (param == NULL || value == NULL) {
            return 0;
        }

        // Comparar los parámetros con las opciones posibles
        if (strcmp(param, "database") == 0) {
            // Obtener el valor después del @
            char *at = strchr(value, '@');
            if (at != NULL) {
                int val = atoi(at+1);
                if (val == 1) {
                    database = 1;
                }
            }
        }
    }

    fclose(fp);

    return database;
}

int insert_usuario(Usuario *usuario) {
    char sql[200];
    sqlite3_stmt *stmt;
    
    sprintf(sql, "INSERT INTO usuario (email, nome, sobrenome, residencia, formacao_academica, ano_formatura) VALUES ('%s', '%s', '%s', '%s', '%s', %d);",
            usuario->email, usuario->nome, usuario->sobrenome, usuario->residencia, usuario->formacao_academica, usuario->ano_formatura);
            
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error inserting data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_finalize(stmt);
    
    return 0;
}

int update_usuario(Usuario *usuario) {
    char sql[200];
    sqlite3_stmt *stmt;
    
    sprintf(sql, "UPDATE usuario SET nome='%s', sobrenome='%s', residencia='%s', formacao_academica='%s', ano_formatura=%d WHERE email='%s';",
            usuario->nome, usuario->sobrenome, usuario->residencia, usuario->formacao_academica, usuario->ano_formatura, usuario->email);
            
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error updating data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_finalize(stmt);
    
    return 0;
}

int delete_usuario(char *email) {
    char sql[100];
    sqlite3_stmt *stmt;
    
    sprintf(sql, "DELETE FROM usuario WHERE email='%s';", email);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_finalize(stmt);
    
    return 0;
}

int select_usuario(char *email) {
    char sql[100];
    sqlite3_stmt *stmt;
    
    sprintf(sql, "SELECT * FROM usuario WHERE email='%s';", email);
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_ROW) {
        fprintf(stderr, "Error selecting data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    printf("Email: %s, Nome: %s, Sobrenome: %s, Residencia: %s, Formacao academica: %s, Ano de formatura: %d\n",
           sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
           sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_int(stmt, 5));
    
    sqlite3_finalize(stmt);
    
    return 0;
}

int select_all_usuarios() {
    char sql[] = "SELECT * FROM usuario;";
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("Email: %s, Nome: %s, Sobrenome: %s, Residencia: %s, Formacao academica: %s, Ano de formatura: %d\n",
               sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2),
               sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4), sqlite3_column_int(stmt, 5));
    }
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error selecting data: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    
    sqlite3_finalize(stmt);
    
    return 0;
}


// Funciones de utilidad
void db_error(sqlite3 *db) {
    printf("Error: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    exit(1);
}

void create_tables(sqlite3 *db) {
    char *err_msg = NULL;

    // Crear tabla Usuario
    char *sql = "CREATE TABLE IF NOT EXISTS Usuario ("
                "email TEXT PRIMARY KEY UNIQUE, "
                "nome TEXT NOT NULL, "
                "sobrenome TEXT NOT NULL, "
                "residencia TEXT NOT NULL, "
                "formacao_academica TEXT NOT NULL, "
                "ano_formatura INT NOT NULL);";

    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Error creando la tabla Usuario: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }

    // Crear tabla Habilidade
    sql = "CREATE TABLE IF NOT EXISTS Habilidade ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "email TEXT NOT NULL, "
          "habilidade TEXT NOT NULL, "
          "FOREIGN KEY (email) REFERENCES Usuario(email));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Error creando la tabla Habilidade: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }

    // Crear tabla Imagem
    sql = "CREATE TABLE IF NOT EXISTS Imagem ("
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "email TEXT NOT NULL, "
          "nome TEXT NOT NULL, "
          "imagem BLOB NOT NULL, "
          "FOREIGN KEY (email) REFERENCES Usuario(email));";

    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Error creando la tabla Imagem: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}



int conect() {
    int rc = sqlite3_open(DATABASE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    create_tables(db); // Llamada a la función para crear las tablas si no existen

    return 0;
}

void desconect() {
    sqlite3_close(db);
}