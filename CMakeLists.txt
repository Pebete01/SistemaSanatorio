cmake_minimum_required(VERSION 3.20)
project(SistemaSanatorio CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURSES_NEED_WIDE TRUE)
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# ---- ENCONTRAR LIBRERÍA DE HILOS ----
find_package(Threads REQUIRED)

# ---- ENCONTRAR CURL (para geocodificación)
find_package(CURL REQUIRED) # <-- NUEVO

# ---- ENCONTRAR nlohmann_json (para parsear JSON)
find_package(nlohmann_json 3.2.0 REQUIRED) # <-- NUEVO

add_executable(SistemaSanatorio
        src/main.cpp
        src/SistemaSanatorio.cpp
        src/Paciente.cpp
        src/Profesional.cpp
        src/Especialidad.cpp
        src/Sanatorio.cpp
        src/Turno.cpp
        ui/menu.cpp
        src/GeocodificadorAPI.cpp
        src/HTTPClient.cpp
        src/DatosMock.cpp
        src/DatosMock.h
        src/InterfazUsuario.cpp
        src/InterfazUsuario.h
)
target_include_directories(SistemaSanatorio PRIVATE ${CURSES_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/ui ${CMAKE_SOURCE_DIR}/src)

# ---- ENLAZAR LIBRERÍA DE HILOS ----
target_link_libraries(SistemaSanatorio PRIVATE
        ${CURSES_LIBRARIES}
        Threads::Threads
        CURL::libcurl              # <-- NUEVO
        nlohmann_json::nlohmann_json  # <-- NUEVO
)
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
        COMMENT "Copiando script de enviar_mail al directorio de compilacion..."
)

add_custom_command(TARGET SistemaSanatorio POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/scripts/ia_server.py
        $<TARGET_FILE_DIR:SistemaSanatorio>/scripts/ia_server.py
        COMMENT "Copiando script de ia_server al directorio de compilacion..."
)

target_link_libraries(${PROJECT_NAME} PRIVATE curl ws2_32)
#ver si saco esto