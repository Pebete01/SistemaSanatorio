//
// Created by tomyl on 26/10/2025.
//

#include "DatosMock.h"

// Includes necesarios para que funcione este módulo
#include "SistemaSanatorio.h"
#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "Sanatorio.h"
#include "menu.h" // Para message_center

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>

/*
 * @brief Convierte un time_point de C++ a un string "YYYY-MM-DD HH:MM".
 * Es 'static' para que solo sea visible dentro de este archivo.
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

    // ========== NUEVO: CONFIGURAR DISPONIBILIDAD HORARIA DE PROFESIONALES ==========

    // Profesional 201 (Carlos Demo - Cardiólogo)
    Profesional* profDemo = app.buscarProfesionalPorId(201);
    if (profDemo)
    {
        // Lunes 8:00 a 12:00
        profDemo->agregarDisponibilidad("Lunes", 8, 0, 12, 0);

        // Miércoles 14:00 a 18:30
        profDemo->agregarDisponibilidad("Miércoles", 14, 0, 18, 30);

        // Viernes 9:00 a 13:00
        profDemo->agregarDisponibilidad("Viernes", 9, 0, 13, 0);

        // Turnos de 30 minutos
        profDemo->setDuracionTurno(30);

        message_center("Mock Data", "Disponibilidad configurada para Dr. Demo");
    }

    // Profesional 202 (Ana Prueba - Dermatóloga)
    Profesional* profPrueba = app.buscarProfesionalPorId(202);
    if (profPrueba)
    {
        // Martes 10:00 a 14:00
        profPrueba->agregarDisponibilidad("Martes", 10, 0, 14, 0);

        // Jueves 15:00 a 19:00
        profPrueba->agregarDisponibilidad("Jueves", 15, 0, 19, 0);

        // Sábado 9:00 a 12:30
        profPrueba->agregarDisponibilidad("Sábado", 9, 0, 12, 30);

        // Turnos de 45 minutos (consultas más largas)
        profPrueba->setDuracionTurno(45);

        message_center("Mock Data", "Disponibilidad configurada para Dra. Prueba");
    }

    // ===============================================================================

    // --- 3. Pacientes (con dirección y coordenadas) ---
    // Paciente 301 (con tu email)
    app.agregarPaciente(new Paciente(301, "Paciente", "Principal", "tomaslajnis@gmail.com",
                                     "Av. Espora 550, Adrogue", 12345, "Test OS",
                                     -34.8015, -58.3855)); // Coordenadas Falsas (Adrogué)

    // Paciente 302 (email falso)
    app.agregarPaciente(new Paciente(302, "Roberto", "Gomez", "roberto.gomez@mailfalso.com",
                                     "Calle Falsa 123, CABA", 67890, "Otra OS",
                                     -34.6037, -58.3816)); // Coordenadas Falsas (CABA)

    // --- 4. Sanatorios (con dirección y coordenadas) ---
    // Sanatorio 0 (Índice 0)
    app.agregarSanatorio(new Sanatorio("Sanatorio Central Adrogue", "Bynnon 1700, Adrogue",
                                       -34.7980, -58.3880));
    // Sanatorio 1 (Índice 1)
    app.agregarSanatorio(new Sanatorio("Clinica Norte CABA", "Av. Santa Fe 3000, CABA",
                                       -34.5880, -58.4060));

    // --- 5. Asignar Especialidades y Profesionales a Sanatorios ---

    // Primero, obtenemos los punteros a los Sanatorios
    Sanatorio* sanatorio0 = app.buscarSanatorioPorIndice(0);
    Sanatorio* sanatorio1 = app.buscarSanatorioPorIndice(1);

    // Obtenemos los punteros a las Especialidades
    Especialidad* espCardio = app.buscarEspecialidadPorId(10);
    Especialidad* espDerma = app.buscarEspecialidadPorId(11);

    // Los profesionales ya los obtuvimos arriba (profDemo y profPrueba)

    // Ahora asignamos, verificando que los punteros no sean nulos
    if (sanatorio0)
    {
        if (espCardio)  sanatorio0->agregarEspecialidad(espCardio);
        if (espDerma)   sanatorio0->agregarEspecialidad(espDerma);
        if (profDemo)   sanatorio0->agregarProfesional(profDemo);
        if (profPrueba) sanatorio0->agregarProfesional(profPrueba);
    }

    if (sanatorio1)
    {
        if (espDerma)   sanatorio1->agregarEspecialidad(espDerma);
        if (profPrueba) sanatorio1->agregarProfesional(profPrueba);
    }

    // --- 6. Definición de Tiempos ---
    auto now = std::chrono::system_clock::now();
    std::string error;

    // --- 7. Agendar los 3 Turnos de Prueba ---
    try
    {
        // TURNO 1: (Notificación Inminente) - En 28 minutos
        // Paciente 301 con Profesional 201
        auto tiempo_turno_1 = now + std::chrono::minutes(28);
        std::string str_turno_1 = formatearFechaHora(tiempo_turno_1);

        app.agendarTurno(1001, 301, 201, 10, str_turno_1, 30, error);

        // TURNO 2: (Futuro Lejano) - En 2 horas
        // Paciente 302 con Profesional 202
        auto tiempo_turno_2 = now + std::chrono::hours(2);
        std::string str_turno_2 = formatearFechaHora(tiempo_turno_2);

        app.agendarTurno(1002, 302, 202, 11, str_turno_2, 45, error);

        // TURNO 3: (Día Siguiente) - Mañana
        // Paciente 301 con Profesional 201
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