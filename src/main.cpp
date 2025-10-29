#include "SistemaSanatorio.h"
#include "menu.h"

#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "Sanatorio.h"
#include "DatosMock.h" // para datos mock

#include <vector>
#include <stdexcept>
#include <string>
#include <cmath>

// ---------------- utils ----------------
static int to_int(const std::string &s)
{
    size_t p = 0;
    int v = std::stoi(s, &p);
    if (p != s.size())
        throw std::invalid_argument("nan");
    return v;
}

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


// ---------------- Sanatorios ---------------

static void ui_agregar_sanatorio(EmpresaSanatorio &app)
{
    std::string nombre = input_box("Sanatorios - Agregar", "Nombre:", 50);
    std::string direccion;
    std::pair<double, double> coordenadas;

    while (true) {
        direccion = input_box("Sanatorios - Agregar", "Direccion:", 50);
        coordenadas = app.geocodificarDireccion(direccion);

        if (coordenadas.first != 0.0 || coordenadas.second != 0.0)
            break;
        message_center("Error", "Direccion invalida. Intente nuevamente.");
    }

    if (!confirm_box("Confirmar", "Guardar?"))
    {
        message_center("Alta", "Cancelado");
        return;
    }

    try
    {
        Sanatorio* nuevo = new Sanatorio(nombre, direccion,
                                         coordenadas.first, coordenadas.second);
        app.agregarSanatorio(nuevo);
        message_center("Alta", "Sanatorio agregado correctamente");
    }
    catch (const std::exception &e)
    {
        message_center("Error", std::string(e.what()));
    }
}

static void ui_listar_sanatorios(EmpresaSanatorio &app)
{
    int cant = app.getCantidadSanatorios();
    if (cant == 0)
    {
        message_center("Sanatorios", "No hay sanatorios registrados");
        return;
    }

    std::vector<std::string> lista;
    Sanatorio** sanatorios = app.getSanatorios();

    for (int i = 0; i < cant; ++i)
    {
        if (sanatorios[i])
        {
            std::string linea =
                    std::to_string(i + 1) + ". " +
                    sanatorios[i]->getNombre() +
                    " | " + sanatorios[i]->getDireccion();
            lista.push_back(linea);
        }
    }

    list_box("Sanatorios", lista);
}


