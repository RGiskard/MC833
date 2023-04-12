cmake_minimum_required(VERSION 3.10)

project(TCP_Project C)

# Obtenemos todos los archivos .c de la carpeta src
file(GLOB SOURCES "src/*.c")

# Creamos un ejecutable llamado tcp_simulation
add_executable(tcp_simulation ${SOURCES} main.c)

# Agregamos la carpeta include
target_include_directories(tcp_simulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Indicamos donde queremos que se cree el ejecutable
set_target_properties(tcp_simulation PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
