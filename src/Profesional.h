#pragma once
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "Persona.h"
#include "Especialidad.h"

struct FranjaHoraria {
    std::string dia;   // "Lunes", "Martes", etc.
    int horaInicio;    // 8
    int minutoInicio;  // 30
    int horaFin;       // 12
    int minutoFin;     // 30
};

// ✅ NUEVA ESTRUCTURA: Para gestionar disponibilidad por sanatorio
struct DisponibilidadSanatorio {
    int indiceSanatorio;  // Índice del sanatorio en el array de EmpresaSanatorio
    std::vector<FranjaHoraria> franjas;  // Horarios en ese sanatorio
};

class Profesional : public Persona
{
private:
    bool estado;
    int numeroDeProfesional;
    Especialidad especialidad;
    static int numProfesionales;

    // ✅ MODIFICADO: Ahora la disponibilidad es por sanatorio
    std::vector<DisponibilidadSanatorio> disponibilidadPorSanatorio;

    std::map<std::string, std::vector<std::string>> turnosOcupados; // fecha (YYYY-MM-DD) → horas ocupadas (HH:MM)
    int duracionTurnoMin = 30; // duración de cada turno

public:
    Profesional(int numero,
                const Especialidad &esp,
                int id,
                const std::string &nombre,
                const std::string &apellido,
                const std::string &mail);

    ~Profesional() override;

    void modificarEstado();
    void mostrar() override;

    int getNumeroProfesional() const;
    const Especialidad &getEspecialidad() const;
    bool getEstado() const { return estado; }

    static void instanciasVivas();

    // ✅ MODIFICADO: Ahora agregar disponibilidad incluye el sanatorio
    void agregarDisponibilidad(int indiceSanatorio, const std::string &dia,
                               int horaInicio, int minutoInicio, int horaFin, int minutoFin);

    // ✅ NUEVO: Obtener turnos disponibles en un sanatorio específico
    std::vector<std::pair<std::string, std::string>> obtenerTurnosDisponibles(
            int indiceSanatorio, int diasDesdeHoy = 14);

    // ✅ NUEVO: Verificar si trabaja en un sanatorio
    bool trabajaEnSanatorio(int indiceSanatorio) const;

    // ✅ NUEVO: Obtener horarios en un sanatorio específico
    std::vector<FranjaHoraria> obtenerHorariosSanatorio(int indiceSanatorio) const;

    bool reservarTurno(const std::string &fecha, const std::string &hora);
    void mostrarDisponibilidad() const;
    void setDuracionTurno(int minutos) { duracionTurnoMin = minutos; }
    int getDuracionTurno() const { return duracionTurnoMin; }
};