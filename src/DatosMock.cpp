//
// Created by tomyl on 26/10/2025.
//

#include "DatosMock.h"

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
#include <cmath>

static std::string formatearFechaHora(const std::chrono::system_clock::time_point& time_point)
{
    std::time_t t = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

static std::string encontrarProximoTurnoDisponible(
        Profesional* prof,
        int indiceSanatorio,
        const std::chrono::system_clock::time_point& objetivo,
        int diasBusqueda = 14)
{
    if (!prof) return "";

    auto turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorio, diasBusqueda);

    if (turnosDisponibles.empty()) return "";

    // Convertir el 'objetivo' a std::tm para compararlo
    std::time_t t_objetivo = std::chrono::system_clock::to_time_t(objetivo);
    std::tm tm_objetivo = *std::localtime(&t_objetivo);
    std::string s_fecha_objetivo = formatearFechaHora(objetivo).substr(0, 10);
    std::string s_hora_objetivo = formatearFechaHora(objetivo).substr(11, 5);

    // Buscar el primer turno después del objetivo
    for (const auto& [fecha, hora] : turnosDisponibles)
    {
        if (fecha > s_fecha_objetivo || (fecha == s_fecha_objetivo && hora >= s_hora_objetivo))
        {
            return fecha + " " + hora;
        }
    }

    // Si no se encuentra uno después, devolver el primero disponible
    return turnosDisponibles[0].first + " " + turnosDisponibles[0].second;
}