// ---------------- Pacientes ----------------
static void ui_agregar_paciente(EmpresaSanatorio &app)
{

    std::string sid = input_box("Pacientes - Agregar", "ID:", 10);
    std::string snaf = input_box("Pacientes - Agregar", "Nro Afiliado:", 10);
    std::string nombre = input_box("Pacientes - Agregar", "Nombre:", 40);
    std::string apellido = input_box("Pacientes - Agregar", "Apellido:", 40);
    std::string direccion;
    std::pair<double, double> direcCoordenadas;

    while (true) {
        direccion = input_box("Pacientes - Agregar", "Direccion:", 50);
        direcCoordenadas = app.geocodificarDireccion(direccion);

        if (direcCoordenadas.first != 0.0 || direcCoordenadas.second != 0.0)
            break; // Dirección válida
        message_center("Error", "Direccion invalida. Intente nuevamente.");
    }

    std::string mail = input_box("Pacientes - Agregar", "Email:", 50);
    std::string obra = input_box("Pacientes - Agregar", "Obra social:", 40);

    if (!confirm_box("Confirmar", "Guardar?"))
    {
        message_center("Alta", "Cancelado");
        return;
    }
    try
    {
        int id = to_int(sid), naf = to_int(snaf);

        app.agregarPaciente(new Paciente(id, nombre, apellido, mail, direccion, naf, obra,direcCoordenadas.first, direcCoordenadas.second));
        message_center("Alta", "OK");
    }
    catch (const std::exception &e)
    {
        message_center("Alta", "Error: " + std::string(e.what()));
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
        std::string mail = input_box("Editar Paciente", "Mail (" + p->getMail() + "):", 40);
//AGREGAR LAT Y LON ASI COMO ABAJO
        std::string direccion;
        std::pair<double, double> direcCoordenadas;

        while (true) {
            direccion = input_box("Editar Paciente", "Direccion (" + p->getDireccion() + "):", 50);
            if(direccion.empty())break;
            direcCoordenadas = app.geocodificarDireccion(direccion);

            if (direcCoordenadas.first != 0.0 || direcCoordenadas.second != 0.0)
                break; // Dirección válida
            message_center("Error", "Direccion invalida. Intente nuevamente.");
        }

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
                               obra.empty() ? p->getObraSocial() : obra,
                               mail.empty() ? p->getMail() : mail,
                               direccion.empty() ? p->getDireccion() : direccion,
                               direccion.empty() ? p->getLatitud() : direcCoordenadas.first,
                               direccion.empty() ? p->getLongitud() : direcCoordenadas.second);

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
    if (app.getCantidadSanatorios() == 0)
    {
        message_center("Error", "Primero debe agregar sanatorios");
        return;
    }

    std::string sid = input_box("Especialidades - Agregar", "ID:", 10);
    std::string nom = input_box("Especialidades - Agregar", "Nombre:", 40);

    std::vector<std::string> listaSanatorios;
    Sanatorio** sanatorios = app.getSanatorios();
    int cantSan = app.getCantidadSanatorios();

    for (int i = 0; i < cantSan; ++i)
    {
        if (sanatorios[i])
        {
            listaSanatorios.push_back(
                    std::to_string(i + 1) + ". " + sanatorios[i]->getNombre()
            );
        }
    }


    std::string sSan = input_box_with_list(
            "Especialidades - Agregar",
            listaSanatorios,
            "Numeros de sanatorios (ej: 1,2,4) o 'todos':",
            50
    );


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

        // ✅ NUEVO: Crear especialidad una vez
        Especialidad* nueva = new Especialidad(id, nom);
        app.agregarEspecialidad(nueva);

        // ✅ NUEVO: Procesar selección de sanatorios
        std::vector<int> indicesSanatorios;
        int cantSan = app.getCantidadSanatorios();

        if (sSan == "todos" || sSan == "TODOS")
        {
            // Agregar a todos
            for (int i = 0; i < cantSan; ++i)
            {
                indicesSanatorios.push_back(i);
            }
        }
        else
        {
            // Parsear números separados por coma
            std::string numero;
            sSan += ','; // Para procesar el último número

            for (char c : sSan)
            {
                if (c == ',' || c == ' ')
                {
                    if (!numero.empty())
                    {
                        try
                        {
                            int n = to_int(numero) - 1; // Convertir a índice base 0
                            if (n >= 0 && n < cantSan)
                            {
                                indicesSanatorios.push_back(n);
                            }
                            numero.clear();
                        }
                        catch (...)
                        {
                            // Ignorar números inválidos
                        }
                    }
                }
                else
                {
                    numero += c;
                }
            }
        }

        // ✅ NUEVO: Agregar a todos los sanatorios seleccionados
        for (int idx : indicesSanatorios)
        {
            app.agregarEspecialidadASanatorio(idx, nueva);
        }

        message_center("Alta",
                       "Especialidad guardada en " + std::to_string(indicesSanatorios.size()) + " sanatorio(s)");
    }
    catch (...)
    {
        message_center("Error", "Dato invalido");
    }
}


