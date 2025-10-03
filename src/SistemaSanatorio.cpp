#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <cstdio>

#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"
#include "Turno.h"
#include "Sanatorio.h"
#include "SistemaSanatorio.h"

using namespace std;

// ================== PACIENTES ==================

std::vector<std::string> EmpresaSanatorio::listarPacientesTexto() const
{
    std::vector<std::string> out;
    out.reserve(cantidadPacientes);
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (!listaPacientes[i])
            continue;
        std::ostringstream os;
        os << *listaPacientes[i];
        out.push_back(os.str());
    }
    return out;
}

bool EmpresaSanatorio::eliminarPacientePorId(int id)
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] && listaPacientes[i]->getId() == id)
        {
            delete listaPacientes[i];
            for (int j = i + 1; j < cantidadPacientes; ++j)
                listaPacientes[j - 1] = listaPacientes[j];
            --cantidadPacientes;
            return true;
        }
    }
    return false;
}

Paciente *EmpresaSanatorio::buscarPacientePorId(int id)
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] && listaPacientes[i]->getId() == id)
            return listaPacientes[i];
    }
    return nullptr;
}

void EmpresaSanatorio::actualizarPaciente(int id,
                                          const std::string &nombre,
                                          const std::string &apellido,
                                          int nroAfiliado,
                                          const std::string &obraSocial)
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] && listaPacientes[i]->getId() == id)
        {
            Paciente *viejo = listaPacientes[i];
            listaPacientes[i] = new Paciente(id, nombre, apellido, nroAfiliado, obraSocial);
            delete viejo;
            return;
        }
    }
}

// ================== CONSTRUCCIÓN/DESTRUCCIÓN ==================

EmpresaSanatorio::EmpresaSanatorio() = default;

EmpresaSanatorio::~EmpresaSanatorio()
{
    for (int i = 0; i < cantidadPacientes; ++i)
        delete listaPacientes[i];
    delete[] listaPacientes;

    for (int i = 0; i < cantidadProfesionales; ++i)
        delete profesionales[i];
    delete[] profesionales;

    for (int i = 0; i < cantidadTurnos; ++i)
        delete turnos[i];
    delete[] turnos;

    for (int i = 0; i < cantidadSanatorios; ++i)
        delete sanatorios[i];
    delete[] sanatorios;

    for (int i = 0; i < cantidadEspecialidades; ++i)
        delete especialidades[i];
    delete[] especialidades;
}

// ================== CRECIMIENTO DE LISTAS ==================

void EmpresaSanatorio::agrandarListaPaciente()
{
    int capacidadNueva = (capacidadPacientes == 0) ? 4 : capacidadPacientes * 2;
    auto **listaNueva = new Paciente *[capacidadNueva];
    for (int i = 0; i < cantidadPacientes; ++i)
        listaNueva[i] = listaPacientes[i];
    delete[] listaPacientes;
    listaPacientes = listaNueva;
    capacidadPacientes = capacidadNueva;
}

void EmpresaSanatorio::agregarPaciente(Paciente *p)
{
    if (!p)
        return;
    if (capacidadPacientes == cantidadPacientes)
        agrandarListaPaciente();
    listaPacientes[cantidadPacientes++] = p;
}

void EmpresaSanatorio::agrandarListaEspecialidad()
{
    int capacidadNueva = (capacidadEspecialidad == 0) ? 4 : capacidadEspecialidad * 2;
    auto **listaNueva = new Especialidad *[capacidadNueva];
    // copiar SOLO los existentes
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        listaNueva[i] = especialidades[i];
    }
    delete[] especialidades;
    especialidades = listaNueva;
    capacidadEspecialidad = capacidadNueva;
}

void EmpresaSanatorio::agregarEspecialidad(Especialidad *p)
{
    if (!p)
        return;
    if (capacidadEspecialidad == cantidadEspecialidades)
        agrandarListaEspecialidad();
    especialidades[cantidadEspecialidades++] = p;
}

void EmpresaSanatorio::agrandarListaProfesionales()
{
    int capacidadNueva = (capacidadProfesionales == 0) ? 4 : capacidadProfesionales * 2;
    auto **listaNueva = new Profesional *[capacidadNueva];
    for (int i = 0; i < cantidadProfesionales; ++i)
        listaNueva[i] = profesionales[i];
    delete[] profesionales;
    profesionales = listaNueva;
    capacidadProfesionales = capacidadNueva;
}

