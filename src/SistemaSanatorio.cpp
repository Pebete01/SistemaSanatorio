#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <sstream>
#include <cstdio>
#include <cstdlib> // Para system()
#include <ctime> // Para time() y mktime()
#include <cmath>
#include "iomanip"

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

Paciente* EmpresaSanatorio::agregarPaciente(
        const std::string& nombre, const std::string& apellido,
        const std::string& mail, const std::string& direccion,
        int nroAfiliado, const std::string& obraSocial,
        double lat, double lon, // <-- Usamos las coordenadas recibidas
        std::string& error)
{
    try
    {
        // 1. Generamos el ID (nuestra lógica autoincremental)
        int nuevoId = proximoIdPaciente++;

        // 2. Creamos el objeto (usando las coordenadas validadas por la UI)
        Paciente* p = new Paciente(nuevoId, nombre, apellido, mail, direccion,
                                   nroAfiliado, obraSocial, lat, lon);

        // 3. Agregamos al array (tu lógica original)
        if (capacidadPacientes == cantidadPacientes)
            agrandarListaPaciente();
        listaPacientes[cantidadPacientes++] = p;

        return p;
    }
    catch (const std::exception& e)
    {
        error = e.what();
        return nullptr;
    }
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

Especialidad* EmpresaSanatorio::agregarEspecialidad(const std::string& nombre, std::string& error)
{
    // Buena práctica: validar si ya existe
    if (buscarEspecialidadPorNombre(nombre)) {
        error = "Esa especialidad ya existe";
        return nullptr;
    }

    try
    {
        int nuevoId = proximoIdEspecialidad++;
        Especialidad* esp = new Especialidad(nuevoId, nombre);

        if (capacidadEspecialidad == cantidadEspecialidades)
            agrandarListaEspecialidad(); // (Asegúrate que esta función exista)
        especialidades[cantidadEspecialidades++] = esp;
        return esp;
    }
    catch (const std::exception& e)
    {
        error = e.what();
        return nullptr;
    }
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

Profesional* EmpresaSanatorio::agregarProfesional(
        int nroMatricula, const Especialidad& esp,
        const std::string& nombre, const std::string& apellido,
        const std::string& mail, std::string& error)
{
    try
    {
        int nuevoId = proximoIdProfesional++;
        // El constructor es: Profesional(int numero, const Especialidad &esp, int id, ...)
        Profesional* p = new Profesional(nroMatricula, esp, nuevoId, nombre, apellido, mail);

        if (capacidadProfesionales == cantidadProfesionales)
            agrandarListaProfesionales(); // (Asegúrate que esta función exista)
        profesionales[cantidadProfesionales++] = p;
        return p;
    }
    catch (const std::exception& e)
    {
        error = e.what();
        return nullptr;
    }
}





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

int EmpresaSanatorio::agendarTurno(int idPaciente, int idProfesional, int sanatorioIdx,
                                   int idEspecialidad, const std::string &fechaHora, int durMin, std::string &error){
    // --- 1. Validaciones (El código que ya tenías) ---
    Paciente* pac = buscarPacientePorId(idPaciente);
    Profesional* prof = buscarProfesionalPorId(idProfesional);
    Especialidad* esp = buscarEspecialidadPorId(idEspecialidad); // <-- NUEVA VALIDACIÓN

    if (!pac) {
        error = "Paciente no encontrado";
        return 0; // 0 (o -1) para indicar error
    }
    if (!prof) {
        error = "Profesional no encontrado";
        return 0;
    }
    if (!esp) { // <-- NUEVA VALIDACIÓN
        error = "Especialidad no encontrada";
        return 0;
    }

    // --- 2. VALIDACIÓN DE COHERENCIA ---
    // Verificamos que el profesional tenga la especialidad solicitada
    if (prof->getEspecialidad().getId() != idEspecialidad) { // <-- NUEVA VALIDACIÓN
        error = "El profesional no pertenece a la especialidad indicada.";
        return 0;
    }

    // --- 2. Parsear Fecha/Hora (El código que ya tenías) ---
    std::tm tm = {};
    std::istringstream ss(fechaHora);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M"); // (Asegúrate que <iomanip> esté incluido)

    if (ss.fail()) {
        error = "Formato de fecha/hora invalido. Usar YYYY-MM-DD HH:MM";
        return 0;
    }
    int h = tm.tm_hour;
    int m = tm.tm_min;
    int minOfDay = h * 60 + m;

    char fechaBuffer[11];
    std::strftime(fechaBuffer, 11, "%Y-%m-%d", &tm); // (Asegúrate que <ctime> esté incluido)
    std::string fechaStr = fechaBuffer;

    char horaBuffer[6];
    std::strftime(horaBuffer, 6, "%H:%M", &tm);
    std::string horaStr = horaBuffer;


    // --- 3. Verificar Disponibilidad (El código que ya tenías) ---
    // (¡Importante! Esto debe hacerse ANTES de bloquear el mutex de la lista de turnos)
    if (!prof->reservarTurno(fechaStr, horaStr)) {
        error = "El horario ya no esta disponible o es invalido";
        return 0;
    }

    // --- 4. Generar ID y Crear el Turno ---
    int nuevoIdTurno = proximoIdTurno++; // Generamos el ID autoincremental

    // ¡Importante! Bloqueamos el mutex antes de modificar el vector de turnos,
    // para protegerlo del hilo de notificaciones.
    std::lock_guard<std::mutex> lock(mtx);

    // Usando la forma exacta que pediste (asumiendo que tu vector se llama 'turnos'):
    agenda.push_back(TurnoRec{
            nuevoIdTurno,   // idTurno
            idPaciente,
            idProfesional,
            idEspecialidad,
            sanatorioIdx,
            fechaStr,       // fecha ("YYYY-MM-DD")
            minOfDay,       // minOfDay (HH*60+MM)
            durMin,
            true,           // activo
            false           // notificado (asumo que 'false' es el default)
    });

    // --- 5. Devolver el nuevo ID ---
    return nuevoIdTurno;
}

bool EmpresaSanatorio::cancelarTurnoPorId(int idTurno)
{
    std::lock_guard<std::mutex> lock(mtx);
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
        const auto *san = buscarSanatorioPorIndice(t.sanatorioIdx);

        char hhmm[6];
        std::snprintf(hhmm, sizeof(hhmm), "%02d:%02d", t.minOfDay / 60, t.minOfDay % 60);
        out.push_back(
                "T#" + std::to_string(t.id) +
                " | " + t.fecha + " " + std::string(hhmm) +
                " | Prof: " + (pr ? pr->getApellido() + ", " + pr->getNombre() : "?") +
                " | Pac: " + (pa ? pa->getApellido() + ", " + pa->getNombre() : "?") +
                " | Esp: " + (es ? es->getNombre() : "?") +
                " | San: " + (san ? san->getNombre() : "?") +
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



Sanatorio* EmpresaSanatorio::buscarSanatorioPorIndice(int idx)
{
    if (idx < 0 || idx >= cantidadSanatorios)
        return nullptr;
    return sanatorios[idx];
}

const Sanatorio* EmpresaSanatorio::buscarSanatorioPorIndice(int idx) const
{
    if (idx < 0 || idx >= cantidadSanatorios)
        return nullptr;
    return sanatorios[idx];
}

void EmpresaSanatorio::agrandarListaSanatorios()
{
    int capacidadNueva = (capacidadSanatorios == 0) ? 4 : capacidadSanatorios * 2;
    auto **listaNueva = new Sanatorio *[capacidadNueva];
    for (int i = 0; i < cantidadSanatorios; ++i)
        listaNueva[i] = sanatorios[i];
    delete[] sanatorios;
    sanatorios = listaNueva;
    capacidadSanatorios = capacidadNueva;
}

Sanatorio* EmpresaSanatorio::agregarSanatorio(
        const std::string& nombre, const std::string& direccion,
        double lat, double lon,
        std::string& error)
{
    try
    {
        int nuevoId = proximoIdSanatorio++;
        Sanatorio* s = new Sanatorio(nuevoId, nombre, direccion, lat, lon);

        if (capacidadSanatorios == cantidadSanatorios)
            agrandarListaSanatorios(); // (Asegúrate que esta función exista)
        sanatorios[cantidadSanatorios++] = s;
        return s;
    }
    catch (const std::exception& e)
    {
        error = e.what();
        return nullptr;
    }
}


void EmpresaSanatorio::agregarEspecialidadASanatorio(int indiceSanatorio, Especialidad* esp)
{
    if (!esp)
        return;

    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->agregarEspecialidad(esp);
    }
}

void EmpresaSanatorio::eliminarEspecialidadDeSanatorio(int indiceSanatorio, int idEspecialidad)
{
    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->eliminarEspecialidad(idEspecialidad);
    }
}

void EmpresaSanatorio::agregarProfesionalASanatorio(int indiceSanatorio, Profesional* prof)
{
    if (!prof)
        return;

    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->agregarProfesional(prof);
    }
}

void EmpresaSanatorio::eliminarProfesionalDeSanatorio(int indiceSanatorio, int idProfesional)
{
    Sanatorio* san = buscarSanatorioPorIndice(indiceSanatorio);
    if (san)
    {
        san->eliminarProfesional(idProfesional);
    }
}


std::vector<std::pair<int, double>> EmpresaSanatorio::buscarSanatoriosPorEspecialidad(
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

std::vector<std::pair<int, double>> EmpresaSanatorio::buscarSanatoriosPorProfesional(
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

std::vector<int> EmpresaSanatorio::obtenerProfesionalesPorEspecialidad(int idEspecialidad) const
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

std::vector<int> EmpresaSanatorio::obtenerProfesionalesPorSanatorio(int indiceSanatorio) const
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


Sanatorio* EmpresaSanatorio::buscarSanatorioPorId(int id)
{
    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->getId() == id)
            return sanatorios[i];
    }
    return nullptr;
}

const Sanatorio* EmpresaSanatorio::buscarSanatorioPorId(int id) const
{
    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->getId() == id)
            return sanatorios[i];
    }
    return nullptr;
}

