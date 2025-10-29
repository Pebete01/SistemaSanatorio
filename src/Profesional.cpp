#include "Profesional.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>

int Profesional::numProfesionales = 0;

Profesional::Profesional(int numero,
                         const Especialidad &esp,
                         int id,
                         const std::string &nombre,
                         const std::string &apellido,
                         const std::string &mail): Persona(id, nombre, apellido, mail),
          estado(true),
          numeroDeProfesional(numero),
          especialidad(esp)
{
    ++numProfesionales;
}

Profesional::~Profesional()
{
    --numProfesionales;
}

void Profesional::modificarEstado()
{
    estado = !estado;
}

void Profesional::mostrar()
{
    std::cout << "ID: " << getId()
              << " Dr/Dra: " << getNombre() << ' ' << getApellido()
              << " Numero de profesional: " << numeroDeProfesional
              << " Estado: " << (estado ? "Activo" : "Inactivo")
              << '\n';
}

int Profesional::getNumeroProfesional() const
{
    return numeroDeProfesional;
}

const Especialidad &Profesional::getEspecialidad() const
{
    return especialidad;
}

void Profesional::instanciasVivas()
{
    std::cout << "Profesionales vivos: " << numProfesionales << '\n';
}

// ✅ MODIFICADO: Ahora agregar disponibilidad incluye el sanatorio
void Profesional::agregarDisponibilidad(int indiceSanatorio, const std::string &dia,
                                        int horaInicio, int minutoInicio,
                                        int horaFin, int minutoFin)
{
    // Buscar si ya existe disponibilidad para este sanatorio
    for (auto &dispSan : disponibilidadPorSanatorio)
    {
        if (dispSan.indiceSanatorio == indiceSanatorio)
        {
            // Ya existe, agregar la franja horaria
            dispSan.franjas.push_back({dia, horaInicio, minutoInicio, horaFin, minutoFin});
            return;
        }
    }

    // No existe, crear nueva disponibilidad para este sanatorio
    DisponibilidadSanatorio nuevaDisp;
    nuevaDisp.indiceSanatorio = indiceSanatorio;
    nuevaDisp.franjas.push_back({dia, horaInicio, minutoInicio, horaFin, minutoFin});
    disponibilidadPorSanatorio.push_back(nuevaDisp);
}

// ✅ NUEVO: Verificar si trabaja en un sanatorio
bool Profesional::trabajaEnSanatorio(int indiceSanatorio) const
{
    for (const auto &dispSan : disponibilidadPorSanatorio)
    {
        if (dispSan.indiceSanatorio == indiceSanatorio)
            return true;
    }
    return false;
}

// ✅ NUEVO: Obtener horarios en un sanatorio específico
std::vector<FranjaHoraria> Profesional::obtenerHorariosSanatorio(int indiceSanatorio) const
{
    for (const auto &dispSan : disponibilidadPorSanatorio)
    {
        if (dispSan.indiceSanatorio == indiceSanatorio)
            return dispSan.franjas;
    }
    return std::vector<FranjaHoraria>(); // Vacío si no trabaja ahí
}

void Profesional::mostrarDisponibilidad() const
{
    std::cout << "\nDisponibilidad de " << getNombre() << " " << getApellido() << ":\n";

    for (const auto &dispSan : disponibilidadPorSanatorio)
    {
        std::cout << "  Sanatorio (índice " << dispSan.indiceSanatorio << "):\n";
        for (const auto &f : dispSan.franjas)
        {
            std::cout << "   - " << f.dia << ": "
                      << std::setw(2) << std::setfill('0') << f.horaInicio << ":"
                      << std::setw(2) << f.minutoInicio << " a "
                      << std::setw(2) << f.horaFin << ":"
                      << std::setw(2) << f.minutoFin << '\n';
        }
    }
}

// Utilidad: convertir time_point a string YYYY-MM-DD
static std::string fechaToStr(std::chrono::system_clock::time_point tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm *tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d");
    return oss.str();
}

// Utilidad: obtener día de semana (0=domingo, 1=lunes, ..., 6=sábado)
static int diaSemana(std::chrono::system_clock::time_point tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm *tm = std::localtime(&t);
    return tm->tm_wday;
}

// Días mapeados a texto
static std::string nombreDia(int wday)
{
    static const char *dias[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
    return dias[wday];
}

// ✅ MODIFICADO: Ahora obtener turnos disponibles es por sanatorio
std::vector<std::pair<std::string, std::string>> Profesional::obtenerTurnosDisponibles(
        int indiceSanatorio, int diasDesdeHoy)
{
    std::vector<std::pair<std::string, std::string>> disponibles;

    // Buscar la disponibilidad para este sanatorio
    std::vector<FranjaHoraria> horariosSanatorio;
    for (const auto &dispSan : disponibilidadPorSanatorio)
    {
        if (dispSan.indiceSanatorio == indiceSanatorio)
        {
            horariosSanatorio = dispSan.franjas;
            break;
        }
    }

    if (horariosSanatorio.empty())
        return disponibles; // No trabaja en este sanatorio

    auto hoy = std::chrono::system_clock::now();

    for (int d = 0; d < diasDesdeHoy; ++d)
    {
        auto fecha = hoy + std::chrono::hours(24 * d);
        int wday = diaSemana(fecha);
        std::string diaTexto = nombreDia(wday);

        // Buscar si el profesional atiende este día en este sanatorio
        for (const auto &f : horariosSanatorio)
        {
            if (f.dia == diaTexto)
            {
                std::string fechaStr = fechaToStr(fecha);

                // Generar todos los slots de tiempo
                int minutosInicio = f.horaInicio * 60 + f.minutoInicio;
                int minutosFin = f.horaFin * 60 + f.minutoFin;

                for (int m = minutosInicio; m < minutosFin; m += duracionTurnoMin)
                {
                    int h = m / 60;
                    int min = m % 60;

                    std::ostringstream horaStr;
                    horaStr << std::setw(2) << std::setfill('0') << h << ":"
                            << std::setw(2) << std::setfill('0') << min;
                    std::string hora = horaStr.str();

                    // Verificar si no está ocupado
                    auto &ocupados = turnosOcupados[fechaStr];
                    if (std::find(ocupados.begin(), ocupados.end(), hora) == ocupados.end())
                    {
                        disponibles.emplace_back(fechaStr, hora);
                    }
                }
            }
        }
    }

    return disponibles;
}

bool Profesional::reservarTurno(const std::string &fecha, const std::string &hora)
{
    // Nota: Esta función ahora reserva el turno globalmente (en todos los sanatorios)
    // porque un profesional no puede estar en dos lugares al mismo tiempo
    turnosOcupados[fecha].push_back(hora);
    return true;
}