void EmpresaSanatorio::agregarProfesional(Profesional *p)
{
    if (!p)
        return;
    if (cantidadProfesionales == capacidadProfesionales)
        agrandarListaProfesionales();
    profesionales[cantidadProfesionales++] = p;
}

void EmpresaSanatorio::agrandarListaTurnos()
{
    int capacidadNueva = (capacidadTurno == 0) ? 4 : capacidadTurno * 2;
    auto **listaNueva = new Turno *[capacidadNueva];
    for (int i = 0; i < cantidadTurnos; ++i)
        listaNueva[i] = turnos[i];
    delete[] turnos;
    turnos = listaNueva;
    capacidadTurno = capacidadNueva;
}

void EmpresaSanatorio::agregarTurnos(Turno *p)
{
    if (!p)
        return;
    if (capacidadTurno == cantidadTurnos)
        agrandarListaTurnos();
    turnos[cantidadTurnos++] = p;
}

// ================== ORDENAMIENTOS ==================

void EmpresaSanatorio::ordenarPacientesPorApellido()
{
    for (int i = 1; i < cantidadPacientes; ++i)
    {
        Paciente *key = listaPacientes[i];
        int j = i - 1;
        while (j >= 0 && (*key) < (*listaPacientes[j]))
        {
            listaPacientes[j + 1] = listaPacientes[j];
            --j;
        }
        listaPacientes[j + 1] = key;
    }
}

void EmpresaSanatorio::ordenarProfesionalesPorApellido()
{
    std::sort(profesionales, profesionales + cantidadProfesionales,
              [](const Profesional *a, const Profesional *b)
              {
                  if (!a || !b)
                      return b != nullptr; // nulos al final
                  if (a->getApellido() != b->getApellido())
                      return a->getApellido() < b->getApellido();
                  return a->getNombre() < b->getNombre();
              });
}

// ================== VALIDACIONES IO ==================

int EmpresaSanatorio::validarEntero(const string &mensaje)
{
    int valor;
    while (true)
    {
        cout << mensaje;
        if (cin >> valor)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }
        else
        {
            cout << "Entrada inválida. Ingrese un número válido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string EmpresaSanatorio::validarTexto(const string &mensaje)
{
    string valor;
    while (true)
    {
        cout << mensaje;
        getline(cin, valor);
        if (!valor.empty())
            return valor;
        cout << "El texto no puede estar vacío." << endl;
    }
}

// ================== FACTORÍAS ==================

Especialidad *EmpresaSanatorio::nuevaEspecialidad()
{
    int id = validarEntero("Ingrese ID especialidad: ");
    string nombre = validarTexto("Ingrese nombre de Especialidad: ");
    return new Especialidad(id, nombre);
}

Paciente *EmpresaSanatorio::nuevoPaciente()
{
    int id = validarEntero("Ingrese ID del paciente: ");
    int numAfiliado = validarEntero("Ingrese numero afiliado: ");
    string nombre = validarTexto("Ingrese nombre: ");
    string apellido = validarTexto("Ingrese apellido: ");
    string obraSocial = validarTexto("Ingrese obra Social: ");
    return new Paciente(id, nombre, apellido, numAfiliado, obraSocial);
}

Profesional *EmpresaSanatorio::nuevoProfesional()
{
    int id = validarEntero("Ingrese ID del profesional: ");
    int numProfesional = validarEntero("Ingrese numero de profesional: ");
    int idEsp = validarEntero("Ingrese ID de especialidad: ");
    string nombre = validarTexto("Ingrese nombre: ");
    string apellido = validarTexto("Ingrese apellido: ");

    Especialidad *esp = buscarEspecialidadPorId(idEsp);
    if (!esp)
    {
        cout << "Especialidad inexistente. Cancele o cargue la especialidad antes." << endl;
        return nullptr;
    }
    // Constructor: (numero, Especialidad, id, nombre, apellido)
    return new Profesional(numProfesional, *esp, id, nombre, apellido);
}

Sanatorio *EmpresaSanatorio::nuevoSanatorio()
{
    string nombre = validarTexto("Ingrese nombre del sanatorio: ");
    string ubicacion = validarTexto("Ingrese ubicacion del sanatorio: ");
    return new Sanatorio(nombre, ubicacion);
}

// ================== PROFESIONALES ==================

Profesional *EmpresaSanatorio::buscarProfesionalPorId(int id)
{
    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] && profesionales[i]->getId() == id)
            return profesionales[i];
    }
    return nullptr;
}

