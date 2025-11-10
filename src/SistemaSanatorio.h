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

class EmpresaSanatorio {
private:
    GeocodificadorAPI geocodificadorApi;
    Paciente **listaPacientes = nullptr;
    Profesional **profesionales = nullptr;
    Turno **turnos = nullptr;
    Sanatorio **sanatorios = nullptr;
    Especialidad **especialidades = nullptr;

    int capacidadPacientes = 0, cantidadPacientes = 0;
    int capacidadProfesionales = 0, cantidadProfesionales = 0;
    int capacidadTurno = 0, cantidadTurnos = 0;
    int capacidadSanatorios = 0, cantidadSanatorios = 0;
    int capacidadEspecialidad = 0, cantidadEspecialidades = 0;

    // Contadores para el próximo ID a asignar
    // (Iniciados en números altos para no chocar con los DatosMock)
    int proximoIdPaciente = 1100;
    int proximoIdProfesional = 100;
    int proximoIdEspecialidad = 100;
    int proximoIdSanatorio = 100;
    int proximoIdTurno = 5100;

    struct TurnoRec {
        int id;
        int pacienteId;
        int profesionalId;
        int especialidadId;
        int sanatorioIdx;
        std::string fecha; // "YYYY-MM-DD"
        int minOfDay;      // HH*60 + MM
        int durMin;        // duración en minutos
        bool activo{true};
        bool recordatorioEnviado{false}; // evita reenvio
    };
    std::vector <TurnoRec> agenda;

    std::thread worker_notificaciones;
    std::atomic<bool> running_notificaciones{false};
    mutable std::mutex mtx; // Mutex para proteger el acceso a 'agenda' y 'listaPacientes'
    void revisarTurnosLoop();

public:

    EmpresaSanatorio() { geocodificadorApi.configurarRegion("AR"); };

    ~EmpresaSanatorio();

    EmpresaSanatorio(const EmpresaSanatorio &) = delete;

    EmpresaSanatorio &operator=(const EmpresaSanatorio &) = delete;

    EmpresaSanatorio(EmpresaSanatorio &&) = delete;

    EmpresaSanatorio &operator=(EmpresaSanatorio &&) = delete;


    void iniciarServicioNotificaciones();

    void detenerServicioNotificaciones();

    Paciente *buscarPacientePorId(int id);

    const Paciente *buscarPacientePorId(int id) const;

    Paciente *buscarPacientePorNombre(const std::string &nombre, const std::string &apellido);
    const Paciente *buscarPacientePorNombre(const std::string &nombre, const std::string &apellido) const;

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

    std::vector <std::string> listarPacientesTexto() const;

    // ================== CAMBIO 1 ==================
    // La firma de agregarPaciente ahora acepta las coordenadas
    // (ya no tiene que geocodificar, solo recibe los datos).
    Paciente* agregarPaciente(const std::string& nombre, const std::string& apellido,
                              const std::string& mail, const std::string& direccion,
                              int nroAfiliado, const std::string& obraSocial,
                              double lat, double lon, // <-- Recibe las coordenadas
                              std::string& error);


    void agrandarListaPaciente();

    void ordenarPacientesPorApellido();

    Profesional *buscarProfesionalPorId(int id);

    const Profesional *buscarProfesionalPorId(int id) const;

    Profesional *buscarProfesionalPorNombre(const std::string &nombre, const std::string &apellido);
    const Profesional *buscarProfesionalPorNombre(const std::string &nombre, const std::string &apellido) const;

    bool eliminarProfesionalPorId(int id);

    std::vector <std::string> listarProfesionalesTexto() const;

    Profesional* agregarProfesional(int nroMatricula, const Especialidad& esp,
                                    const std::string& nombre, const std::string& apellido,
                                    const std::string& mail, std::string& error);

    void agrandarListaProfesionales();

    void ordenarProfesionalesPorApellido();

    void agregarProfesionalASanatorio(int indiceSanatorio, Profesional* prof);
    void eliminarProfesionalDeSanatorio(int indiceSanatorio, int idProfesional);

    Especialidad *buscarEspecialidadPorId(int id);

    const Especialidad *buscarEspecialidadPorId(int id) const;

    Especialidad *buscarEspecialidadPorNombre(const std::string &nombre);
    const Especialidad *buscarEspecialidadPorNombre(const std::string &nombre) const;

    bool eliminarEspecialidadPorId(int id);

    std::vector <std::string> listarEspecialidadesTexto() const;

    Especialidad* agregarEspecialidad(const std::string& nombre, std::string& error);

    void agrandarListaEspecialidad();

    void agregarEspecialidadASanatorio(int indiceSanatorio, Especialidad *esp);
    void eliminarEspecialidadDeSanatorio(int indiceSanatorio, int idEspecialidad);

    Sanatorio *buscarSanatorioPorIndice(int idx);

    const Sanatorio *buscarSanatorioPorIndice(int idx) const;

    int getCantidadSanatorios() const { return cantidadSanatorios; }

    Sanatorio **getSanatorios() { return sanatorios; }

    const Sanatorio *const *getSanatorios() const { return sanatorios; }

    Sanatorio* agregarSanatorio(const std::string& nombre, const std::string& direccion,
                                double lat, double lon, // <-- Recibe las coordenadas
                                std::string& error);

    void agrandarListaSanatorios();
    Sanatorio* buscarSanatorioPorId(int id);
    const Sanatorio* buscarSanatorioPorId(int id) const;
    bool eliminarSanatorioPorId(int id, std::string& error);
    std::vector<std::string> listarEspecialidadesDeSanatorio(int idSanatorio) const;
    std::vector<std::string> listarProfesionalesDeSanatorio(int idSanatorio) const;
    std::vector<std::string> listarPacientesDeSanatorio(int idSanatorio) const;

    // fechaHora: "YYYY-MM-DD HH:MM"
    int agendarTurno(int idPaciente, int idProfesional, int sanatorioIdx,
                     int idEspecialidad, const std::string &fechaHora, int durMin, std::string &error);

    bool cancelarTurnoPorId(int idTurno);

    std::vector <std::string> listarTurnosTexto() const;

    std::vector <std::string> listarTurnosPorProfesionalTexto(int idProfesional) const;

    std::vector<std::pair<int, double>> buscarSanatoriosPorEspecialidad(
            int idEspecialidad,
            double latPaciente,
            double lonPaciente
    ) const;

    std::vector<std::pair<int, double>> buscarSanatoriosPorProfesional(
            int idProfesional,
            double latPaciente,
            double lonPaciente
    ) const;

    std::vector<int> obtenerProfesionalesPorEspecialidad(int idEspecialidad) const;
    std::vector<int> obtenerProfesionalesPorSanatorio(int indiceSanatorio) const;

    std::vector<int> obtenerProfesionalesPorEspecialidadEnSanatorio(int idEspecialidad, int indiceSanatorio) const;


    std::string validarTexto(const std::string &mensaje);

    std::pair<double, double> geocodificarDireccion(const std::string &direccion) {
        return geocodificadorApi.obtenerCoordenadas(direccion);
    }

};