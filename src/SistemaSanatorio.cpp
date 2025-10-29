#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <cstdio>
#include <cstdlib> // Para system()
#include <ctime> // Para time() y mktime()
#include <cmath>

#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"
#include "Turno.h"
#include "Sanatorio.h"
#include "SistemaSanatorio.h"
#include "GeocodificadorAPI.h"


using namespace std;

static double calcularDistancia(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371.0; // Radio de la Tierra en km
    const double PI = 3.14159265358979323846;

    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;

    double a = sin(dLat/2) * sin(dLat/2) +
               cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) *
               sin(dLon/2) * sin(dLon/2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}
// ================== SERVICIOS DE NOTIFICACIONES ==================
void EmpresaSanatorio::iniciarServicioNotificaciones()
{
    if (!running_notificaciones)
    {
        running_notificaciones = true;
        worker_notificaciones = std::thread(&EmpresaSanatorio::revisarTurnosLoop, this);
    }
}

void EmpresaSanatorio::detenerServicioNotificaciones()
{
    running_notificaciones = false;
    if (worker_notificaciones.joinable())
    {
        worker_notificaciones.join();
    }
}

void EmpresaSanatorio::revisarTurnosLoop()
{
    while (running_notificaciones)
    {
        { // Bloque para el lock_guard
            std::lock_guard<std::mutex> lock(mtx);

            for (auto &turno : agenda)
            {
                if (turno.activo && !turno.recordatorioEnviado)
                {
                    // Lógica para verificar si es hora de enviar
                    std::tm turno_tm = {};
                    sscanf(turno.fecha.c_str(), "%d-%d-%d", &turno_tm.tm_year, &turno_tm.tm_mon, &turno_tm.tm_mday);
                    turno_tm.tm_hour = turno.minOfDay / 60;
                    turno_tm.tm_min = turno.minOfDay % 60;

                    turno_tm.tm_year -= 1900;
                    turno_tm.tm_mon -= 1;

                    time_t tiempoTurno = mktime(&turno_tm);
                    time_t tiempoRecordatorio = tiempoTurno - 30 * 60; // 30 mins antes
                    time_t tiempoActual = time(nullptr);

                    if (tiempoActual >= tiempoRecordatorio)
                    {
                        const Paciente* pac = buscarPacientePorId(turno.pacienteId);
                        if (pac && !pac->getMail().empty())
                        {
                            std::string nombreCompleto = pac->getNombre() + " " + pac->getApellido();
                            std::string hora_str;
                            char buf[6];
                            snprintf(buf, sizeof(buf), "%02d:%02d", turno_tm.tm_hour, turno_tm.tm_min);
                            hora_str = buf;

                            // Construimos el comando
                            std::string comando = "python scripts/enviar_mail.py \"" + nombreCompleto + "\" " + pac->getMail() + " " + turno.fecha + " " + hora_str;

                            // Descomentar para depurar el comando que se ejecuta
                            // message_center("DEBUG", comando.c_str());

                            system(comando.c_str());
                            turno.recordatorioEnviado = true;
                        }
                    }
                }
            }
        } // El lock_guard se libera aquí
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Revisar cada 5 seg
    }
}


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
                                          const std::string &obraSocial,
                                          const std::string &mail,
                                          const std::string &direccion,
                                          double lat,
                                          double lon)
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] && listaPacientes[i]->getId() == id)
        {
            Paciente *viejo = listaPacientes[i];
            listaPacientes[i] = new Paciente(id, nombre, apellido,mail,direccion, nroAfiliado, obraSocial,lat,lon);
            delete viejo;
            return;
        }
    }
}

// ================== CONSTRUCCIÓN/DESTRUCCIÓN ==================

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
    detenerServicioNotificaciones();
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
    string mail = validarTexto("Ingrese Mail: ");
    string direccion = validarTexto("Ingrese su direccion: ");
    auto coords = geocodificadorApi.obtenerCoordenadas(direccion);
    while (coords.first == 0.0 and coords.second == 0.0){
        direccion = validarTexto("Ingrese su direccion: ");
        coords = geocodificadorApi.obtenerCoordenadas(direccion);
    }
    return new Paciente(id, nombre, apellido, mail, direccion,
                        numAfiliado, obraSocial, coords.first, coords.second);
}

Profesional *EmpresaSanatorio::nuevoProfesional()
{
    int id = validarEntero("Ingrese ID del profesional: ");
    int numProfesional = validarEntero("Ingrese numero de profesional: ");
    int idEsp = validarEntero("Ingrese ID de especialidad: ");
    string nombre = validarTexto("Ingrese nombre: ");
    string apellido = validarTexto("Ingrese apellido: ");
    string mail = validarTexto("Ingrese su mail: ");

    Especialidad *esp = buscarEspecialidadPorId(idEsp);
    if (!esp)
    {
        cout << "Especialidad inexistente. Cancele o cargue la especialidad antes." << endl;
        return nullptr;
    }
    // Constructor: (numero, Especialidad, id, nombre, apellido)
    return new Profesional(numProfesional, *esp, id, nombre, apellido,mail);
}