bool EmpresaSanatorio::eliminarProfesionalPorId(int id)
{
    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] && profesionales[i]->getId() == id)
        {
            delete profesionales[i];
            for (int j = i + 1; j < cantidadProfesionales; ++j)
                profesionales[j - 1] = profesionales[j];
            --cantidadProfesionales;
            return true;
        }
    }
    return false;
}

std::vector<std::string> EmpresaSanatorio::listarProfesionalesTexto() const
{
    std::vector<std::string> out;
    out.reserve(cantidadProfesionales);
    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (!profesionales[i])
            continue;
        const auto *pr = profesionales[i];
        out.push_back(
            "ID " + std::to_string(pr->getId()) +
            " | " + pr->getApellido() + ", " + pr->getNombre() +
            " | Nº Prof: " + std::to_string(pr->getNumeroProfesional()));
    }
    return out;
}

// ================== ESPECIALIDADES ==================

Especialidad *EmpresaSanatorio::buscarEspecialidadPorId(int id)
{
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (especialidades[i] && especialidades[i]->getId() == id)
            return especialidades[i];
    }
    return nullptr;
}

bool EmpresaSanatorio::eliminarEspecialidadPorId(int id)
{
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (especialidades[i] && especialidades[i]->getId() == id)
        {
            delete especialidades[i];
            for (int j = i + 1; j < cantidadEspecialidades; ++j)
                especialidades[j - 1] = especialidades[j];
            --cantidadEspecialidades;
            return true;
        }
    }
    return false;
}

std::vector<std::string> EmpresaSanatorio::listarEspecialidadesTexto() const
{
    std::vector<std::string> out;
    out.reserve(cantidadEspecialidades);
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (!especialidades[i])
            continue;
        const auto *e = especialidades[i];
        out.push_back(
            "ID " + std::to_string(e->getId()) +
            " | " + e->getNombre());
    }
    return out;
}

// ================== SUBMENÚS Y MENÚ ==================

void EmpresaSanatorio::subMenuAgregar()
{
    std::string opcion;
    bool on = true;
    while (on)
    {
        std::cout << "\nOPCIONES PARA AGREGAR\n"
                  << "especialidades | pacientes | profesionales | sanatorios | volver\n"
                  << "Escriba una opcion: ";
        std::getline(std::cin, opcion);
        std::transform(opcion.begin(), opcion.end(), opcion.begin(), ::tolower);

        if (opcion == "pacientes")
        {
            agregarPaciente(nuevoPaciente());
        }
        else if (opcion == "especialidades")
        {
            agregarEspecialidad(nuevaEspecialidad());
        }
        else if (opcion == "profesionales")
        {
            if (Profesional *pr = nuevoProfesional())
                agregarProfesional(pr);
            else
                std::cout << "Profesional no creado (ID de especialidad inexistente).\n";
        }
        else if (opcion == "sanatorios")
        {
            std::cout << "No implementado aún.\n";
        }
        else if (opcion == "volver")
            on = false;
        else
            std::cout << "Opcion no valida.\n";
    }
}

void EmpresaSanatorio::subMenuMostrar()
{
    std::string opcion;
    bool on = true;
    while (on)
    {
        std::cout << "\nOPCIONES PARA MOSTRAR\n"
                  << "especialidades | pacientes | profesionales | sanatorios | volver\n"
                  << "Escriba una opcion: ";
        std::getline(std::cin, opcion);
        std::transform(opcion.begin(), opcion.end(), opcion.begin(), ::tolower);

        if (opcion == "pacientes")
        {
            for (int i = 0; i < cantidadPacientes; ++i)
                if (listaPacientes[i])
                    listaPacientes[i]->mostrar();
        }
        else if (opcion == "especialidades")
        {
            for (int i = 0; i < cantidadEspecialidades; ++i)
                if (especialidades[i])
                    especialidades[i]->mostrar();
        }
        else if (opcion == "profesionales")
        {
            for (int i = 0; i < cantidadProfesionales; ++i)
                if (profesionales[i])
                    profesionales[i]->mostrar();
        }
        else if (opcion == "sanatorios")
        {
            std::cout << "No implementado aún.\n";
        }
        else if (opcion == "volver")
            on = false;
        else
            std::cout << "Opcion no valida.\n";
    }
}

