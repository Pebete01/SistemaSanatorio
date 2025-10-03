#pragma once
#include <string>
#include <vector>

// Forward declarations
class Paciente;
class Profesional;
class Turno;
class Sanatorio;
class Especialidad;

class EmpresaSanatorio
{
private:
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
    };
    std::vector<TurnoRec> agenda;

public:
    // --- Ciclo de vida / Regla de 5 ---
    EmpresaSanatorio();
    ~EmpresaSanatorio();
    EmpresaSanatorio(const EmpresaSanatorio &) = delete;
    EmpresaSanatorio &operator=(const EmpresaSanatorio &) = delete;
    EmpresaSanatorio(EmpresaSanatorio &&) = delete;
    EmpresaSanatorio &operator=(EmpresaSanatorio &&) = delete;

    // ===================== PACIENTES =====================
    Paciente *buscarPacientePorId(int id);
    const Paciente *buscarPacientePorId(int id) const;
    bool eliminarPacientePorId(int id);
    void actualizarPaciente(int id,
                            const std::string &nombre,
                            const std::string &apellido,
                            int nroAfiliado,
                            const std::string &obraSocial);
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
};