Sanatorio *EmpresaSanatorio::nuevoSanatorio()
{
    string nombre = validarTexto("Ingrese nombre del sanatorio: ");
    string ubicacion = validarTexto("Ingrese ubicacion del sanatorio: ");
    double lat, lon;
    pair<double,double> coordenadasUbicacion = geocodificadorApi.obtenerCoordenadas(ubicacion);
    return new Sanatorio(nombre, ubicacion,coordenadasUbicacion.first,coordenadasUbicacion.second);
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
        const auto* pr = profesionales[i];
        out.push_back(
                "ID: " + std::to_string(pr->getId()) + " | " +
                pr->getApellido() + ", " + pr->getNombre() +
                " | Nº Prof: " + std::to_string(pr->getNumeroProfesional())
        );
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
        const auto* e = especialidades[i];
        out.push_back(
                "ID: " + std::to_string(e->getId()) + " | " + e->getNombre()
        );
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

// <-- MODIFICADO: Busca esta función y reemplázala completa
bool EmpresaSanatorio::agendarTurno(int idTurno, int idPaciente, int idProfesional, int idEspecialidad,
                                    const std::string &fechaHora, int durMin, std::string &error)
{
    std::lock_guard<std::mutex> lock(mtx);

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

    // <-- NUEVO: Buscar sanatorio donde trabaja el profesional
    int sanatorioIdx = -1;
    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->tieneProfesional(idProfesional))
        {
            sanatorioIdx = i;
            break;  // Tomar el primero encontrado
        }
    }

    if (sanatorioIdx == -1)
    {
        error = "Profesional no trabaja en ningún sanatorio";
        return false;
    }

    // Alta en agenda
    agenda.push_back(TurnoRec{
            idTurno, idPaciente, idProfesional, idEspecialidad, sanatorioIdx,  // <-- MODIFICADO: agregar sanatorioIdx
            fecha, minOfDay, durMin, true});
    return true;
}

bool EmpresaSanatorio::cancelarTurnoPorId(int idTurno)
{
    std::lock_guard<std::mutex> lock(mtx); // <-- AÑADIR LOCK
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

// <-- MODIFICADO: Busca esta función y reemplázala
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
        const auto *san = buscarSanatorioPorIndice(t.sanatorioIdx);  // <-- NUEVO

        char hhmm[6];
        std::snprintf(hhmm, sizeof(hhmm), "%02d:%02d", t.minOfDay / 60, t.minOfDay % 60);
        out.push_back(
                "T#" + std::to_string(t.id) +
                " | " + t.fecha + " " + std::string(hhmm) +
                " | Prof: " + (pr ? pr->getApellido() + ", " + pr->getNombre() : "?") +
                " | Pac: " + (pa ? pa->getApellido() + ", " + pa->getNombre() : "?") +
                " | Esp: " + (es ? es->getNombre() : "?") +
                " | San: " + (san ? san->getNombre() : "?") +  // <-- NUEVO
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


// =================== SANATORIOS ==================  // <-- NUEVO: sección completa

Sanatorio* EmpresaSanatorio::buscarSanatorioPorIndice(int idx)  // <-- NUEVO
{
    if (idx < 0 || idx >= cantidadSanatorios)
        return nullptr;
    return sanatorios[idx];
}

const Sanatorio* EmpresaSanatorio::buscarSanatorioPorIndice(int idx) const  // <-- NUEVO
{
    if (idx < 0 || idx >= cantidadSanatorios)
        return nullptr;
    return sanatorios[idx];
}

void EmpresaSanatorio::agrandarListaSanatorios()  // <-- NUEVO
{
    int capacidadNueva = (capacidadSanatorios == 0) ? 4 : capacidadSanatorios * 2;
    auto **listaNueva = new Sanatorio *[capacidadNueva];
    for (int i = 0; i < cantidadSanatorios; ++i)
        listaNueva[i] = sanatorios[i];
    delete[] sanatorios;
    sanatorios = listaNueva;
    capacidadSanatorios = capacidadNueva;
}

void EmpresaSanatorio::agregarSanatorio(Sanatorio* s)  // <-- NUEVO
{
    if (!s)
        return;
    if (capacidadSanatorios == cantidadSanatorios)
        agrandarListaSanatorios();
    sanatorios[cantidadSanatorios++] = s;
}


void EmpresaSanatorio::agregarEspecialidadASanatorio(int indiceSanatorio, Especialidad* esp)  // <-- NUEVO
{
    if (!esp)
        return;

    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->agregarEspecialidad(esp);
    }
}

void EmpresaSanatorio::eliminarEspecialidadDeSanatorio(int indiceSanatorio, int idEspecialidad)  // <-- NUEVO
{
    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->eliminarEspecialidad(idEspecialidad);
    }
}

// ============================================================================
// <-- NUEVAS IMPLEMENTACIONES: Gestión de profesionales en sanatorios
// ============================================================================

void EmpresaSanatorio::agregarProfesionalASanatorio(int indiceSanatorio, Profesional* prof)  // <-- NUEVO
{
    if (!prof)
        return;

    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->agregarProfesional(prof);
    }
}