void EmpresaSanatorio::subMenuEliminar()
{
    std::string opcion;
    bool on = true;
    while (on)
    {
        std::cout << "\nOPCIONES PARA ELIMINAR\n"
                  << "especialidades | pacientes | profesionales | sanatorios | volver\n"
                  << "Escriba una opcion: ";
        std::getline(std::cin, opcion);
        std::transform(opcion.begin(), opcion.end(), opcion.begin(), ::tolower);

        if (opcion == "pacientes")
        {
            int id = validarEntero("ID a eliminar: ");
            std::cout << (eliminarPacientePorId(id) ? "Eliminado\n" : "ID inexistente\n");
        }
        else if (opcion == "especialidades")
        {
            int id = validarEntero("ID de especialidad a eliminar: ");
            std::cout << (eliminarEspecialidadPorId(id) ? "Eliminada\n" : "ID inexistente\n");
        }
        else if (opcion == "profesionales")
        {
            int id = validarEntero("ID de profesional a eliminar: ");
            std::cout << (eliminarProfesionalPorId(id) ? "Eliminado\n" : "ID inexistente\n");
        }
        else if (opcion == "sanatorios")
        {
            std::cout << "No implementado aún.\n";
        }
        else if (opcion == "volver")
            on = false;
        else
            std::cout << "Opcion no valida.\n";
    }
}

void EmpresaSanatorio::menu()
{
    bool funcionando = true;
    string opcion;

    while (funcionando)
    {
        cout << "\n===== MENU =====\n";
        cout << "Opciones: agregar | eliminar | mostrar | salir\n";
        cout << "Escriba una opcion: ";
        getline(cin, opcion);
        transform(opcion.begin(), opcion.end(), opcion.begin(), ::tolower);

        if (opcion == "agregar")
            subMenuAgregar();
        else if (opcion == "eliminar")
            subMenuEliminar();
        else if (opcion == "mostrar")
            subMenuMostrar();
        else if (opcion == "salir")
            funcionando = false;
        else
            cout << "Opcion no valida, por favor escriba una opcion\n";
    }
}

static bool parse_fecha_hora(const std::string &s, std::string &fechaOut, int &minOfDayOut)
{
    // Espera "YYYY-MM-DD HH:MM"
    int Y, M, D, h, m;
    if (std::sscanf(s.c_str(), "%d-%d-%d %d:%d", &Y, &M, &D, &h, &m) != 5)
        return false;
    if (Y < 1900 || M < 1 || M > 12 || D < 1 || D > 31 || h < 0 || h > 23 || m < 0 || m > 59)
        return false;
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d", Y, M, D);
    fechaOut = buf;
    minOfDayOut = h * 60 + m;
    return true;
}

static bool solapan(int aStart, int aDur, int bStart, int bDur)
{
    const int aEnd = aStart + aDur;
    const int bEnd = bStart + bDur;
    return (aStart < bEnd) && (bStart < aEnd);
}

// ================== Turnos: agendar/cancelar/listar ==================

bool EmpresaSanatorio::agendarTurno(int idTurno, int idPaciente, int idProfesional, int idEspecialidad,
                                    const std::string &fechaHora, int durMin, std::string &error)
{
    // Validaciones de existencia
    if (!buscarPacientePorId(idPaciente))
    {
        error = "Paciente inexistente";
        return false;
    }
    if (!buscarProfesionalPorId(idProfesional))
    {
        error = "Profesional inexistente";
        return false;
    }
    if (!buscarEspecialidadPorId(idEspecialidad))
    {
        error = "Especialidad inexistente";
        return false;
    }
    if (durMin <= 0)
    {
        error = "Duración inválida";
        return false;
    }

    // Parse fecha/hora
    std::string fecha;
    int minOfDay;
    if (!parse_fecha_hora(fechaHora, fecha, minOfDay))
    {
        error = "Fecha/Hora inválida";
        return false;
    }

    // ID de turno único
    for (const auto &t : agenda)
    {
        if (t.activo && t.id == idTurno)
        {
            error = "ID de turno ya usado";
            return false;
        }
    }

    // Regla de no solapamiento por profesional (misma fecha)
    for (const auto &t : agenda)
    {
        if (!t.activo)
            continue;
        if (t.profesionalId == idProfesional && t.fecha == fecha)
        {
            if (solapan(t.minOfDay, t.durMin, minOfDay, durMin))
            {
                error = "Solapamiento con otro turno del profesional";
                return false;
            }
        }
    }

    // Alta en agenda
    agenda.push_back(TurnoRec{
        idTurno, idPaciente, idProfesional, idEspecialidad, fecha, minOfDay, durMin, true});
    return true;
}

