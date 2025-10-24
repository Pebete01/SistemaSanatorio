#pragma once
#include <string>
#include <vector>
#include "thread"
#include "mutex"
#include <atomic>
#include <chrono>
#include "GeocodificadorAPI.h"


// Forward declarations
class Paciente;
class Profesional;
class Turno;
class Sanatorio;
class Especialidad;
class GeocodificadorAPI;

class EmpresaSanatorio
{
private:
    GeocodificadorAPI geocodificadorApi;
    // --- Almacenamiento ---
    Paciente **listaPacientes = nullptr;
    Profesional **profesionales = nullptr;
    Turno **turnos = nullptr;
    Sanatorio **sanatorios = nullptr;
    Especialidad **especialidades = nullptr;

    // --- Capacidades y contadores ---
    int capacidadPacientes = 0, cantidadPacientes = 0;
    int capacidadProfesionales = 0, cantidadProfesionales = 0;
    int capacidadTurno = 0, cantidadTurnos = 0;
    int capacidadSanatorios = 0, cantidadSanatorios = 0;
    int capacidadEspecialidad = 0, cantidadEspecialidades = 0;

    // =================== AGENDA (Turnos) ===================
    struct TurnoRec
    {
        int id;
        int pacienteId;
        int profesionalId;
        int especialidadId;
        std::string fecha; // "YYYY-MM-DD"
        int minOfDay;      // HH*60 + MM
        int durMin;        // duración en minutos
        bool activo{true};
        bool recordatorioEnviado{false}; // evita reenvio
    };
    std::vector<TurnoRec> agenda;

    // ---- NUEVOS MIEMBROS/METODOS PARA EL SERVICIO DE NOTIFICACIONES ----
    std::thread worker_notificaciones;
    std::atomic<bool> running_notificaciones{false};
    mutable std::mutex mtx; // Mutex para proteger el acceso a 'agenda' y 'listaPacientes'
    void revisarTurnosLoop();

public:
    // --- Ciclo de vida / Regla de 5 ---
    EmpresaSanatorio(){geocodificadorApi.configurarRegion("AR");};
    ~EmpresaSanatorio();
    EmpresaSanatorio(const EmpresaSanatorio &) = delete;
    EmpresaSanatorio &operator=(const EmpresaSanatorio &) = delete;
    EmpresaSanatorio(EmpresaSanatorio &&) = delete;
    EmpresaSanatorio &operator=(EmpresaSanatorio &&) = delete;

    // ===================== RECORDATORIO =====================
    void iniciarServicioNotificaciones();
    void detenerServicioNotificaciones();
    // ===================== PACIENTES =====================
    Paciente *buscarPacientePorId(int id);
    const Paciente *buscarPacientePorId(int id) const;
    bool eliminarPacientePorId(int id);
    void actualizarPaciente(int id,
                            const std::string &nombre,
                            const std::string &apellido,
                            int nroAfiliado,
                            const std::string &obraSocial,
                            const std::string &mail,
                            const std::string &direccion,
                            double lat,
                            double lon);
    std::vector<std::string> listarPacientesTexto() const;
    void agregarPaciente(Paciente *p);
    void agrandarListaPaciente();
    void ordenarPacientesPorApellido();

    // =================== PROFESIONALES ===================
    Profesional *buscarProfesionalPorId(int id);
    const Profesional *buscarProfesionalPorId(int id) const;
    bool eliminarProfesionalPorId(int id);
    std::vector<std::string> listarProfesionalesTexto() const;
    void agregarProfesional(Profesional *p);
    void agrandarListaProfesionales();
    void ordenarProfesionalesPorApellido();

    // =================== ESPECIALIDADES ==================
    Especialidad *buscarEspecialidadPorId(int id);
    const Especialidad *buscarEspecialidadPorId(int id) const;
    
    bool eliminarEspecialidadPorId(int id);
    std::vector<std::string> listarEspecialidadesTexto() const;
    void agregarEspecialidad(Especialidad *p);
    void agrandarListaEspecialidad();


    // =================== SANATORIOS ==================
    Sanatorio* buscarSanatorioPorIndice(int idx);
    const Sanatorio* buscarSanatorioPorIndice(int idx) const;
    int getCantidadSanatorios() const { return cantidadSanatorios; }
    Sanatorio** getSanatorios() { return sanatorios; }
    const Sanatorio* const* getSanatorios() const { return sanatorios; }
    void agregarSanatorio(Sanatorio* s);
    void agrandarListaSanatorios();

    // ======================= TURNOS ======================
    // fechaHora: "YYYY-MM-DD HH:MM"
    bool agendarTurno(int idTurno, int idPaciente, int idProfesional, int idEspecialidad,
                      const std::string &fechaHora, int durMin, std::string &error);
    bool cancelarTurnoPorId(int idTurno);
    std::vector<std::string> listarTurnosTexto() const;
    std::vector<std::string> listarTurnosPorProfesionalTexto(int idProfesional) const;

    // ======================= OTROS =======================
    void agrandarListaTurnos();
    void agregarTurnos(Turno *p); // mantenido para compatibilidad
    Sanatorio *nuevoSanatorio();

    // ===================== FACTORÍAS UI ==================
    Paciente *nuevoPaciente();
    Especialidad *nuevaEspecialidad();
    Profesional *nuevoProfesional();

    // ==================== UTILIDADES UI ==================
    int validarEntero(const std::string &mensaje);
    std::string validarTexto(const std::string &mensaje);

    // ======================== MENÚ =======================
    void menu();
    void subMenuMostrar();
    void subMenuAgregar();
    void subMenuEliminar();

    std::pair<double, double> geocodificarDireccion(const std::string& direccion) {
        return geocodificadorApi.obtenerCoordenadas(direccion);
    }
};