static void ui_eliminar_especialidad(EmpresaSanatorio &app)
{
    std::string sid = input_box("Especialidades - Eliminar", "ID:", 10);

    try
    {
        int id = to_int(sid);
        Especialidad* esp = app.buscarEspecialidadPorId(id);

        if (!esp)
        {
            message_center("Error", "Especialidad inexistente");
            return;
        }

        // ✅ NUEVO: Preguntar de dónde eliminar
        std::vector<std::string> opciones = {
                "Eliminar de sanatorios especificos",
                "Eliminar de TODOS los sanatorios"
        };

        int eleccion = run_submenu("Eliminar Especialidad", opciones);

        if (eleccion < 0)
        {
            message_center("Eliminar", "Cancelado");
            return;
        }

        if (eleccion == 0)  // ✅ Eliminar de específicos
        {
            // Mostrar sanatorios que tienen esta especialidad
            std::vector<std::string> sanatoriosConEsp;
            std::vector<int> indicesSanatorios;

            Sanatorio** sanatorios = app.getSanatorios();
            int cantSan = app.getCantidadSanatorios();

            for (int i = 0; i < cantSan; ++i)
            {
                if (sanatorios[i] && sanatorios[i]->tieneEspecialidad(id))
                {
                    sanatoriosConEsp.push_back(
                            std::to_string(indicesSanatorios.size() + 1) + ". " +
                            sanatorios[i]->getNombre()
                    );
                    indicesSanatorios.push_back(i);
                }
            }

            if (sanatoriosConEsp.empty())
            {
                message_center("Info", "Esta especialidad no esta en ningun sanatorio");
                return;
            }

            list_box("Sanatorios con esta especialidad", sanatoriosConEsp);

            // ✅ NUEVO: Pedir múltiples sanatorios separados por coma
            std::string seleccion = input_box(
                    "Eliminar",
                    "Numeros separados por coma (ej: 1,3,5) o 'todos':",
                    50
            );

            if (!confirm_box("Confirmar", "Eliminar de los sanatorios seleccionados?"))
            {
                message_center("Eliminar", "Cancelado");
                return;
            }

            // Procesar selección
            if (seleccion == "todos" || seleccion == "TODOS")
            {
                // Eliminar de todos los sanatorios que la tienen
                for (int idx : indicesSanatorios)
                {
                    app.eliminarEspecialidadDeSanatorio(idx, id);
                }
                message_center("Eliminar", "Especialidad eliminada de todos los sanatorios");
            }
            else
            {
                // Parsear números separados por coma
                std::vector<int> numerosSeleccionados;
                std::string numero;
                seleccion += ','; // Para procesar el último número

                for (char c : seleccion)
                {
                    if (c == ',' || c == ' ')
                    {
                        if (!numero.empty())
                        {
                            try
                            {
                                int n = to_int(numero) - 1; // Convertir a índice base 0
                                if (n >= 0 && n < (int)indicesSanatorios.size())
                                {
                                    numerosSeleccionados.push_back(indicesSanatorios[n]);
                                }
                                numero.clear();
                            }
                            catch (...)
                            {
                                // Ignorar números inválidos
                            }
                        }
                    }
                    else
                    {
                        numero += c;
                    }
                }

                // Eliminar de los seleccionados
                for (int idx : numerosSeleccionados)
                {
                    app.eliminarEspecialidadDeSanatorio(idx, id);
                }

                message_center("Eliminar",
                               "Eliminada de " + std::to_string(numerosSeleccionados.size()) + " sanatorio(s)");
            }
        }
        else  // ✅ Eliminar de TODOS
        {
            if (!confirm_box("CONFIRMAR", "Eliminar de TODOS los sanatorios Y del sistema?"))
            {
                message_center("Eliminar", "Cancelado");
                return;
            }

            // Eliminar completamente (de todos los sanatorios y del sistema global)
            bool ok = app.eliminarEspecialidadPorId(id);
            message_center("Eliminar", ok ? "Especialidad eliminada completamente" : "Error al eliminar");
        }
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}

static void ui_listar_especialidades(EmpresaSanatorio &app)
{
    auto v = app.listarEspecialidadesTexto();
    v.empty() ? message_center("Especialidades", "No hay registros") : list_box("Especialidades", v);
}

// ---------------- Profesionales ----------------
// ============================================================================
// ✅ MODIFICADO: Agregar profesional a múltiples sanatorios
// ============================================================================

static void ui_agregar_profesional(EmpresaSanatorio &app)
{
    if (app.getCantidadSanatorios() == 0)
    {
        message_center("Error", "Primero debe agregar sanatorios");
        return;
    }

    std::string sid = input_box("Profesionales - Agregar", "ID:", 10);
    std::string snum = input_box("Profesionales - Agregar", "Nro Profesional:", 10);
    std::string sidE = input_box("Profesionales - Agregar", "ID Especialidad:", 10);
    std::string nom = input_box("Profesionales - Agregar", "Nombre:", 40);
    std::string ape = input_box("Profesionales - Agregar", "Apellido:", 40);
    std::string mail = input_box("Profesionales - Agregar", "Email:", 50);

    std::vector<std::string> listaSanatorios;
    Sanatorio** sanatorios = app.getSanatorios();
    int cantSan = app.getCantidadSanatorios();

    for (int i = 0; i < cantSan; ++i)
    {
        if (sanatorios[i])
        {
            listaSanatorios.push_back(
                    std::to_string(i + 1) + ". " + sanatorios[i]->getNombre()
            );
        }
    }


    std::string sSan = input_box_with_list(
            "Profesionales - Agregar",
            listaSanatorios,
            "Numeros de sanatorios (ej: 1,2,4) o 'todos':",
            50
    );

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

        // ✅ NUEVO: Crear profesional una vez
        Profesional* nuevo = new Profesional(num, *esp, id, nom, ape, mail);
        app.agregarProfesional(nuevo);

        // ✅ NUEVO: Procesar selección de sanatorios
        std::vector<int> indicesSanatorios;
        int cantSan = app.getCantidadSanatorios();

        if (sSan == "todos" || sSan == "TODOS")
        {
            for (int i = 0; i < cantSan; ++i)
            {
                indicesSanatorios.push_back(i);
            }
        }
        else
        {
            // Parsear números
            std::string numero;
            sSan += ',';

            for (char c : sSan)
            {
                if (c == ',' || c == ' ')
                {
                    if (!numero.empty())
                    {
                        try
                        {
                            int n = to_int(numero) - 1;
                            if (n >= 0 && n < cantSan)
                            {
                                indicesSanatorios.push_back(n);
                            }
                            numero.clear();
                        }
                        catch (...) {}
                    }
                }
                else
                {
                    numero += c;
                }
            }
        }

        // ✅ NUEVO: Agregar a todos los sanatorios seleccionados
        for (int idx : indicesSanatorios)
        {
            app.agregarProfesionalASanatorio(idx, nuevo);
        }

        message_center("Alta",
                       "Profesional guardado en " + std::to_string(indicesSanatorios.size()) + " sanatorio(s)");
    }
    catch (...)
    {
        message_center("Error", "Campos numericos invalidos");
    }
}