bool EmpresaSanatorio::cancelarTurnoPorId(int idTurno)
{
    for (auto &t : agenda)
    {
        if (t.id == idTurno && t.activo)
        {
            t.activo = false;
            return true;
        }
    }
    return false;
}

std::vector<std::string> EmpresaSanatorio::listarTurnosTexto() const
{
    std::vector<int> idx;
    idx.reserve(agenda.size());
    for (int i = 0; i < (int)agenda.size(); ++i)
        if (agenda[i].activo)
            idx.push_back(i);
    std::sort(idx.begin(), idx.end(), [&](int a, int b)
              {
        if (agenda[a].fecha != agenda[b].fecha) return agenda[a].fecha < agenda[b].fecha;
        return agenda[a].minOfDay < agenda[b].minOfDay; });

    std::vector<std::string> out;
    for (int k : idx)
    {
        const auto &t = agenda[k];
        const auto *pr = buscarProfesionalPorId(t.profesionalId);
        const auto *pa = buscarPacientePorId(t.pacienteId);
        const auto *es = buscarEspecialidadPorId(t.especialidadId);
        char hhmm[6];
        std::snprintf(hhmm, sizeof(hhmm), "%02d:%02d", t.minOfDay / 60, t.minOfDay % 60);
        out.push_back(
            "T#" + std::to_string(t.id) +
            " | " + t.fecha + " " + std::string(hhmm) +
            " | Prof: " + (pr ? pr->getApellido() + ", " + pr->getNombre() : "?") +
            " | Pac: " + (pa ? pa->getApellido() + ", " + pa->getNombre() : "?") +
            " | Esp: " + (es ? es->getNombre() : "?") +
            " | " + std::to_string(t.durMin) + " min");
    }
    return out;
}

std::vector<std::string> EmpresaSanatorio::listarTurnosPorProfesionalTexto(int idProfesional) const
{
    std::vector<int> idx;
    for (int i = 0; i < (int)agenda.size(); ++i)
        if (agenda[i].activo && agenda[i].profesionalId == idProfesional)
            idx.push_back(i);

    std::sort(idx.begin(), idx.end(), [&](int a, int b)
              {
        if (agenda[a].fecha != agenda[b].fecha) return agenda[a].fecha < agenda[b].fecha;
        return agenda[a].minOfDay < agenda[b].minOfDay; });

    std::vector<std::string> out;
    for (int k : idx)
    {
        const auto &t = agenda[k];
        const auto *pa = buscarPacientePorId(t.pacienteId);
        const auto *es = buscarEspecialidadPorId(t.especialidadId);
        char hhmm[6];
        std::snprintf(hhmm, sizeof(hhmm), "%02d:%02d", t.minOfDay / 60, t.minOfDay % 60);
        out.push_back(
            "T#" + std::to_string(t.id) +
            " | " + t.fecha + " " + std::string(hhmm) +
            " | Pac: " + (pa ? pa->getApellido() + ", " + pa->getNombre() : "?") +
            " | Esp: " + (es ? es->getNombre() : "?") +
            " | " + std::to_string(t.durMin) + " min");
    }
    return out;
}

// === Overloads const requeridos por listarTurnos* ===
const Paciente *EmpresaSanatorio::buscarPacientePorId(int id) const
{
    for (int i = 0; i < cantidadPacientes; ++i)
        if (listaPacientes[i] && listaPacientes[i]->getId() == id)
            return listaPacientes[i];
    return nullptr;
}

const Profesional *EmpresaSanatorio::buscarProfesionalPorId(int id) const
{
    for (int i = 0; i < cantidadProfesionales; ++i)
        if (profesionales[i] && profesionales[i]->getId() == id)
            return profesionales[i];
    return nullptr;
}

const Especialidad *EmpresaSanatorio::buscarEspecialidadPorId(int id) const
{
    for (int i = 0; i < cantidadEspecialidades; ++i)
        if (especialidades[i] && especialidades[i]->getId() == id)
            return especialidades[i];
    return nullptr;
}