void EmpresaSanatorio::eliminarProfesionalDeSanatorio(int indiceSanatorio, int idProfesional)  // <-- NUEVO
{
    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->eliminarProfesional(idProfesional);
    }
}


// ============================================================================
// <-- NUEVAS IMPLEMENTACIONES: Búsqueda de sanatorios por especialidad/profesional
// ============================================================================

std::vector<std::pair<int, double>> EmpresaSanatorio::buscarSanatoriosPorEspecialidad(  // <-- NUEVO
        int idEspecialidad,
        double latPaciente,
        double lonPaciente
) const
{
    std::vector<std::pair<int, double>> resultado;  // pair<indiceSanatorio, distancia>

    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->tieneEspecialidad(idEspecialidad))
        {
            double dist = calcularDistancia(
                    latPaciente, lonPaciente,
                    sanatorios[i]->getLatitud(),
                    sanatorios[i]->getLongitud()
            );
            resultado.push_back({i, dist});
        }
    }

    // Ordenar por distancia (menor a mayor)
    std::sort(resultado.begin(), resultado.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    return resultado;
}

std::vector<std::pair<int, double>> EmpresaSanatorio::buscarSanatoriosPorProfesional(  // <-- NUEVO
        int idProfesional,
        double latPaciente,
        double lonPaciente
) const
{
    std::vector<std::pair<int, double>> resultado;

    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->tieneProfesional(idProfesional))
        {
            double dist = calcularDistancia(
                    latPaciente, lonPaciente,
                    sanatorios[i]->getLatitud(),
                    sanatorios[i]->getLongitud()
            );
            resultado.push_back({i, dist});
        }
    }

    // Ordenar por distancia
    std::sort(resultado.begin(), resultado.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    return resultado;
}

std::vector<int> EmpresaSanatorio::obtenerProfesionalesPorEspecialidad(int idEspecialidad) const  // <-- NUEVO
{
    std::vector<int> resultado;

    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] &&
            profesionales[i]->getEspecialidad().getId() == idEspecialidad)
        {
            resultado.push_back(profesionales[i]->getId());
        }
    }

    return resultado;
}

std::vector<int> EmpresaSanatorio::obtenerProfesionalesPorSanatorio(int indiceSanatorio) const  // <-- NUEVO
{
    std::vector<int> resultado;

    const Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (!san)
        return resultado;

    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] && san->tieneProfesional(profesionales[i]->getId()))
        {
            resultado.push_back(profesionales[i]->getId());
        }
    }

    return resultado;
}


// ============================================================================
// ✅ NUEVAS IMPLEMENTACIONES: Búsqueda por nombre
// ============================================================================

Paciente *EmpresaSanatorio::buscarPacientePorNombre(const std::string &nombre, const std::string &apellido)
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] &&
            listaPacientes[i]->getNombre() == nombre &&
            listaPacientes[i]->getApellido() == apellido)
        {
            return listaPacientes[i];
        }
    }
    return nullptr;
}

const Paciente *EmpresaSanatorio::buscarPacientePorNombre(const std::string &nombre, const std::string &apellido) const
{
    for (int i = 0; i < cantidadPacientes; ++i)
    {
        if (listaPacientes[i] &&
            listaPacientes[i]->getNombre() == nombre &&
            listaPacientes[i]->getApellido() == apellido)
        {
            return listaPacientes[i];
        }
    }
    return nullptr;
}

Profesional *EmpresaSanatorio::buscarProfesionalPorNombre(const std::string &nombre, const std::string &apellido)
{
    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] &&
            profesionales[i]->getNombre() == nombre &&
            profesionales[i]->getApellido() == apellido)
        {
            return profesionales[i];
        }
    }
    return nullptr;
}

const Profesional *EmpresaSanatorio::buscarProfesionalPorNombre(const std::string &nombre, const std::string &apellido) const
{
    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] &&
            profesionales[i]->getNombre() == nombre &&
            profesionales[i]->getApellido() == apellido)
        {
            return profesionales[i];
        }
    }
    return nullptr;
}

Especialidad *EmpresaSanatorio::buscarEspecialidadPorNombre(const std::string &nombre)
{
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (especialidades[i] && especialidades[i]->getNombre() == nombre)
        {
            return especialidades[i];
        }
    }
    return nullptr;
}

const Especialidad *EmpresaSanatorio::buscarEspecialidadPorNombre(const std::string &nombre) const
{
    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (especialidades[i] && especialidades[i]->getNombre() == nombre)
        {
            return especialidades[i];
        }
    }
    return nullptr;
}

std::vector<int> EmpresaSanatorio::obtenerProfesionalesPorEspecialidadEnSanatorio(int idEspecialidad, int indiceSanatorio) const
{
    std::vector<int> resultado;

    const Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (!san)
        return resultado;

    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] &&
            profesionales[i]->getEspecialidad().getId() == idEspecialidad &&
            san->tieneProfesional(profesionales[i]->getId()))
        {
            resultado.push_back(profesionales[i]->getId());
        }
    }

    return resultado;
}

