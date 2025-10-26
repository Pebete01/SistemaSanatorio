//
// Created by tomyl on 24/10/2025.
//

#include "DatosMocks.h"

// Ahora este archivo necesita incluir todo lo que usa:
#include "SistemaSanatorio.h"
#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "menu.h" // Para poder usar message_center

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>


static std::string formatearFechaHora(const std::chrono::system_clock::time_point& time_point)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}


/**
 * @brief Implementación de la función para cargar datos mock.
 */
void cargarDatosMock(EmpresaSanatorio &app)
{
    // --- 1. Especialidades ---
    app.agregarEspecialidad(new Especialidad(10, "Cardiologia"));
    app.agregarEspecialidad(new Especialidad(11, "Dermatologia"));

    // --- 2. Profesionales ---
    Especialidad* cardio = app.buscarEspecialidadPorId(10);
    Especialidad* derma = app.buscarEspecialidadPorId(11);

    if (cardio) {
        app.agregarProfesional(new Profesional(101, *cardio, 201, "Carlos", "Demo", "dr.demo@mailfalso.com"));
    }
    if (derma) {
        app.agregarProfesional(new Profesional(102, *derma, 202, "Ana", "Prueba", "dra.prueba@mailfalso.com"));
    }

    // --- 3. Pacientes ---
    app.agregarPaciente(new Paciente(301, "Paciente", "Principal", "tomaslajnis@gmail.com", 12345, "Test OS"));
    app.agregarPaciente(new Paciente(302, "Roberto", "Gomez", "roberto.gomez@mailfalso.com", 67890, "Otra OS"));

    // --- 4. Definición de Tiempos ---
    auto now = std::chrono::system_clock::now();
    std::string error;

    // --- 5. Agendar los 3 Turnos de Prueba ---
    try
    {
        // TURNO 1: (Notificación Inminente) - En 28 minutos
        auto tiempo_turno_1 = now + std::chrono::minutes(28);
        std::string str_turno_1 = formatearFechaHora(tiempo_turno_1);
        app.agendarTurno(1001, 301, 201, 10, str_turno_1, 30, error);

        // TURNO 2: (Futuro Lejano) - En 2 horas
        auto tiempo_turno_2 = now + std::chrono::hours(2);
        std::string str_turno_2 = formatearFechaHora(tiempo_turno_2);
        app.agendarTurno(1002, 302, 202, 11, str_turno_2, 45, error);

        // TURNO 3: (Día Siguiente) - Mañana
        auto tiempo_turno_3 = now + std::chrono::hours(24) + std::chrono::minutes(28);
        std::string str_turno_3 = formatearFechaHora(tiempo_turno_3);
        app.agendarTurno(1003, 301, 201, 10, str_turno_3, 30, error);

        message_center("Datos Mock", "Turnos de prueba creados con exito.");
    }
    catch (const std::exception& e)
    {
        message_center("Mock Data", "Error al crear turnos mock.");
    }
}