// ============================================================================
// ✅ MODIFICADO: Eliminar profesional de sanatorios específicos
// ============================================================================

static void ui_eliminar_profesional(EmpresaSanatorio &app)
{
    std::string sid = input_box("Profesionales - Eliminar", "ID:", 10);

    try
    {
        int id = to_int(sid);
        Profesional* prof = app.buscarProfesionalPorId(id);

        if (!prof)
        {
            message_center("Error", "Profesional inexistente");
            return;
        }

        // ✅ NUEVO: Preguntar de dónde eliminar
        std::vector<std::string> opciones = {
                "Eliminar de sanatorios especificos",
                "Eliminar de TODOS los sanatorios"
        };

        int eleccion = run_submenu("Eliminar Profesional", opciones);

        if (eleccion < 0)
        {
            message_center("Eliminar", "Cancelado");
            return;
        }

        if (eleccion == 0)  // Eliminar de específicos
        {
            // Mostrar sanatorios donde trabaja
            std::vector<std::string> sanatoriosConProf;
            std::vector<int> indicesSanatorios;

            Sanatorio** sanatorios = app.getSanatorios();
            int cantSan = app.getCantidadSanatorios();

            for (int i = 0; i < cantSan; ++i)
            {
                if (sanatorios[i] && sanatorios[i]->tieneProfesional(id))
                {
                    sanatoriosConProf.push_back(
                            std::to_string(indicesSanatorios.size() + 1) + ". " +
                            sanatorios[i]->getNombre()
                    );
                    indicesSanatorios.push_back(i);
                }
            }

            if (sanatoriosConProf.empty())
            {
                message_center("Info", "Este profesional no trabaja en ningun sanatorio");
                return;
            }

            list_box("Sanatorios donde trabaja", sanatoriosConProf);

            std::string seleccion = input_box(
                    "Eliminar",
                    "Numeros separados por coma o 'todos':",
                    50
            );

            if (!confirm_box("Confirmar", "Eliminar de los sanatorios seleccionados?"))
            {
                message_center("Eliminar", "Cancelado");
                return;
            }

            if (seleccion == "todos" || seleccion == "TODOS")
            {
                for (int idx : indicesSanatorios)
                {
                    app.eliminarProfesionalDeSanatorio(idx, id);
                }
                message_center("Eliminar", "Profesional eliminado de todos los sanatorios");
            }
            else
            {
                // Parsear selección
                std::vector<int> numerosSeleccionados;
                std::string numero;
                seleccion += ',';

                for (char c : seleccion)
                {
                    if (c == ',' || c == ' ')
                    {
                        if (!numero.empty())
                        {
                            try
                            {
                                int n = to_int(numero) - 1;
                                if (n >= 0 && n < (int)indicesSanatorios.size())
                                {
                                    numerosSeleccionados.push_back(indicesSanatorios[n]);
                                }
                                numero.clear();
                            }
                            catch (...) {}
                        }
                    }
                    else
                    {
                        numero += c;
                    }
                }

                for (int idx : numerosSeleccionados)
                {
                    app.eliminarProfesionalDeSanatorio(idx, id);
                }

                message_center("Eliminar",
                               "Eliminado de " + std::to_string(numerosSeleccionados.size()) + " sanatorio(s)");
            }
        }
        else  // Eliminar de TODOS
        {
            if (!confirm_box("CONFIRMAR", "Eliminar de TODOS los sanatorios Y del sistema?"))
            {
                message_center("Eliminar", "Cancelado");
                return;
            }

            bool ok = app.eliminarProfesionalPorId(id);
            message_center("Eliminar", ok ? "Profesional eliminado completamente" : "Error al eliminar");
        }
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

// ============================================================================
static void ui_agendar_turno_por_profesional(EmpresaSanatorio &app)
{
    // 1️⃣ PEDIR ID DE TURNO
    std::string sid = input_box("Turnos - Por Profesional", "ID Turno (numero unico):", 10);

    // 2️⃣ PEDIR DATOS DEL PACIENTE (por nombre)
    std::string nombrePac = input_box("Turnos - Por Profesional", "Nombre del Paciente:", 40);
    std::string apellidoPac = input_box("Turnos - Por Profesional", "Apellido del Paciente:", 40);

    const Paciente* pac = app.buscarPacientePorNombre(nombrePac, apellidoPac);
    if (!pac)
    {
        message_center("Error", "Paciente no encontrado con ese nombre y apellido");
        return;
    }

    // 3️⃣ LISTAR Y ELEGIR PROFESIONAL
    auto listaProfesionales = app.listarProfesionalesTexto();
    if (listaProfesionales.empty())
    {
        message_center("Error", "No hay profesionales registrados");
        return;
    }

    std::string sIdProf = input_box_with_list(
            "Turnos - Seleccione Profesional",
            listaProfesionales,
            "Ingrese ID del profesional:",
            10
    );

    int idProfesional;
    try {
        idProfesional = to_int(sIdProf);
    } catch (...) {
        message_center("Error", "ID invalido");
        return;
    }

    Profesional* prof = app.buscarProfesionalPorId(idProfesional);
    if (!prof)
    {
        message_center("Error", "ID de profesional invalido o no encontrado");
        return;
    }

    int idEspecialidad = prof->getEspecialidad().getId();

    // 4️⃣ BUSCAR SANATORIOS DONDE TRABAJA EL PROFESIONAL (ordenados por cercanía)
    auto sanatoriosOrdenados = app.buscarSanatoriosPorProfesional(
            idProfesional,
            pac->getLatitud(),
            pac->getLongitud()
    );

    if (sanatoriosOrdenados.empty())
    {
        message_center("Error", "El profesional no trabaja en ningun sanatorio");
        return;
    }

    int indiceSanatorioSeleccionado = sanatoriosOrdenados[0].first;

    // Si hay más de un sanatorio, mostrar opciones
    if (sanatoriosOrdenados.size() > 1)
    {
        std::vector<std::string> listaSanatorios;
        for (const auto& [idx, dist] : sanatoriosOrdenados)
        {
            Sanatorio* san = app.buscarSanatorioPorIndice(idx);
            if (san)
            {
                char distStr[50];
                snprintf(distStr, sizeof(distStr), "%.2f km", dist);
                listaSanatorios.push_back(
                        std::to_string(listaSanatorios.size() + 1) + ". " +
                        san->getNombre() + " - " + std::string(distStr)
                );
            }
        }

        std::string sSan = input_box_with_list(
                "Turnos - Seleccione Sanatorio",
                listaSanatorios,
                "Numero de sanatorio (ordenados por cercania):",
                10
        );

        int numSan = to_int(sSan) - 1;
        if (numSan < 0 || numSan >= (int)sanatoriosOrdenados.size())
        {
            message_center("Error", "Numero de sanatorio invalido");
            return;
        }

        indiceSanatorioSeleccionado = sanatoriosOrdenados[numSan].first;
    }
    else
    {
        // Solo hay un sanatorio, mostrar info
        Sanatorio* san = app.buscarSanatorioPorIndice(indiceSanatorioSeleccionado);
        if (san)
        {
            char distStr[100];
            snprintf(distStr, sizeof(distStr), "%.2f km", sanatoriosOrdenados[0].second);
            message_center("Sanatorio Seleccionado",
                           san->getNombre() + " - " + std::string(distStr));
        }
    }

    // 5️⃣ ✅ MODIFICADO: Obtener turnos disponibles EN ESE SANATORIO ESPECÍFICO
    auto turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorioSeleccionado, 14);

    if (turnosDisponibles.empty())
    {
        // Buscar en las siguientes 2 semanas
        turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorioSeleccionado, 28);

        if (turnosDisponibles.empty())
        {
            message_center("Error", "No hay turnos disponibles en el proximo mes en este sanatorio");
            return;
        }

        message_center("Aviso", "No hay turnos en las proximas 2 semanas. Mostrando semanas 3-4");
    }

    // Mostrar lista de turnos disponibles
    std::vector<std::string> listaTurnos;
    for (const auto& [fecha, hora] : turnosDisponibles)
    {
        listaTurnos.push_back(
                std::to_string(listaTurnos.size() + 1) + ". " +
                fecha + " a las " + hora
        );

        if (listaTurnos.size() >= 50) break;
    }

    std::string sTurno = input_box_with_list(
            "Turnos Disponibles",
            listaTurnos,
            "Seleccione numero de turno:",
            10
    );

    int numTurno = to_int(sTurno) - 1;
    if (numTurno < 0 || numTurno >= (int)turnosDisponibles.size())
    {
        message_center("Error", "Numero de turno invalido");
        return;
    }

    std::string fechaSeleccionada = turnosDisponibles[numTurno].first;
    std::string horaSeleccionada = turnosDisponibles[numTurno].second;

    if (!confirm_box("Confirmar", "Agendar turno para " + fechaSeleccionada + " a las " + horaSeleccionada + "?"))
    {
        message_center("Turnos", "Cancelado");
        return;
    }

    // 6️⃣ RESERVAR EL TURNO
    if (!prof->reservarTurno(fechaSeleccionada, horaSeleccionada))
    {
        message_center("Error", "No se pudo reservar el turno (ya ocupado)");
        return;
    }

    std::string fh = fechaSeleccionada + " " + horaSeleccionada;
    std::string err;

    try
    {
        bool ok = app.agendarTurno(
                to_int(sid), pac->getId(), idProfesional, idEspecialidad,
                fh, prof->getDuracionTurno(), err);

        if (!ok)
        {
            message_center("Turnos", "Error: " + err);
        }
        else
        {
            message_center("Turnos", "Turno agendado correctamente");
        }
    }
    catch (...)
    {
        message_center("Error", "ID de turno invalido");
    }
}
// ============================================================================
static void ui_agendar_turno_por_especialidad(EmpresaSanatorio &app)
{
    // 1️⃣ PEDIR ID DE TURNO
    std::string sid = input_box("Turnos - Por Especialidad", "ID Turno (numero unico):", 10);

    // 2️⃣ PEDIR DATOS DEL PACIENTE
    std::string nombrePac = input_box("Turnos - Por Especialidad", "Nombre del Paciente:", 40);
    std::string apellidoPac = input_box("Turnos - Por Especialidad", "Apellido del Paciente:", 40);

    const Paciente* pac = app.buscarPacientePorNombre(nombrePac, apellidoPac);
    if (!pac)
    {
        message_center("Error", "Paciente no encontrado con ese nombre y apellido");
        return;
    }

    if (pac->getLatitud() == 0.0 && pac->getLongitud() == 0.0)
    {
        message_center("Error", "Paciente sin coordenadas registradas");
        return;
    }

    // 3️⃣ LISTAR Y ELEGIR ESPECIALIDAD
    auto listaEspecialidades = app.listarEspecialidadesTexto();
    if (listaEspecialidades.empty())
    {
        message_center("Error", "No hay especialidades registradas");
        return;
    }

    std::string sIdEsp = input_box_with_list(
            "Turnos - Seleccione Especialidad",
            listaEspecialidades,
            "Ingrese ID de especialidad:",
            10
    );

    int idEspecialidad;
    try {
        idEspecialidad = to_int(sIdEsp);
    } catch (...) {
        message_center("Error", "ID invalido");
        return;
    }

    Especialidad* esp = app.buscarEspecialidadPorId(idEspecialidad);
    if (!esp)
    {
        message_center("Error", "ID de especialidad invalido o no encontrado");
        return;
    }

    // 4️⃣ BUSCAR SANATORIOS CON ESA ESPECIALIDAD
    auto sanatorios = app.buscarSanatoriosPorEspecialidad(
            idEspecialidad,
            pac->getLatitud(),
            pac->getLongitud()
    );

    if (sanatorios.empty())
    {
        message_center("Error", "No hay sanatorios con esa especialidad");
        return;
    }

    // Mostrar sanatorios ordenados por cercanía
    std::vector<std::string> listaSanatorios;
    for (const auto& [idx, dist] : sanatorios)
    {
        Sanatorio* san = app.buscarSanatorioPorIndice(idx);
        if (san)
        {
            char distStr[50];
            snprintf(distStr, sizeof(distStr), "%.2f km", dist);
            listaSanatorios.push_back(
                    std::to_string(listaSanatorios.size() + 1) + ". " +
                    san->getNombre() + " - " + std::string(distStr)
            );
        }
    }

    std::string sSan = input_box_with_list(
            "Sanatorios con esta Especialidad",
            listaSanatorios,
            "Numero de sanatorio (ordenados por cercania):",
            10
    );

    int numSan = to_int(sSan) - 1;
    if (numSan < 0 || numSan >= (int)sanatorios.size())
    {
        message_center("Error", "Numero de sanatorio invalido");
        return;
    }

    int indiceSanatorio = sanatorios[numSan].first;

    // 5️⃣ OBTENER PROFESIONALES DE ESA ESPECIALIDAD EN ESE SANATORIO
    std::vector<int> profesionalesIds = app.obtenerProfesionalesPorEspecialidadEnSanatorio(
            idEspecialidad,
            indiceSanatorio
    );

    if (profesionalesIds.empty())
    {
        message_center("Error", "No hay profesionales de esa especialidad en el sanatorio");
        return;
    }

    // Mostrar profesionales
    std::vector<std::string> listaProfesionales;
    for (int idProf : profesionalesIds)
    {
        const Profesional* prof = app.buscarProfesionalPorId(idProf);
        if (prof)
        {
            listaProfesionales.push_back(
                    "ID: " + std::to_string(prof->getId()) + " | " +
                    prof->getApellido() + ", " + prof->getNombre()
            );
        }
    }

    std::string sIdProf = input_box_with_list(
            "Turnos - Seleccione Profesional",
            listaProfesionales,
            "Ingrese ID del profesional:",
            10
    );

    int idProfesional;
    try {
        idProfesional = to_int(sIdProf);
    } catch (...) {
        message_center("Error", "ID invalido");
        return;
    }

    Profesional* prof = app.buscarProfesionalPorId(idProfesional);
    if (!prof)
    {
        message_center("Error", "ID de profesional invalido o no encontrado");
        return;
    }

    // 6️⃣ ✅ MODIFICADO: Obtener turnos disponibles EN ESE SANATORIO ESPECÍFICO
    auto turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorio, 14);

    if (turnosDisponibles.empty())
    {
        turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorio, 28);

        if (turnosDisponibles.empty())
        {
            message_center("Error", "No hay turnos disponibles en el proximo mes en este sanatorio");
            return;
        }

        message_center("Aviso", "No hay turnos en las proximas 2 semanas. Mostrando semanas 3-4");
    }

    // Mostrar lista de turnos disponibles
    std::vector<std::string> listaTurnos;
    for (const auto& [fecha, hora] : turnosDisponibles)
    {
        listaTurnos.push_back(
                std::to_string(listaTurnos.size() + 1) + ". " +
                fecha + " a las " + hora
        );

        if (listaTurnos.size() >= 50) break;
    }

    std::string sTurno = input_box_with_list(
            "Turnos Disponibles",
            listaTurnos,
            "Seleccione numero de turno:",
            10
    );

    int numTurno = to_int(sTurno) - 1;
    if (numTurno < 0 || numTurno >= (int)turnosDisponibles.size())
    {
        message_center("Error", "Numero de turno invalido");
        return;
    }

    std::string fechaSeleccionada = turnosDisponibles[numTurno].first;
    std::string horaSeleccionada = turnosDisponibles[numTurno].second;

    if (!confirm_box("Confirmar", "Agendar turno para " + fechaSeleccionada + " a las " + horaSeleccionada + "?"))
    {
        message_center("Turnos", "Cancelado");
        return;
    }

    // 7️⃣ RESERVAR EL TURNO
    if (!prof->reservarTurno(fechaSeleccionada, horaSeleccionada))
    {
        message_center("Error", "No se pudo reservar el turno (ya ocupado)");
        return;
    }

    std::string fh = fechaSeleccionada + " " + horaSeleccionada;
    std::string err;

    try
    {
        bool ok = app.agendarTurno(
                to_int(sid), pac->getId(), idProfesional, idEspecialidad,
                fh, prof->getDuracionTurno(), err);

        if (!ok)
        {
            message_center("Turnos", "Error: " + err);
        }
        else
        {
            message_center("Turnos", "Turno agendado correctamente");
        }
    }
    catch (...)
    {
        message_center("Error", "ID de turno invalido");
    }
}
// ============================================================================
// ✅ FUNCIÓN PRINCIPAL DE AGENDAR TURNO (llama a las dos anteriores)
// ============================================================================
static void ui_agendar_turno(EmpresaSanatorio &app)
{
    std::vector<std::string> opciones = {
            "Por Profesional especifico",
            "Por Especialidad (busca sanatorio mas cercano)"
    };

    int metodo = run_submenu("Agendar Turno - Metodo", opciones);

    if (metodo < 0)
    {
        message_center("Turnos", "Cancelado");
        return;
    }

    if (metodo == 0)  // Por profesional
    {
        ui_agendar_turno_por_profesional(app);
    }
    else if (metodo == 1)  // Por especialidad
    {
        ui_agendar_turno_por_especialidad(app);
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

// ============================================================================
// ✅ MODIFICADO: Configurar disponibilidad ahora incluye el sanatorio
// ============================================================================
static void ui_configurar_disponibilidad_profesional(EmpresaSanatorio &app)
{
    if (app.getCantidadSanatorios() == 0)
    {
        message_center("Error", "Primero debe agregar sanatorios");
        return;
    }

    std::string sidProf = input_box("Disponibilidad", "ID Profesional:", 10);

    try
    {
        int idProf = to_int(sidProf);
        Profesional* prof = app.buscarProfesionalPorId(idProf);

        if (!prof)
        {
            message_center("Error", "Profesional inexistente");
            return;
        }

        message_center("Info", "Profesional: " + prof->getNombre() + " " + prof->getApellido());

        // ✅ NUEVO: Listar sanatorios donde trabaja el profesional
        std::vector<std::string> listaSanatorios;
        std::vector<int> indicesSanatorios;

        Sanatorio** sanatorios = app.getSanatorios();
        int cantSan = app.getCantidadSanatorios();

        for (int i = 0; i < cantSan; ++i)
        {
            if (sanatorios[i] && sanatorios[i]->tieneProfesional(idProf))
            {
                listaSanatorios.push_back(
                        std::to_string(listaSanatorios.size() + 1) + ". " +
                        sanatorios[i]->getNombre()
                );
                indicesSanatorios.push_back(i);
            }
        }

        if (listaSanatorios.empty())
        {
            message_center("Error", "El profesional no trabaja en ningun sanatorio aun");
            return;
        }

        // ✅ NUEVO: Elegir sanatorio
        std::string sSan = input_box_with_list(
                "Disponibilidad - Seleccione Sanatorio",
                listaSanatorios,
                "Numero de sanatorio:",
                10
        );

        int numSan = to_int(sSan) - 1;
        if (numSan < 0 || numSan >= (int)indicesSanatorios.size())
        {
            message_center("Error", "Numero de sanatorio invalido");
            return;
        }

        int indiceSanatorio = indicesSanatorios[numSan];

        // ✅ Elegir día
        std::vector<std::string> dias = {
                "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"
        };

        std::string sDia = input_box_with_list(
                "Disponibilidad",
                dias,
                "Seleccione dia (1-7):",
                10
        );

        int numDia = to_int(sDia) - 1;
        if (numDia < 0 || numDia >= 7)
        {
            message_center("Error", "Dia invalido");
            return;
        }

        std::string horaInicio = input_box("Disponibilidad", "Hora inicio (HH:MM):", 10);
        std::string horaFin = input_box("Disponibilidad", "Hora fin (HH:MM):", 10);

        // Parsear horas
        int hI, mI, hF, mF;
        if (sscanf(horaInicio.c_str(), "%d:%d", &hI, &mI) != 2 ||
            sscanf(horaFin.c_str(), "%d:%d", &hF, &mF) != 2)
        {
            message_center("Error", "Formato de hora invalido (use HH:MM)");
            return;
        }

        if (!confirm_box("Confirmar", "Guardar disponibilidad?"))
        {
            message_center("Disponibilidad", "Cancelado");
            return;
        }

        // ✅ MODIFICADO: Ahora incluye el índice del sanatorio
        prof->agregarDisponibilidad(indiceSanatorio, dias[numDia], hI, mI, hF, mF);

        message_center("Disponibilidad",
                       "Horario agregado correctamente en " + sanatorios[indiceSanatorio]->getNombre());
    }
    catch (...)
    {
        message_center("Error", "ID invalido");
    }
}


// ---------------- main ----------------
int main()
{
    setlocale(LC_ALL, ""); // para datos mock
    EmpresaSanatorio app;
    cargarDatosMock(app); // FUNCION PARA PRUEBAS COMENTAR PARA NO USAR
    app.iniciarServicioNotificaciones(); // <-- INICIAMOS RECORDATORIOS
    init_ui();
    std::vector<std::string> principal = {"Sanatorios","Pacientes", "Profesionales", "Especialidades", "Turnos", "Salir"};

    for (;;)
    {
        int i = run_menu_titled("Empresa Sanatorio", principal);
        if (i < 0 || principal[i] == "Salir")
        {
            app.detenerServicioNotificaciones(); // <-- DETENEMOS EL SERVICIO ANTES DE SALIR
            shutdown_ui();
            break;
        }

        if (principal[i] == "Sanatorios")
        {
            for (;;)
            {
                int s = run_submenu("Sanatorios", {
                        "Agregar",
                        "Listar"
                });

                if (s < 0) break;

                if (s == 0)
                    ui_agregar_sanatorio(app);
                else if (s == 1)
                    ui_listar_sanatorios(app);
            }
        }
        else if (principal[i] == "Pacientes")
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
            int s = run_submenu("Profesionales", {"Agregar", "Eliminar", "Listar", "Ordenar A-Z","Configurar Disponibilidad"});
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
            else if (s == 4)
                ui_configurar_disponibilidad_profesional(app);
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
