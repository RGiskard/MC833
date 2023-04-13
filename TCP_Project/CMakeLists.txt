#sudo apt-get install libjson-c-dev Ubuntu
#brew install json-c
#sudo apt-get install libsqlite3-dev
#brew install sqlite3

cmake_minimum_required(VERSION 3.10)
project(TCP_Project C)

# Buscamos la biblioteca SQLite en el sistema
find_package(SQLite3 REQUIRED)

# Obtenemos todos los archivos .c de la carpeta src
file(GLOB SOURCES "src/*.c")

# Creamos un ejecutable llamado tcp_simulation
add_executable(tcp_simulation ${SOURCES} main.c)

# Agregamos la carpeta include
target_include_directories(tcp_simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Especificamos las bibliotecas que se vincularán con el ejecutable
target_link_libraries(tcp_simulation json-c SQLite::SQLite3)

# Indicamos donde queremos que se cree el ejecutable
set_target_properties(tcp_simulation PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
