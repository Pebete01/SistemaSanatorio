#include "DatosMock.h"
#include "SistemaSanatorio.h"
#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "Sanatorio.h"
#include "menu.h" // Para message_center

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <ctime> // Para std::strftime y std::localtime
#include <cstdio> // Para sscanf

// FIX: Reemplazamos std::put_time por std::strftime (más robusto para MinGW)
static std::string formatTime(const std::chrono::system_clock::time_point& timePoint)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);
    std::tm tm = *std::localtime(&tt);
    char buffer[20];
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", &tm) == 0) {
        return "";
    }
    return std::string(buffer);
}

static std::string getHoyStr()
{
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm tm = *std::localtime(&tt);
    const char* dias[] = {
            "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"
    };
    if (tm.tm_wday >= 0 && tm.tm_wday <= 6) {
        return dias[tm.tm_wday];
    }
    return "Lunes"; // Fallback
}

void cargarDatosMock(EmpresaSanatorio& app)
{
    std::string error;

    // --- 1. CREAR ESPECIALIDADES ---
    // (IDs se autogeneran desde 100 en adelante)
    Especialidad* esp1 = app.agregarEspecialidad("Cardiologia", error);
    Especialidad* esp2 = app.agregarEspecialidad("Dermatologia", error);
    Especialidad* esp3 = app.agregarEspecialidad("Traumatologia", error);
    Especialidad* esp4 = app.agregarEspecialidad("Pediatria", error);
    Especialidad* esp5 = app.agregarEspecialidad("Clinica Medica", error);
    Especialidad* esp6 = app.agregarEspecialidad("Oftalmologia", error);

    // --- 2. CREAR SANATORIOS ---
    // (IDs se autogeneran desde 100 en adelante, se buscan por ÍNDICE 0, 1, 2)
    Sanatorio* san1 = app.agregarSanatorio("Sanatorio Guemes", "Av. Cordoba 3900", -34.5950, -58.4208, error);
    Sanatorio* san2 = app.agregarSanatorio("Hospital Italiano", "Av. Juan B. Alberdi 450", -34.6190, -58.4431, error);
    Sanatorio* san3 = app.agregarSanatorio("Clinica del Sol", "Av. Coronel Diaz 2079", -34.5885, -58.4103, error);

    // --- 3. CREAR PROFESIONALES ---
    // (IDs se autogeneran desde 100 en adelante)
    Profesional* prof1 = nullptr;
    Profesional* prof2 = nullptr;
    Profesional* prof3 = nullptr;
    Profesional* prof4 = nullptr;
    Profesional* prof5 = nullptr;

    if (esp1 && esp2 && esp3 && esp4 && esp5) {
        prof1 = app.agregarProfesional(4032, *esp1, "Juan", "Perez", "jperez@mail.com", error);
        prof2 = app.agregarProfesional(4089, *esp2, "Ana", "Gomez", "agomez@mail.com", error);
        prof3 = app.agregarProfesional(4076, *esp3, "Carlos", "Martinez", "cmartinez@mail.com", error);
        prof4 = app.agregarProfesional(4036, *esp4, "Maria", "Lopez", "mlopez@mail.com", error);
        prof5 = app.agregarProfesional(4012, *esp5, "Luis", "Gonzalez", "lgonzalez@mail.com", error);
    }

    // --- 4. VINCULAR ESPECIALIDADES Y PROFESIONALES A SANATORIOS ---
    if (san1 && san2 && san3 && esp1 && esp2 && esp3 && esp4 && esp5 && esp6 && prof1 && prof2 && prof3 && prof4 && prof5)
    {
        // Sanatorio 1 (Güemes) - Usamos las funciones del .h
        app.agregarEspecialidadASanatorio(0, esp1); // Índice 0
        app.agregarEspecialidadASanatorio(0, esp3);
        app.agregarEspecialidadASanatorio(0, esp5);
        app.agregarProfesionalASanatorio(0, prof1);
        app.agregarProfesionalASanatorio(0, prof3);
        app.agregarProfesionalASanatorio(0, prof5);

        // Sanatorio 2 (Italiano)
        app.agregarEspecialidadASanatorio(1, esp2); // Índice 1
        app.agregarEspecialidadASanatorio(1, esp4);
        app.agregarProfesionalASanatorio(1, prof2);
        app.agregarProfesionalASanatorio(1, prof4);

        // Sanatorio 3 (Del Sol)
        app.agregarEspecialidadASanatorio(2, esp1); // Índice 2
        app.agregarEspecialidadASanatorio(2, esp6);
        app.agregarEspecialidadASanatorio(2, esp5);
        app.agregarProfesionalASanatorio(2, prof1);
        app.agregarProfesionalASanatorio(2, prof5);
    }

    // --- 5. CONFIGURAR DISPONIBILIDAD DE PROFESIONALES ---
    std::string hoy = getHoyStr();
    if (prof1 && prof2 && prof3 && prof4 && prof5)
    {
        prof1->agregarDisponibilidad(0, hoy, 9, 0, 12, 0); // Índice 0
        prof1->agregarDisponibilidad(0, "Miercoles", 9, 0, 12, 0);
        prof1->agregarDisponibilidad(2, "Viernes", 14, 0, 18, 0); // Índice 2
        prof2->agregarDisponibilidad(1, hoy, 8, 0, 16, 0); // Índice 1
        prof2->agregarDisponibilidad(1, "Jueves", 8, 0, 16, 0);
        prof3->agregarDisponibilidad(0, "Lunes", 10, 0, 17, 0);
        prof3->agregarDisponibilidad(0, "Viernes", 10, 0, 17, 0);
        prof4->agregarDisponibilidad(1, "Martes", 9, 30, 13, 0);
        prof4->agregarDisponibilidad(1, "Jueves", 9, 30, 13, 0);
        prof5->agregarDisponibilidad(0, hoy, 9, 0, 13, 0);
        prof5->agregarDisponibilidad(2, "Miercoles", 15, 0, 19, 0);
    }


    // --- 6. CREAR PACIENTES (5) ---
    // (IDs se autogeneran desde 1100 en adelante)
    Paciente* pac1 = app.agregarPaciente("Tomas", "Lajnis", "tomaslajnis@gmail.com", "Av. Callao 100", 11111, "OSDE", -34.6067, -58.3855, error);
    Paciente* pac2 = app.agregarPaciente("Carolina", "Ladislao", "tomylajnis@gmail.com", "Av. Rivadavia 2000", 22222, "Swiss Medical", -34.6083, -58.4018, error);
    Paciente* pac3 = app.agregarPaciente("Laura", "Garcia", "lgarcia@mail.com", "Florida 500", 33333, "Galeno", -34.6010, -58.3732, error);
    Paciente* pac4 = app.agregarPaciente("Martin", "Fernandez", "mfernandez@mail.com", "Av. de Mayo 800", 44444, "OSDE", -34.6087, -58.3800, error);
    Paciente* pac5 = app.agregarPaciente("Lucia", "Rodriguez", "lrodriguez@mail.com", "Corrientes 1500", 55555, "PAMI", -34.6041, -58.3880, error);

    // --- 7. AGENDAR TURNOS (5) ---
    auto now = std::chrono::system_clock::now();

    // Verificamos que los punteros no sean nulos antes de usarlos
    if (!pac1 || !pac2 || !pac3 || !pac4 || !pac5 || !prof1 || !prof2 || !prof3 || !prof4 || !prof5 || !esp1 || !esp2 || !esp3 || !esp4 || !esp5)
    {
        message_center("Error Mocks", "Fallo al crear entidades basicas. Abortando carga de turnos.");
        return;
    }

    // Turno 1 (Tomas) - 20 minutos desde ahora
    auto timeTurno1 = now + std::chrono::minutes(20);
    std::string timeStr1 = formatTime(timeTurno1);
    // (Pac:1100, Prof:100, SanIdx:0, Esp:100)
    app.agendarTurno(pac1->getId(), prof1->getId(), 0, esp1->getId(), timeStr1, 30, error);
    if (!error.empty()) std::cerr << "Error Mock Turno 1: " << error << std::endl;
    error.clear();

    // Turno 2 (Carolina) - 40 minutos desde ahora
    auto timeTurno2 = now + std::chrono::minutes(40);
    std::string timeStr2 = formatTime(timeTurno2);
    // (Pac:1101, Prof:101, SanIdx:1, Esp:101)
    app.agendarTurno(pac2->getId(), prof2->getId(), 1, esp2->getId(), timeStr2, 30, error);
    if (!error.empty()) std::cerr << "Error Mock Turno 2: " << error << std::endl;
    error.clear();

    // Turno 3 (Laura) - Mañana
    auto maniana = now + std::chrono::hours(24);
    std::string timeStr3 = formatTime(maniana);
    timeStr3.replace(11, 5, "10:00");
    // (Pac:1102, Prof:102, SanIdx:0, Esp:102)
    app.agendarTurno(pac3->getId(), prof3->getId(), 0, esp3->getId(), timeStr3, 30, error);
    if (!error.empty()) std::cerr << "Error Mock Turno 3: " << error << std::endl;
    error.clear();

    // Turno 4 (Martin) - Mañana
    std::string timeStr4 = timeStr3;
    timeStr4.replace(11, 5, "11:00");
    // (Pac:1103, Prof:103, SanIdx:1, Esp:103)
    app.agendarTurno(pac4->getId(), prof4->getId(), 1, esp4->getId(), timeStr4, 30, error);
    if (!error.empty()) std::cerr << "Error Mock Turno 4: " << error << std::endl;
    error.clear();

    // Turno 5 (Lucia) - Pasado mañana
    auto pasadoManiana = now + std::chrono::hours(48);
    std::string timeStr5 = formatTime(pasadoManiana);
    timeStr5.replace(11, 5, "09:30");
    // (Pac:1104, Prof:104, SanIdx:0, Esp:104)
    app.agendarTurno(pac5->getId(), prof5->getId(), 0, esp5->getId(), timeStr5, 30, error);
    if (!error.empty()) std::cerr << "Error Mock Turno 5: " << error << std::endl;
    error.clear();
}