void cargarDatosMock(EmpresaSanatorio &app) {
    std::string error; // Variable de error para las nuevas funciones

    // --- 1. Carga de Sanatorios (¡AHORA CAPTURAMOS LOS PUNTEROS!) ---
    Sanatorio* s1 = app.agregarSanatorio("Hospital Aleman", "Av. Pueyrredon 1640, CABA", -34.5947, -58.4019, error);
    Sanatorio* s2 = app.agregarSanatorio("Hospital Italiano", "Tte. Gral. Juan Domingo Peron 4190, CABA", -34.6118, -58.4239, error);
    Sanatorio* s3 = app.agregarSanatorio("Sanatorio Finochietto", "Av. Cordoba 2678, CABA", -34.6042, -58.403, error);
    Sanatorio* s4 = app.agregarSanatorio("Sanatorio Guemes", "Francisco Acuna de Figueroa 1240, CABA", -34.6033, -58.4278, error);
    Sanatorio* s5 = app.agregarSanatorio("Clinica y Maternidad Suizo", "Av. Pueyrredon 1461, CABA", -34.5956, -58.4005, error);

    // --- 2. Carga de Especialidades (¡TAMBIÉN CAPTURAMOS LOS PUNTEROS!) ---
    Especialidad *cardio = app.agregarEspecialidad("Cardiologia", error);
    Especialidad *derma = app.agregarEspecialidad("Dermatologia", error);
    Especialidad *trauma = app.agregarEspecialidad("Traumatologia", error);
    Especialidad *pedia = app.agregarEspecialidad("Pediatria", error);
    Especialidad *clinica = app.agregarEspecialidad("Clinica Medica", error);

    // --- 3. Carga de Profesionales (¡CAPTURAMOS PUNTEROS!) ---
    Profesional *prof1 = nullptr, *prof2 = nullptr, *prof3 = nullptr, *prof4 = nullptr, *prof5 = nullptr;
    Profesional *prof6 = nullptr, *prof7 = nullptr, *prof8 = nullptr, *prof9 = nullptr, *prof10 = nullptr;

    if (cardio) {
        prof1 = app.agregarProfesional(101, *cardio, "Roberto", "Fernandez", "rfernandez@hospital.com", error);
        prof2 = app.agregarProfesional(102, *cardio, "Maria", "Lopez", "mlopez@clinica.com", error);
    }
    if (derma) {
        prof3 = app.agregarProfesional(103, *derma, "Carlos", "Martinez", "cmartinez@sanatorio.com", error);
        prof4 = app.agregarProfesional(104, *derma, "Ana", "Rodriguez", "arodriguez@medico.com", error);
    }
    if (trauma) {
        prof5 = app.agregarProfesional(105, *trauma, "Jorge", "Gonzalez", "jgonzalez@trauma.com", error);
        prof6 = app.agregarProfesional(106, *trauma, "Laura", "Perez", "lperez@ortopedia.com", error);
    }
    if (pedia) {
        prof7 = app.agregarProfesional(107, *pedia, "Diego", "Sanchez", "dsanchez@pediatria.com", error);
        prof8 = app.agregarProfesional(108, *pedia, "Lucia", "Ramirez", "lramirez@ninos.com", error);
    }
    if (clinica) {
        prof9 = app.agregarProfesional(109, *clinica, "Martin", "Torres", "mtorres@clinica.com", error);
        prof10 = app.agregarProfesional(110, *clinica, "Sofia", "Gomez", "sgomez@medicina.com", error);
    }

    // ==========================================================
    // --- 4. ¡NUEVO! VINCULACIÓN DE ENTIDADES (La parte que faltaba) ---
    // ==========================================================

    // Asignamos Especialidades a Sanatorios (Ejemplo)
    if (s1) { // Aleman (idx 0)
        if(cardio) s1->agregarEspecialidad(cardio);
        if(derma) s1->agregarEspecialidad(derma);
        if(clinica) s1->agregarEspecialidad(clinica);
    }
    if (s2) { // Italiano (idx 1)
        if(cardio) s2->agregarEspecialidad(cardio);
        if(trauma) s2->agregarEspecialidad(trauma);
        if(pedia) s2->agregarEspecialidad(pedia);
    }
    if (s3) { // Finochietto (idx 2)
        if(derma) s3->agregarEspecialidad(derma);
        if(trauma) s3->agregarEspecialidad(trauma);
    }
    if (s4) { // Guemes (idx 3)
        if(pedia) s4->agregarEspecialidad(pedia);
        if(clinica) s4->agregarEspecialidad(clinica);
    }
    if (s5) { // Suizo (idx 4)
        if(cardio) s5->agregarEspecialidad(cardio);
        if(derma) s5->agregarEspecialidad(derma);
        if(trauma) s5->agregarEspecialidad(trauma);
        if(pedia) s5->agregarEspecialidad(pedia);
        if(clinica) s5->agregarEspecialidad(clinica);
    }

    // Asignamos Profesionales a Sanatorios (Ejemplo)
    // (Un profesional puede trabajar en varios sanatorios)
    if(prof1) { // R. Fernandez (Cardio)
        if(s1) s1->agregarProfesional(prof1); // Aleman (idx 0)
        if(s2) s2->agregarProfesional(prof1); // Italiano (idx 1)
    }
    if(prof2) { // M. Lopez (Cardio)
        if(s5) s5->agregarProfesional(prof2); // Suizo (idx 4)
    }
    if(prof3) { // C. Martinez (Derma)
        if(s1) s1->agregarProfesional(prof3); // Aleman (idx 0)
        if(s3) s3->agregarProfesional(prof3); // Finochietto (idx 2)
    }
    if(prof4) { // A. Rodriguez (Derma)
        if(s5) s5->agregarProfesional(prof4); // Suizo (idx 4)
    }
    if(prof5) { // J. Gonzalez (Trauma)
        if(s2) s2->agregarProfesional(prof5); // Italiano (idx 1)
    }
    if(prof6) { // L. Perez (Trauma)
        if(s3) s3->agregarProfesional(prof6); // Finochietto (idx 2)
        if(s5) s5->agregarProfesional(prof6); // Suizo (idx 4)
    }
    if(prof7) { // D. Sanchez (Pedia)
        if(s2) s2->agregarProfesional(prof7); // Italiano (idx 1)
        if(s4) s4->agregarProfesional(prof7); // Guemes (idx 3)
    }
    if(prof8) { // L. Ramirez (Pedia)
        if(s5) s5->agregarProfesional(prof8); // Suizo (idx 4)
    }
    if(prof9) { // M. Torres (Clinica)
        if(s1) s1->agregarProfesional(prof9); // Aleman (idx 0)
    }
    if(prof10) { // S. Gomez (Clinica)
        if(s4) s4->agregarProfesional(prof10); // Guemes (idx 3)
        if(s5) s5->agregarProfesional(prof10); // Suizo (idx 4)
    }

    // --- 5. Carga de Pacientes ---
    // (Esto estaba bien como estaba)
    app.agregarPaciente("Juan", "Perez", "jperez@gmail.com", "Av. Corrientes 1500, CABA", 10001, "OSDE", -34.6037, -58.3816, error);
    app.agregarPaciente("Maria", "Garcia", "mgarcia@hotmail.com", "Av. Santa Fe 2500, CABA", 10002, "Swiss Medical", -34.5950, -58.3950, error);
    app.agregarPaciente("Carlos", "Rodriguez", "crodriguez@yahoo.com", "Av. Belgrano 900, CABA", 10003, "Galeno", -34.6125, -58.3750, error);
    app.agregarPaciente("Ana", "Martinez", "amartinez@outlook.com", "Av. Rivadavia 5000, CABA", 10004, "OSDE", -34.6200, -58.4350, error);
    app.agregarPaciente("Luis", "Fernandez", "lfernandez@gmail.com", "Av. Cabildo 1800, CABA", 10005, "Medicus", -34.5650, -58.4450, error);
    app.agregarPaciente("Laura", "Gonzalez", "lgonzalez@hotmail.com", "Av. Callao 800, CABA", 10006, "Swiss Medical", -34.6050, -58.3920, error);
    app.agregarPaciente("Diego", "Lopez", "dlopez@yahoo.com", "Av. Cordoba 2300, CABA", 10007, "OSDE", -34.5990, -58.3950, error);
    app.agregarPaciente("Tomas", "Lajnis", "tomaslajnis@gmail.com", "Av. de Mayo 800, CABA", 10008, "Galeno", -34.6090, -58.3750, error);
    app.agregarPaciente("Valeria", "Sanchez", "vsanchez@outlook.com", "Av. Pueyrredon 1200, CABA", 10009, "Medicus", -34.5950, -58.4000, error);
    app.agregarPaciente("Roberto", "Torres", "rtorres@gmail.com", "Av. Las Heras 2100, CABA", 10010, "Swiss Medical", -34.5880, -58.3990, error);

    // --- 6. ¡NUEVO! Configuración de Disponibilidad ---
    // (Esto estaba vacío antes, por eso fallaba)

    // prof1 (R. Fernandez) en Aleman (idx 0) y Italiano (idx 1)
    if (prof1) {
        prof1->agregarDisponibilidad(0, "Lunes", 9, 0, 12, 0); // idx 0 = Aleman
        prof1->agregarDisponibilidad(1, "Martes", 14, 0, 18, 0); // idx 1 = Italiano
    }
    // prof4 (A. Rodriguez) en Suizo (idx 4)
    if (prof4) {
        prof4->agregarDisponibilidad(4, "Miercoles", 8, 30, 12, 0); // idx 4 = Suizo
        prof4->agregarDisponibilidad(4, "Jueves", 8, 30, 12, 0); // idx 4 = Suizo
    }
    // prof7 (D. Sanchez) en Italiano (idx 1) y Guemes (idx 3)
    if (prof7) {
        prof7->agregarDisponibilidad(1, "Viernes", 10, 0, 15, 0); // idx 1 = Italiano
        prof7->agregarDisponibilidad(3, "Viernes", 16, 0, 20, 0); // idx 3 = Guemes
    }
    // (Agrega más disponibilidad para otros si quieres)


    // --- 7. Carga de Turnos de Prueba (Corregido) ---
    auto now = std::chrono::system_clock::now();

    if (prof1 && cardio) { // prof1 es de Cardiologia (ID 1)
        auto objetivo1 = now + std::chrono::hours(1) + std::chrono::minutes(30);
        std::string fechaHora1 = encontrarProximoTurnoDisponible(prof1, 0, objetivo1, 7); // Sanatorio 0 (Aleman)

        if (!fechaHora1.empty()) {
            // (Sin ID de turno, y con ID Especialidad '1')
            app.agendarTurno(2, prof1->getId(), 0, cardio->getId(), fechaHora1, 30, error); // Paciente 2, Prof 1, Sanatorio 0, Esp 1
        }
    }

    if (prof4 && derma) { // prof4 es de Dermatologia (ID 2)
        auto objetivo2 = now + std::chrono::hours(2);
        std::string fechaHora2 = encontrarProximoTurnoDisponible(prof4, 4, objetivo2, 7); // Sanatorio 4 (Suizo)

        if (!fechaHora2.empty()) {
            // (Sin ID de turno, y con ID Especialidad '2')
            app.agendarTurno(1, prof4->getId(), 4, derma->getId(), fechaHora2, 45, error); // Paciente 1, Prof 4, Sanatorio 4, Esp 2
        }
    }

    if (prof7 && pedia) { // prof7 es de Pediatria (ID 4)
        auto objetivo3 = now + std::chrono::hours(24);
        std::string fechaHora3 = encontrarProximoTurnoDisponible(prof7, 1, objetivo3, 7); // Sanatorio 1 (Italiano)

        if (!fechaHora3.empty()) {
            // (Sin ID de turno, y con ID Especialidad '4')
            app.agendarTurno(5, prof7->getId(), 1, pedia->getId(), fechaHora3, 30, error); // Paciente 5, Prof 7, Sanatorio 1, Esp 4
        }
    }
}