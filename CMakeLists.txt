cmake_minimum_required(VERSION 3.27)
project(SistemaSanatorio)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(SistemaSanatorio
        src/Especialidad.cpp
        src/Especialidad.h
        src/main.cpp
        src/Persona.cpp
        src/Persona.h
        src/Profesional.cpp
        src/Profesional.h
        src/SistemaSanatorio.cpp
        src/SistemaSanatorio.h
        src/Turno.cpp
        src/Turno.h
        src/Paciente.cpp
        src/Paciente.h
        src/Sanatorio.cpp
        src/Sanatorio.h)
