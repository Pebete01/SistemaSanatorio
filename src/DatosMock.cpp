//
// Created by tomyl on 26/10/2025.
//

#include "DatosMock.h"

// Includes necesarios
#include "SistemaSanatorio.h"
#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "Sanatorio.h"
#include "menu.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>

/*
 * @brief Convierte un time_point de C++ a un string "YYYY-MM-DD HH:MM".
 */
static std::string formatearFechaHora(const std::chrono::system_clock::time_point& time_point)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

/**
 * @brief Implementación de la función para cargar datos mock realistas.
 */
void cargarDatosMock(EmpresaSanatorio &app)
{
    message_center("Mock Data", "Iniciando carga de datos de prueba...");

    // ============================================================================
    // 1️⃣ ESPECIALIDADES (5)
    // ============================================================================
    app.agregarEspecialidad(new Especialidad(1, "Cardiologia"));
    app.agregarEspecialidad(new Especialidad(2, "Dermatologia"));
    app.agregarEspecialidad(new Especialidad(3, "Traumatologia"));
    app.agregarEspecialidad(new Especialidad(4, "Pediatria"));
    app.agregarEspecialidad(new Especialidad(5, "Clinica Medica"));

    // ============================================================================
    // 2️⃣ SANATORIOS (5) - Direcciones reales de CABA
    // ============================================================================
    // Sanatorio 0
    app.agregarSanatorio(new Sanatorio(
            "Sanatorio Guemes",
            "Av. Corrientes 2678, CABA",
            -34.6042, -58.3988  // Coordenadas aproximadas Corrientes y Paso
    ));

    // Sanatorio 1
    app.agregarSanatorio(new Sanatorio(
            "Sanatorio Otamendi",
            "Av. Belgrano 1550, CABA",
            -34.6125, -58.3880  // Coordenadas aproximadas Belgrano y Salta
    ));

    // Sanatorio 2
    app.agregarSanatorio(new Sanatorio(
            "Clinica Bazterrica",
            "Juncal 3002, CABA",
            -34.5950, -58.4010  // Coordenadas aproximadas Juncal y Ayacucho
    ));

    // Sanatorio 3
    app.agregarSanatorio(new Sanatorio(
            "Hospital Italiano",
            "Juan D. Peron 4190, CABA",
            -34.6320, -58.4050  // Coordenadas aproximadas Almagro
    ));

    // Sanatorio 4
    app.agregarSanatorio(new Sanatorio(
            "Sanatorio Trinidad Palermo",
            "Av. Dorrego 2201, CABA",
            -34.5780, -58.4280  // Coordenadas aproximadas Palermo
    ));

    // ============================================================================
    // 3️⃣ PROFESIONALES (10) - Nombres realistas
    // ============================================================================
    Especialidad* cardio = app.buscarEspecialidadPorId(1);
    Especialidad* derma = app.buscarEspecialidadPorId(2);
    Especialidad* trauma = app.buscarEspecialidadPorId(3);
    Especialidad* pedia = app.buscarEspecialidadPorId(4);
    Especialidad* clinica = app.buscarEspecialidadPorId(5);

    // Profesional 1 - Cardiólogo
    if (cardio) {
        app.agregarProfesional(new Profesional(
                101, *cardio, 1, "Roberto", "Fernandez", "rfernandez@hospital.com"
        ));
    }

    // Profesional 2 - Cardiólogo
    if (cardio) {
        app.agregarProfesional(new Profesional(
                102, *cardio, 2, "Maria", "Lopez", "mlopez@clinica.com"
        ));
    }

    // Profesional 3 - Dermatólogo
    if (derma) {
        app.agregarProfesional(new Profesional(
                103, *derma, 3, "Carlos", "Martinez", "cmartinez@sanatorio.com"
        ));
    }

    // Profesional 4 - Dermatóloga
    if (derma) {
        app.agregarProfesional(new Profesional(
                104, *derma, 4, "Ana", "Rodriguez", "arodriguez@medico.com"
        ));
    }

    // Profesional 5 - Traumatólogo
    if (trauma) {
        app.agregarProfesional(new Profesional(
                105, *trauma, 5, "Jorge", "Gonzalez", "jgonzalez@trauma.com"
        ));
    }

    // Profesional 6 - Traumatóloga
    if (trauma) {
        app.agregarProfesional(new Profesional(
                106, *trauma, 6, "Laura", "Perez", "lperez@ortopedia.com"
        ));
    }

    // Profesional 7 - Pediatra
    if (pedia) {
        app.agregarProfesional(new Profesional(
                107, *pedia, 7, "Diego", "Sanchez", "dsanchez@pediatria.com"
        ));
    }

    // Profesional 8 - Pediatra
    if (pedia) {
        app.agregarProfesional(new Profesional(
                108, *pedia, 8, "Lucia", "Ramirez", "lramirez@ninos.com"
        ));
    }

    // Profesional 9 - Clínico
    if (clinica) {
        app.agregarProfesional(new Profesional(
                109, *clinica, 9, "Martin", "Torres", "mtorres@clinica.com"
        ));
    }

    // Profesional 10 - Clínica
    if (clinica) {
        app.agregarProfesional(new Profesional(
                110, *clinica, 10, "Sofia", "Gomez", "sgomez@medicina.com"
        ));
    }

    // ============================================================================
    // 4️⃣ ASIGNAR PROFESIONALES Y ESPECIALIDADES A SANATORIOS
    // ============================================================================

    // SANATORIO 0 - Guemes (Cardiología, Dermatología, Clínica)
    app.agregarEspecialidadASanatorio(0, cardio);
    app.agregarEspecialidadASanatorio(0, derma);
    app.agregarEspecialidadASanatorio(0, clinica);

    app.agregarProfesionalASanatorio(0, app.buscarProfesionalPorId(1));  // Fernandez - Cardio
    app.agregarProfesionalASanatorio(0, app.buscarProfesionalPorId(3));  // Martinez - Derma
    app.agregarProfesionalASanatorio(0, app.buscarProfesionalPorId(9));  // Torres - Clínica

    // SANATORIO 1 - Otamendi (Cardiología, Traumatología, Pediatría)
    app.agregarEspecialidadASanatorio(1, cardio);
    app.agregarEspecialidadASanatorio(1, trauma);
    app.agregarEspecialidadASanatorio(1, pedia);

    app.agregarProfesionalASanatorio(1, app.buscarProfesionalPorId(2));  // Lopez - Cardio
    app.agregarProfesionalASanatorio(1, app.buscarProfesionalPorId(5));  // Gonzalez - Trauma
    app.agregarProfesionalASanatorio(1, app.buscarProfesionalPorId(7));  // Sanchez - Pediatría

    // SANATORIO 2 - Bazterrica (Dermatología, Traumatología, Clínica)
    app.agregarEspecialidadASanatorio(2, derma);
    app.agregarEspecialidadASanatorio(2, trauma);
    app.agregarEspecialidadASanatorio(2, clinica);

    app.agregarProfesionalASanatorio(2, app.buscarProfesionalPorId(4));  // Rodriguez - Derma
    app.agregarProfesionalASanatorio(2, app.buscarProfesionalPorId(6));  // Perez - Trauma
    app.agregarProfesionalASanatorio(2, app.buscarProfesionalPorId(10)); // Gomez - Clínica

    // SANATORIO 3 - Hospital Italiano (Todas las especialidades)
    app.agregarEspecialidadASanatorio(3, cardio);
    app.agregarEspecialidadASanatorio(3, derma);
    app.agregarEspecialidadASanatorio(3, trauma);
    app.agregarEspecialidadASanatorio(3, pedia);
    app.agregarEspecialidadASanatorio(3, clinica);

    app.agregarProfesionalASanatorio(3, app.buscarProfesionalPorId(1));  // Fernandez - Cardio
    app.agregarProfesionalASanatorio(3, app.buscarProfesionalPorId(3));  // Martinez - Derma
    app.agregarProfesionalASanatorio(3, app.buscarProfesionalPorId(5));  // Gonzalez - Trauma
    app.agregarProfesionalASanatorio(3, app.buscarProfesionalPorId(8));  // Ramirez - Pediatría
    app.agregarProfesionalASanatorio(3, app.buscarProfesionalPorId(9));  // Torres - Clínica

    // SANATORIO 4 - Trinidad Palermo (Pediatría, Dermatología)
    app.agregarEspecialidadASanatorio(4, pedia);
    app.agregarEspecialidadASanatorio(4, derma);

    app.agregarProfesionalASanatorio(4, app.buscarProfesionalPorId(7));  // Sanchez - Pediatría
    app.agregarProfesionalASanatorio(4, app.buscarProfesionalPorId(8));  // Ramirez - Pediatría
    app.agregarProfesionalASanatorio(4, app.buscarProfesionalPorId(4));  // Rodriguez - Derma

    // ============================================================================
    // 5️⃣ CONFIGURAR DISPONIBILIDAD HORARIA DE PROFESIONALES
    // ============================================================================

    // Profesional 1 - Fernandez (Cardio) - Trabaja en Sanatorio 0 y 3
    Profesional* prof1 = app.buscarProfesionalPorId(1);
    if (prof1) {
        // En Sanatorio 0 (Guemes)
        prof1->agregarDisponibilidad(0, "Lunes", 8, 0, 12, 0);
        prof1->agregarDisponibilidad(0, "Miércoles", 14, 0, 18, 0);

        // En Sanatorio 3 (Hospital Italiano)
        prof1->agregarDisponibilidad(3, "Martes", 9, 0, 13, 0);
        prof1->agregarDisponibilidad(3, "Jueves", 15, 0, 19, 0);

        prof1->setDuracionTurno(30);
    }

    // Profesional 2 - Lopez (Cardio) - Trabaja en Sanatorio 1
    Profesional* prof2 = app.buscarProfesionalPorId(2);
    if (prof2) {
        prof2->agregarDisponibilidad(1, "Lunes", 10, 0, 14, 0);
        prof2->agregarDisponibilidad(1, "Viernes", 8, 0, 12, 0);
        prof2->setDuracionTurno(30);
    }

    // Profesional 3 - Martinez (Derma) - Trabaja en Sanatorio 0 y 3
    Profesional* prof3 = app.buscarProfesionalPorId(3);
    if (prof3) {
        prof3->agregarDisponibilidad(0, "Martes", 10, 0, 14, 0);
        prof3->agregarDisponibilidad(0, "Jueves", 16, 0, 20, 0);
        prof3->agregarDisponibilidad(3, "Miércoles", 9, 0, 13, 0);
        prof3->setDuracionTurno(45);
    }

    // Profesional 4 - Rodriguez (Derma) - Trabaja en Sanatorio 2 y 4
    Profesional* prof4 = app.buscarProfesionalPorId(4);
    if (prof4) {
        prof4->agregarDisponibilidad(2, "Lunes", 14, 0, 18, 0);
        prof4->agregarDisponibilidad(4, "Martes", 9, 0, 13, 0);
        prof4->agregarDisponibilidad(4, "Jueves", 10, 0, 14, 0);
        prof4->setDuracionTurno(45);
    }

    // Profesional 5 - Gonzalez (Trauma) - Trabaja en Sanatorio 1 y 3
    Profesional* prof5 = app.buscarProfesionalPorId(5);
    if (prof5) {
        prof5->agregarDisponibilidad(1, "Lunes", 8, 0, 12, 0);
        prof5->agregarDisponibilidad(1, "Miércoles", 14, 0, 18, 0);
        prof5->agregarDisponibilidad(3, "Viernes", 9, 0, 13, 0);
        prof5->setDuracionTurno(30);
    }

    // Profesional 6 - Perez (Trauma) - Trabaja en Sanatorio 2
    Profesional* prof6 = app.buscarProfesionalPorId(6);
    if (prof6) {
        prof6->agregarDisponibilidad(2, "Martes", 8, 0, 12, 0);
        prof6->agregarDisponibilidad(2, "Jueves", 14, 0, 18, 0);
        prof6->setDuracionTurno(30);
    }

    // Profesional 7 - Sanchez (Pediatría) - Trabaja en Sanatorio 1 y 4
    Profesional* prof7 = app.buscarProfesionalPorId(7);
    if (prof7) {
        prof7->agregarDisponibilidad(1, "Lunes", 9, 0, 13, 0);
        prof7->agregarDisponibilidad(1, "Miércoles", 15, 0, 19, 0);
        prof7->agregarDisponibilidad(4, "Martes", 10, 0, 14, 0);
        prof7->agregarDisponibilidad(4, "Viernes", 8, 0, 12, 0);
        prof7->setDuracionTurno(30);
    }

    // Profesional 8 - Ramirez (Pediatría) - Trabaja en Sanatorio 3 y 4
    Profesional* prof8 = app.buscarProfesionalPorId(8);
    if (prof8) {
        prof8->agregarDisponibilidad(3, "Lunes", 14, 0, 18, 0);
        prof8->agregarDisponibilidad(4, "Martes", 8, 0, 12, 0);
        prof8->agregarDisponibilidad(4, "Jueves", 14, 0, 18, 0);
        prof8->setDuracionTurno(30);
    }

    // Profesional 9 - Torres (Clínica) - Trabaja en Sanatorio 0 y 3
    Profesional* prof9 = app.buscarProfesionalPorId(9);
    if (prof9) {
        prof9->agregarDisponibilidad(0, "Lunes", 8, 0, 12, 0);
        prof9->agregarDisponibilidad(0, "Viernes", 14, 0, 18, 0);
        prof9->agregarDisponibilidad(3, "Miércoles", 9, 0, 13, 0);
        prof9->setDuracionTurno(20);
    }

    // Profesional 10 - Gomez (Clínica) - Trabaja en Sanatorio 2
    Profesional* prof10 = app.buscarProfesionalPorId(10);
    if (prof10) {
        prof10->agregarDisponibilidad(2, "Martes", 10, 0, 14, 0);
        prof10->agregarDisponibilidad(2, "Jueves", 15, 0, 19, 0);
        prof10->setDuracionTurno(20);
    }

    // ============================================================================
    // 6️⃣ PACIENTES (10) - Nombres realistas y direcciones de CABA
    // ============================================================================

    // Paciente 1
    app.agregarPaciente(new Paciente(
            1, "Juan", "Perez", "jperez@gmail.com",
            "Av. Corrientes 1500, CABA", 10001, "OSDE",
            -34.6037, -58.3816
    ));

    // Paciente 2
    app.agregarPaciente(new Paciente(
            2, "Maria", "Garcia", "mgarcia@hotmail.com",
            "Av. Santa Fe 2500, CABA", 10002, "Swiss Medical",
            -34.5950, -58.3950
    ));

    // Paciente 3
    app.agregarPaciente(new Paciente(
            3, "Carlos", "Rodriguez", "crodriguez@yahoo.com",
            "Av. Belgrano 900, CABA", 10003, "Galeno",
            -34.6125, -58.3750
    ));

    // Paciente 4
    app.agregarPaciente(new Paciente(
            4, "Ana", "Martinez", "amartinez@outlook.com",
            "Av. Rivadavia 5000, CABA", 10004, "OSDE",
            -34.6200, -58.4350
    ));

    // Paciente 5
    app.agregarPaciente(new Paciente(
            5, "Luis", "Fernandez", "lfernandez@gmail.com",
            "Av. Cabildo 1800, CABA", 10005, "Medicus",
            -34.5650, -58.4450
    ));

    // Paciente 6
    app.agregarPaciente(new Paciente(
            6, "Laura", "Gonzalez", "lgonzalez@hotmail.com",
            "Av. Callao 800, CABA", 10006, "Swiss Medical",
            -34.6050, -58.3920
    ));

    // Paciente 7
    app.agregarPaciente(new Paciente(
            7, "Diego", "Lopez", "dlopez@yahoo.com",
            "Av. Cordoba 2300, CABA", 10007, "OSDE",
            -34.5990, -58.3950
    ));

    // Paciente 8 - TU EMAIL
    app.agregarPaciente(new Paciente(
            8, "Tomas", "Lajnis", "tomaslajnis@gmail.com",
            "Av. de Mayo 800, CABA", 10008, "Galeno",
            -34.6090, -58.3750
    ));

    // Paciente 9
    app.agregarPaciente(new Paciente(
            9, "Valeria", "Sanchez", "vsanchez@outlook.com",
            "Av. Pueyrredon 1200, CABA", 10009, "Medicus",
            -34.5950, -58.4000
    ));

    // Paciente 10
    app.agregarPaciente(new Paciente(
            10, "Roberto", "Torres", "rtorres@gmail.com",
            "Av. Las Heras 2100, CABA", 10010, "Swiss Medical",
            -34.5880, -58.3990
    ));

    // ============================================================================
    // 7️⃣ TURNOS DE PRUEBA (3 turnos para testing)
    // ============================================================================
    auto now = std::chrono::system_clock::now();
    std::string error;

    try
    {
        // TURNO 1: En 28 minutos - Paciente 8 (Tomas) con Dr. Fernandez
        auto tiempo_turno_1 = now + std::chrono::minutes(28);
        std::string str_turno_1 = formatearFechaHora(tiempo_turno_1);
        app.agendarTurno(5001, 8, 1, 1, str_turno_1, 30, error);

        // TURNO 2: En 2 horas - Paciente 1 con Dra. Rodriguez
        auto tiempo_turno_2 = now + std::chrono::hours(2);
        std::string str_turno_2 = formatearFechaHora(tiempo_turno_2);
        app.agendarTurno(5002, 1, 4, 2, str_turno_2, 45, error);

        // TURNO 3: Mañana - Paciente 5 con Dr. Sanchez
        auto tiempo_turno_3 = now + std::chrono::hours(24) + std::chrono::minutes(30);
        std::string str_turno_3 = formatearFechaHora(tiempo_turno_3);
        app.agendarTurno(5003, 5, 7, 4, str_turno_3, 30, error);

        message_center("Mock Data", "3 turnos de prueba creados con exito");
    }
    catch (const std::exception& e)
    {
        message_center("Mock Data", "Error al crear turnos mock");
    }

}