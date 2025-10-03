#include "SistemaSanatorio.h"
#include "menu.h"

#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"

#include <vector>
#include <stdexcept>
#include <string>

// ---------------- utils ----------------
static int to_int(const std::string &s)
{
    size_t p = 0;
    int v = std::stoi(s, &p);
    if (p != s.size())
        throw std::invalid_argument("nan");
    return v;
}

// ---------------- Pacientes ----------------
static void ui_agregar_paciente(EmpresaSanatorio &app)
{
    std::string sid = input_box("Pacientes - Agregar", "ID:", 10);
    std::string snaf = input_box("Pacientes - Agregar", "Nro Afiliado:", 10);
    std::string nombre = input_box("Pacientes - Agregar", "Nombre:", 40);
    std::string apellido = input_box("Pacientes - Agregar", "Apellido:", 40);
    std::string obra = input_box("Pacientes - Agregar", "Obra social:", 40);
    if (!confirm_box("Confirmar", "Guardar?"))
    {
        message_center("Alta", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid), naf = to_int(snaf);
        app.agregarPaciente(new Paciente(id, nombre, apellido, naf, obra));
        message_center("Alta", "Paciente guardado");
    }
    catch (...)
    {
        message_center("Error", "Campos numericos invalidos");
    }
}

static void ui_eliminar_paciente(EmpresaSanatorio &app)
{
    std::string sid = input_box("Pacientes - Eliminar", "ID:", 10);
    if (!confirm_box("Confirmar", "Eliminar?"))
    {
        message_center("Eliminar", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid);
        message_center("Eliminar", app.eliminarPacientePorId(id) ? "Paciente eliminado" : "ID inexistente");
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}

static void ui_editar_paciente(EmpresaSanatorio &app)
{
    std::string sid = input_box("Pacientes - Editar", "ID:", 10);
    try
    {
        int id = to_int(sid);
        Paciente *p = app.buscarPacientePorId(id);
        if (!p)
        {
            message_center("Editar", "ID inexistente");
            return;
        }

        std::string nombre = input_box("Editar Paciente", "Nombre (" + p->getNombre() + "):", 40);
        std::string apellido = input_box("Editar Paciente", "Apellido (" + p->getApellido() + "):", 40);
        std::string snaf = input_box("Editar Paciente", "Nro Afiliado (" + std::to_string(p->getNumeroDeAfiliado()) + "):", 10);
        std::string obra = input_box("Editar Paciente", "Obra social (" + p->getObraSocial() + "):", 40);

        if (!confirm_box("Confirmar", "Guardar cambios?"))
        {
            message_center("Editar", "Cancelado");
            return;
        }
        int naf = snaf.empty() ? p->getNumeroDeAfiliado() : to_int(snaf);

        app.actualizarPaciente(id,
                               nombre.empty() ? p->getNombre() : nombre,
                               apellido.empty() ? p->getApellido() : apellido,
                               naf,
                               obra.empty() ? p->getObraSocial() : obra);

        message_center("Editar", "Cambios guardados");
    }
    catch (...)
    {
        message_center("Error", "Campo numerico invalido");
    }
}

// ---------------- Especialidades ----------------
static void ui_agregar_especialidad(EmpresaSanatorio &app)
{
    std::string sid = input_box("Especialidades - Agregar", "ID:", 10);
    std::string nom = input_box("Especialidades - Agregar", "Nombre:", 40);
    if (!confirm_box("Confirmar", "Guardar?"))
    {
        message_center("Alta", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid);
        if (app.buscarEspecialidadPorId(id))
        {
            message_center("Error", "ID existente");
            return;
        }
        app.agregarEspecialidad(new Especialidad(id, nom));
        message_center("Alta", "Especialidad guardada");
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}

static void ui_eliminar_especialidad(EmpresaSanatorio &app)
{
    std::string sid = input_box("Especialidades - Eliminar", "ID:", 10);
    if (!confirm_box("Confirmar", "Eliminar?"))
    {
        message_center("Eliminar", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid);
        message_center("Eliminar", app.eliminarEspecialidadPorId(id) ? "Especialidad eliminada" : "ID inexistente");
    }
    catch (...)
    {
        message_center("Error", "ID invqlido");
    }
}

static void ui_listar_especialidades(EmpresaSanatorio &app)
{
    auto v = app.listarEspecialidadesTexto();
    v.empty() ? message_center("Especialidades", "No hay registros") : list_box("Especialidades", v);
}

// ---------------- Profesionales ----------------
static void ui_agregar_profesional(EmpresaSanatorio &app)
{
    std::string sid = input_box("Profesionales - Agregar", "ID:", 10);
    std::string snum = input_box("Profesionales - Agregar", "Nro Profesional:", 10);
    std::string sidE = input_box("Profesionales - Agregar", "ID Especialidad:", 10);
    std::string nom = input_box("Profesionales - Agregar", "Nombre:", 40);
    std::string ape = input_box("Profesionales - Agregar", "Apellido:", 40);
    if (!confirm_box("Confirmar", "Guardar?"))
    {
        message_center("Alta", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid), num = to_int(snum), idE = to_int(sidE);
        auto *esp = app.buscarEspecialidadPorId(idE);
        if (!esp)
        {
            message_center("Error", "Especialidad inexistente");
            return;
        }
        app.agregarProfesional(new Profesional(num, *esp, id, nom, ape));
        message_center("Alta", "Profesional guardado");
    }
    catch (...)
    {
        message_center("Error", "Campos numericos invalidos");
    }
}

static void ui_eliminar_profesional(EmpresaSanatorio &app)
{
    std::string sid = input_box("Profesionales - Eliminar", "ID:", 10);
    if (!confirm_box("Confirmar", "Eliminar?"))
    {
        message_center("Eliminar", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid);
        message_center("Eliminar", app.eliminarProfesionalPorId(id) ? "Profesional eliminado" : "ID inexistente");
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}

static void ui_listar_profesionales(EmpresaSanatorio &app)
{
    auto v = app.listarProfesionalesTexto();
    v.empty() ? message_center("Profesionales", "No hay registros") : list_box("Profesionales", v);
}

// ===== Turnos =====
static int to_int(const std::string &s); // ya definido arriba

static void ui_agendar_turno(EmpresaSanatorio &app)
{
    std::string sid = input_box("Turnos - Agendar", "ID Turno:", 10);
    std::string sidP = input_box("Turnos - Agendar", "ID Paciente:", 10);
    std::string sidR = input_box("Turnos - Agendar", "ID Profesional:", 10);
    std::string sidE = input_box("Turnos - Agendar", "ID Especialidad:", 10);
    std::string f = input_box("Turnos - Agendar", "Fecha (YYYY-MM-DD):", 12);
    std::string h = input_box("Turnos - Agendar", "Hora (HH:MM):", 5);
    std::string sdur = input_box("Turnos - Agendar", "Duracion (min):", 5);
    if (!confirm_box("Confirmar", "Agendar?"))
    {
        message_center("Turnos", "Cancelado");
        return;
    }
    try
    {
        std::string fh = f + " " + h;
        std::string err;
        bool ok = app.agendarTurno(
            to_int(sid), to_int(sidP), to_int(sidR), to_int(sidE),
            fh, to_int(sdur), err);
        message_center("Turnos", ok ? "Turno agendado" : err);
    }
    catch (...)
    {
        message_center("Error", "Campos numericos invalidos");
    }
}

static void ui_cancelar_turno(EmpresaSanatorio &app)
{
    std::string sid = input_box("Turnos - Cancelar", "ID Turno:", 10);
    if (!confirm_box("Confirmar", "Cancelar?"))
    {
        message_center("Turnos", "Cancelado");
        return;
    }
    try
    {
        bool ok = app.cancelarTurnoPorId(to_int(sid));
        message_center("Turnos", ok ? "Turno cancelado" : "No existe/ya cancelado");
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}

static void ui_listar_turnos(EmpresaSanatorio &app)
{
    auto v = app.listarTurnosTexto();
    v.empty() ? message_center("Turnos", "No hay registros") : list_box("Turnos", v);
}

// ---------------- main ----------------
int main()
{
    EmpresaSanatorio app;

    init_ui();
    std::vector<std::string> principal = {"Pacientes", "Profesionales", "Especialidades", "Turnos", "Salir"};

    for (;;)
    {
        int i = run_menu_titled("Sanatorio", principal);
        if (i < 0 || principal[i] == "Salir")
        {
            shutdown_ui();
            break;
        }

        if (principal[i] == "Pacientes")
        {
            for (;;)
            {
                int s = run_submenu("Pacientes", {"Agregar", "Eliminar", "Listar", "Ordenar A-Z", "Editar"});
                if (s < 0)
                    break;
                if (s == 0)
                    ui_agregar_paciente(app);
                else if (s == 1)
                    ui_eliminar_paciente(app);
                else if (s == 2)
                {
                    auto v = app.listarPacientesTexto();
                    v.empty() ? message_center("Pacientes", "No hay registros")
                              : list_box("Pacientes", v);
                }
                else if (s == 3)
                {
                    app.ordenarPacientesPorApellido();
                    message_center("Pacientes", "Ordenado por Apellido");
                }
                else if (s == 4)
                    ui_editar_paciente(app);
            }
        }
        else if (principal[i] == "Profesionales")
        {
            int s = run_submenu("Profesionales", {"Agregar", "Eliminar", "Listar", "Ordenar A-Z"});
            if (s == 0)
                ui_agregar_profesional(app);
            else if (s == 1)
                ui_eliminar_profesional(app);
            else if (s == 2)
                ui_listar_profesionales(app);
            else if (s == 3)
            {
                app.ordenarProfesionalesPorApellido();
                message_center("Profesionales", "Ordenado por Apellido");
            }
        }
        else if (principal[i] == "Especialidades")
        {
            int s = run_submenu("Especialidades", {"Agregar", "Eliminar", "Listar"});
            if (s == 0)
                ui_agregar_especialidad(app);
            else if (s == 1)
                ui_eliminar_especialidad(app);
            else if (s == 2)
                ui_listar_especialidades(app);
        }
        else if (principal[i] == "Turnos")
        {
            int s = run_submenu("Turnos", {"Agendar", "Cancelar", "Listar"});
            if (s == 0)
                ui_agendar_turno(app);
            else if (s == 1)
                ui_cancelar_turno(app);
            else if (s == 2)
                ui_listar_turnos(app);
        }
    }
    return 0;
}