bool EmpresaSanatorio::eliminarSanatorioPorId(int id, std::string& error)
{
    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->getId() == id)
        {
            // Verificar si tiene turnos activos
            for (const auto& turno : agenda)
            {
                if (turno.activo && turno.sanatorioIdx == i)
                {
                    error = "No se puede eliminar: el sanatorio tiene turnos activos";
                    return false;
                }
            }

            delete sanatorios[i];
            for (int j = i + 1; j < cantidadSanatorios; ++j)
                sanatorios[j - 1] = sanatorios[j];
            --cantidadSanatorios;

            // Actualizar índices de sanatorios en turnos
            for (auto& turno : agenda)
            {
                if (turno.sanatorioIdx > i)
                    turno.sanatorioIdx--;
            }

            return true;
        }
    }
    error = "Sanatorio no encontrado";
    return false;
}

std::vector<std::string> EmpresaSanatorio::listarEspecialidadesDeSanatorio(int idSanatorio) const
{
    std::vector<std::string> resultado;

    const Sanatorio* san = buscarSanatorioPorId(idSanatorio);
    if (!san)
        return resultado;

    for (int i = 0; i < cantidadEspecialidades; ++i)
    {
        if (especialidades[i] && san->tieneEspecialidad(especialidades[i]->getId()))
        {
            resultado.push_back(
                    "ID: " + std::to_string(especialidades[i]->getId()) +
                    " | " + especialidades[i]->getNombre()
            );
        }
    }

    return resultado;
}

