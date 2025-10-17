cmake_minimum_required(VERSION 3.20)
project(SistemaSanatorio CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURSES_NEED_WIDE TRUE)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# ---- ENCONTRAR LIBRERÍA DE HILOS ----
find_package(Threads REQUIRED)

add_executable(SistemaSanatorio
        src/main.cpp
        src/SistemaSanatorio.cpp
        src/Paciente.cpp
        src/Profesional.cpp
        src/Especialidad.cpp
        src/Sanatorio.cpp
        src/Turno.cpp
        ui/menu.cpp
)
target_include_directories(SistemaSanatorio PRIVATE ${CURSES_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/ui ${CMAKE_SOURCE_DIR}/src)

# ---- ENLAZAR LIBRERÍA DE HILOS ----
target_link_libraries(SistemaSanatorio PRIVATE ${CURSES_LIBRARIES} Threads::Threads)

if(NOT CURSES_LIBRARIES)
  target_link_libraries(SistemaSanatorio PRIVATE ncursesw)
endif()

# ====================================================================
# COPIAR SCRIPTS NECESARIOS AL DIRECTORIO DEL EJECUTABLE
# ====================================================================
# Esta instrucción se ejecuta DESPUÉS de compilar el target "SistemaSanatorio".
# Le dice a CMake que copie nuestro script desde el directorio de fuentes
# al directorio donde se generó el ejecutable final.
add_custom_command(TARGET SistemaSanatorio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/scripts/enviar_mail.py
        $<TARGET_FILE_DIR:SistemaSanatorio>/scripts/enviar_mail.py
        COMMENT "Copiando script de Python al directorio de compilacion..."
)