std::vector<std::string> EmpresaSanatorio::listarProfesionalesDeSanatorio(int idSanatorio) const
{
    std::vector<std::string> resultado;

    const Sanatorio* san = buscarSanatorioPorId(idSanatorio);
    if (!san)
        return resultado;

    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        if (profesionales[i] && san->tieneProfesional(profesionales[i]->getId()))
        {
            resultado.push_back(
                    "ID: " + std::to_string(profesionales[i]->getId()) +
                    " | " + profesionales[i]->getApellido() + ", " + profesionales[i]->getNombre() +
                    " | Especialidad: " + profesionales[i]->getEspecialidad().getNombre()
            );
        }
    }

    return resultado;
}

std::vector<std::string> EmpresaSanatorio::listarPacientesDeSanatorio(int idSanatorio) const
{
    std::vector<std::string> resultado;
    std::vector<int> pacientesIds;

    // Buscar pacientes que tengan turnos en este sanatorio
    for (int i = 0; i < cantidadSanatorios; ++i)
    {
        if (sanatorios[i] && sanatorios[i]->getId() == idSanatorio)
        {
            for (const auto& turno : agenda)
            {
                if (turno.activo && turno.sanatorioIdx == i)
                {
                    // Evitar duplicados
                    bool yaAgregado = false;
                    for (int id : pacientesIds)
                    {
                        if (id == turno.pacienteId)
                        {
                            yaAgregado = true;
                            break;
                        }
                    }
                    if (!yaAgregado)
                        pacientesIds.push_back(turno.pacienteId);
                }
            }
            break;
        }
    }

    // Generar lista de texto
    for (int id : pacientesIds)
    {
        const Paciente* pac = buscarPacientePorId(id);
        if (pac)
        {
            resultado.push_back(
                    "ID: " + std::to_string(pac->getId()) +
                    " | " + pac->getApellido() + ", " + pac->getNombre() +
                    " | Obra Social: " + pac->getObraSocial()
            );
        }
    }

    return resultado;
}