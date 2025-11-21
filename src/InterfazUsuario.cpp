#include "InterfazUsuario.h"
#include "menu.h"
#include "Paciente.h"
#include "Profesional.h"
#include "Especialidad.h"
#include "Sanatorio.h"

#include <vector>
#include <stdexcept>
#include <string>
#include <cmath>

namespace InterfazUsuario {

    int to_int(const std::string &s) {
        size_t p = 0;
        int v = std::stoi(s, &p);
        if (p != s.size())
            throw std::invalid_argument("nan");
        return v;
    }

    double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
        const double R = 6371.0; // Radio de la Tierra en km
        const double PI = 3.14159265358979323846;

        double dLat = (lat2 - lat1) * PI / 180.0;
        double dLon = (lon2 - lon1) * PI / 180.0;

        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) *
                   sin(dLon / 2) * sin(dLon / 2);

        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return R * c;
    }

// ============================================================================
// SANATORIOS
// ============================================================================

    void ui_agregar_sanatorio(EmpresaSanatorio &app) {
        std::string nombre = input_box("Sanatorios - Agregar", "Nombre:", 50);
        if (nombre.empty()) return;

        // MANTENEMOS TU BUCLE DE VALIDACIÓN
        std::string direccion;
        std::pair<double, double> direcCoordenadas;
        while (true) {
            direccion = input_box("Sanatorios - Agregar", "Direccion:", 50);
            if (direccion.empty()) return;

            direcCoordenadas = app.geocodificarDireccion(direccion);

            if (direcCoordenadas.first != 0.0 || direcCoordenadas.second != 0.0)
                break;
            message_center("Error", "Direccion invalida o no encontrada.\nIntente nuevamente.");
        }

        std::string error;
        Sanatorio *nuevoSanatorio = app.agregarSanatorio(
                nombre, direccion, direcCoordenadas.first, direcCoordenadas.second, error
        );

        if (nuevoSanatorio) {
            std::string msg = "ID Asignado: " + std::to_string(nuevoSanatorio->getId());
            message_center("Sanatorio creado con exito", msg);
        } else {
            message_center("Error", error);
        }
    }

    void ui_listar_sanatorios(EmpresaSanatorio &app) {
        int cant = app.getCantidadSanatorios();
        if (cant == 0) {
            message_center("Sanatorios", "No hay sanatorios registrados");
            return;
        }

        std::vector<std::string> lista;
        Sanatorio **sanatorios = app.getSanatorios();

        for (int i = 0; i < cant; ++i) {
            if (sanatorios[i]) {
                std::string linea =
                        std::to_string(i + 1) + ". " +
                        sanatorios[i]->getNombre() +
                        " | " + sanatorios[i]->getDireccion();
                lista.push_back(linea);
            }
        }

        list_box("Sanatorios", lista);
    }

    void ui_eliminar_sanatorio(EmpresaSanatorio &app) {
        int cant = app.getCantidadSanatorios();
        if (cant == 0) {
            message_center("Sanatorios", "No hay sanatorios registrados");
            return;
        }

        std::vector<std::string> lista;
        Sanatorio **sanatorios = app.getSanatorios();

        for (int i = 0; i < cant; ++i) {
            if (sanatorios[i]) {
                std::string linea =
                        std::to_string(i + 1) + ". " +
                        sanatorios[i]->getNombre() +
                        " | " + sanatorios[i]->getDireccion();
                lista.push_back(linea);
            }
        }

        std::string sNum = input_box_with_list(
                "Sanatorios - Eliminar",
                lista,
                "Numero de sanatorio:",
                10
        );

        try {
            int numero = to_int(sNum) - 1; // Convertir a índice (1 -> 0, 2 -> 1, etc.)

            if (numero < 0 || numero >= cant) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            Sanatorio* san = sanatorios[numero];
            if (!san) {
                message_center("Error", "Sanatorio no encontrado");
                return;
            }

            if (!confirm_box("Confirmar", "¿Eliminar sanatorio '" + san->getNombre() + "'?")) {
                message_center("Eliminar", "Cancelado");
                return;
            }

            std::string error;
            bool ok = app.eliminarSanatorioPorId(san->getId(), error);

            if (ok) {
                message_center("Eliminar", "Sanatorio eliminado exitosamente");
            } else {
                message_center("Error", error.empty() ? "No se pudo eliminar" : error);
            }
        }
        catch (...) {
            message_center("Error", "Numero invalido");
        }
    }

    void ui_listar_especialidades_sanatorio(EmpresaSanatorio &app) {
        int cant = app.getCantidadSanatorios();
        if (cant == 0) {
            message_center("Sanatorios", "No hay sanatorios registrados");
            return;
        }

        std::vector<std::string> lista;
        Sanatorio **sanatorios = app.getSanatorios();

        for (int i = 0; i < cant; ++i) {
            if (sanatorios[i]) {
                std::string linea =
                        std::to_string(i + 1) + ". " +
                        sanatorios[i]->getNombre() +
                        " | " + sanatorios[i]->getDireccion();
                lista.push_back(linea);
            }
        }

        std::string sNum = input_box_with_list(
                "Sanatorios - Especialidades",
                lista,
                "Numero de sanatorio:",
                10
        );

        try {
            int numero = to_int(sNum) - 1; // Convertir a índice

            if (numero < 0 || numero >= cant) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            Sanatorio* san = sanatorios[numero];
            if (!san) {
                message_center("Error", "Sanatorio no encontrado");
                return;
            }

            auto listaEsp = app.listarEspecialidadesDeSanatorio(san->getId());

            if (listaEsp.empty()) {
                message_center("Especialidades", "El sanatorio '" + san->getNombre() + "' no tiene especialidades registradas");
            } else {
                list_box("Especialidades de " + san->getNombre(), listaEsp);
            }
        }
        catch (...) {
            message_center("Error", "Numero invalido");
        }
    }

    void ui_listar_profesionales_sanatorio(EmpresaSanatorio &app) {
        int cant = app.getCantidadSanatorios();
        if (cant == 0) {
            message_center("Sanatorios", "No hay sanatorios registrados");
            return;
        }

        std::vector<std::string> lista;
        Sanatorio **sanatorios = app.getSanatorios();

        for (int i = 0; i < cant; ++i) {
            if (sanatorios[i]) {
                std::string linea =
                        std::to_string(i + 1) + ". " +
                        sanatorios[i]->getNombre() +
                        " | " + sanatorios[i]->getDireccion();
                lista.push_back(linea);
            }
        }

        std::string sNum = input_box_with_list(
                "Sanatorios - Profesionales",
                lista,
                "Numero de sanatorio:",
                10
        );

        try {
            int numero = to_int(sNum) - 1; // Convertir a índice

            if (numero < 0 || numero >= cant) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            Sanatorio* san = sanatorios[numero];
            if (!san) {
                message_center("Error", "Sanatorio no encontrado");
                return;
            }

            auto listaProf = app.listarProfesionalesDeSanatorio(san->getId());

            if (listaProf.empty()) {
                message_center("Profesionales", "El sanatorio '" + san->getNombre() + "' no tiene profesionales registrados");
            } else {
                list_box("Profesionales de " + san->getNombre(), listaProf);
            }
        }
        catch (...) {
            message_center("Error", "Numero invalido");
        }
    }

    void ui_listar_pacientes_sanatorio(EmpresaSanatorio &app) {
        int cant = app.getCantidadSanatorios();
        if (cant == 0) {
            message_center("Sanatorios", "No hay sanatorios registrados");
            return;
        }

        std::vector<std::string> lista;
        Sanatorio **sanatorios = app.getSanatorios();

        for (int i = 0; i < cant; ++i) {
            if (sanatorios[i]) {
                std::string linea =
                        std::to_string(i + 1) + ". " +
                        sanatorios[i]->getNombre() +
                        " | " + sanatorios[i]->getDireccion();
                lista.push_back(linea);
            }
        }

        std::string sNum = input_box_with_list(
                "Sanatorios - Pacientes",
                lista,
                "Numero de sanatorio:",
                10
        );

        try {
            int numero = to_int(sNum) - 1; // Convertir a índice

            if (numero < 0 || numero >= cant) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            Sanatorio* san = sanatorios[numero];
            if (!san) {
                message_center("Error", "Sanatorio no encontrado");
                return;
            }

            auto listaPac = app.listarPacientesDeSanatorio(san->getId());

            if (listaPac.empty()) {
                message_center("Pacientes", "El sanatorio '" + san->getNombre() + "' no tiene pacientes con turnos activos");
            } else {
                list_box("Pacientes de " + san->getNombre(), listaPac);
            }
        }
        catch (...) {
            message_center("Error", "Numero invalido");
        }
    }
// ============================================================================
// PACIENTES
// ============================================================================

    void ui_agregar_paciente(EmpresaSanatorio &app) {
        // 1. Quitamos la petición del ID (lógica autoincremental)
        // std::string s_id = input_box("...", "ID (numerico):", 10); // <-- SE FUE

        // 2. Pedimos los datos
        std::string nombre = input_box("Pacientes - Agregar", "Nombre:", 50);
        if (nombre.empty()) return;
        std::string apellido = input_box("Pacientes - Agregar", "Apellido:", 50);
        if (apellido.empty()) return;
        std::string mail = input_box("Pacientes - Agregar", "Mail:", 50);

        // 3. MANTENEMOS TU BUCLE DE VALIDACIÓN (¡Perfecto!)
        std::string direccion;
        std::pair<double, double> direcCoordenadas;
        while (true) {
            direccion = input_box("Pacientes - Agregar", "Direccion:", 50); // (subí el maxlen)
            if (direccion.empty()) return; // Permitir cancelar

            direcCoordenadas = app.geocodificarDireccion(direccion);

            if (direcCoordenadas.first != 0.0 || direcCoordenadas.second != 0.0)
                break;
            message_center("Error", "Direccion invalida o no encontrada.\nIntente nuevamente.");
        }

        // 4. Pedimos el resto de datos
        std::string s_nroAfiliado = input_box("Pacientes - Agregar", "Nro Afiliado:", 15);
        std::string obraSocial = input_box("Pacientes - Agregar", "Obra Social:", 30);

        try {
            int nroAfiliado = to_int(s_nroAfiliado);

            std::string error;
            Paciente *nuevoPaciente = app.agregarPaciente(
                    nombre, apellido, mail, direccion, nroAfiliado, obraSocial,
                    direcCoordenadas.first, direcCoordenadas.second, error
            );


            if (nuevoPaciente) {
                std::string msg = "ID Asignado: " + std::to_string(nuevoPaciente->getId());
                message_center("Paciente agregado con exito", msg);
            } else {
                message_center("Error", error);
            }
        }
        catch (const std::exception &e) {
            message_center("Error", "Datos invalidos");
        }
    }

    void ui_eliminar_paciente(EmpresaSanatorio &app) {
        std::string sid = input_box("Pacientes - Eliminar", "ID:", 10);
        if (!confirm_box("Confirmar", "Eliminar?")) {
            message_center("Eliminar", "Cancelado");
            return;
        }
        try {
            int id = to_int(sid);
            message_center("Eliminar", app.eliminarPacientePorId(id) ? "Paciente eliminado" : "ID inexistente");
        }
        catch (...) {
            message_center("Error", "ID invalido");
        }
    }

    void ui_editar_paciente(EmpresaSanatorio &app) {
        std::string sid = input_box("Pacientes - Editar", "ID:", 10);
        try {
            int id = to_int(sid);
            Paciente *p = app.buscarPacientePorId(id);
            if (!p) {
                message_center("Editar", "ID inexistente");
                return;
            }

            std::string nombre = input_box("Editar Paciente", "Nombre (" + p->getNombre() + "):", 40);
            std::string apellido = input_box("Editar Paciente", "Apellido (" + p->getApellido() + "):", 40);
            std::string snaf = input_box("Editar Paciente",
                                         "Nro Afiliado (" + std::to_string(p->getNumeroDeAfiliado()) + "):", 10);
            std::string obra = input_box("Editar Paciente", "Obra social (" + p->getObraSocial() + "):", 40);
            std::string mail = input_box("Editar Paciente", "Mail (" + p->getMail() + "):", 40);

            std::string direccion;
            std::pair<double, double> direcCoordenadas;

            while (true) {
                direccion = input_box("Editar Paciente", "Direccion (" + p->getDireccion() + "):", 50);
                if (direccion.empty())break;
                direcCoordenadas = app.geocodificarDireccion(direccion);

                if (direcCoordenadas.first != 0.0 || direcCoordenadas.second != 0.0)
                    break;
                message_center("Error", "Direccion invalida. Intente nuevamente.");
            }

            if (!confirm_box("Confirmar", "Guardar cambios?")) {
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
        catch (...) {
            message_center("Error", "Campo numerico invalido");
        }
    }

// ============================================================================
// ESPECIALIDADES
// ============================================================================

    void ui_agregar_especialidad(EmpresaSanatorio &app) {
        // NO PEDIMOS ID
        std::string nombre = input_box("Especialidades - Agregar", "Nombre:", 50);
        if (nombre.empty()) return;

        std::string error;
        Especialidad *esp = app.agregarEspecialidad(nombre, error); // Nueva llamada

        if (esp) {
            message_center("Especialidad agregada ", "ID: " + std::to_string(esp->getId()));
        } else {
            message_center("Error", error.empty() ? "Error desconocido" : error);
        }
    }

    void ui_eliminar_especialidad(EmpresaSanatorio &app) {
        std::string sid = input_box("Especialidades - Eliminar", "ID:", 10);

        try {
            int id = to_int(sid);
            Especialidad *esp = app.buscarEspecialidadPorId(id);

            if (!esp) {
                message_center("Error", "Especialidad inexistente");
                return;
            }

            std::vector<std::string> opciones = {
                    "Eliminar de sanatorios especificos",
                    "Eliminar de TODOS los sanatorios"
            };

            int eleccion = run_submenu("Eliminar Especialidad", opciones);

            if (eleccion < 0) {
                message_center("Eliminar", "Cancelado");
                return;
            }

            if (eleccion == 0) {
                std::vector<std::string> sanatoriosConEsp;
                std::vector<int> indicesSanatorios;

                Sanatorio **sanatorios = app.getSanatorios();
                int cantSan = app.getCantidadSanatorios();

                for (int i = 0; i < cantSan; ++i) {
                    if (sanatorios[i] && sanatorios[i]->tieneEspecialidad(id)) {
                        sanatoriosConEsp.push_back(
                                std::to_string(indicesSanatorios.size() + 1) + ". " +
                                sanatorios[i]->getNombre()
                        );
                        indicesSanatorios.push_back(i);
                    }
                }

                if (sanatoriosConEsp.empty()) {
                    message_center("Info", "Esta especialidad no esta en ningun sanatorio");
                    return;
                }

                list_box("Sanatorios con esta especialidad", sanatoriosConEsp);

                std::string seleccion = input_box(
                        "Eliminar",
                        "Numeros separados por coma (ej: 1,3,5) o 'todos':",
                        50
                );

                if (!confirm_box("Confirmar", "Eliminar de los sanatorios seleccionados?")) {
                    message_center("Eliminar", "Cancelado");
                    return;
                }

                if (seleccion == "todos" || seleccion == "TODOS") {
                    for (int idx: indicesSanatorios) {
                        app.eliminarEspecialidadDeSanatorio(idx, id);
                    }
                    message_center("Eliminar", "Especialidad eliminada de todos los sanatorios");
                } else {
                    std::vector<int> numerosSeleccionados;
                    std::string numero;
                    seleccion += ',';

                    for (char c: seleccion) {
                        if (c == ',' || c == ' ') {
                            if (!numero.empty()) {
                                try {
                                    int n = to_int(numero) - 1;
                                    if (n >= 0 && n < (int) indicesSanatorios.size()) {
                                        numerosSeleccionados.push_back(indicesSanatorios[n]);
                                    }
                                    numero.clear();
                                }
                                catch (...) {}
                            }
                        } else {
                            numero += c;
                        }
                    }

                    for (int idx: numerosSeleccionados) {
                        app.eliminarEspecialidadDeSanatorio(idx, id);
                    }

                    message_center("Eliminar",
                                   "Eliminada de " + std::to_string(numerosSeleccionados.size()) + " sanatorio(s)");
                }
            } else {
                if (!confirm_box("CONFIRMAR", "Eliminar de TODOS los sanatorios Y del sistema?")) {
                    message_center("Eliminar", "Cancelado");
                    return;
                }

                bool ok = app.eliminarEspecialidadPorId(id);
                message_center("Eliminar", ok ? "Especialidad eliminada completamente" : "Error al eliminar");
            }
        }
        catch (...) {
            message_center("Error", "ID invalido");
        }
    }

    void ui_listar_especialidades(EmpresaSanatorio &app) {
        auto v = app.listarEspecialidadesTexto();
        v.empty() ? message_center("Especialidades", "No hay registros") : list_box("Especialidades", v);
    }

// ============================================================================
// PROFESIONALES
// ============================================================================

    void ui_agregar_profesional(EmpresaSanatorio &app) {
        // NO PEDIMOS EL ID DEL SISTEMA
        // std::string s_id = input_box("...", "ID (numerico):", 10); // <-- SE FUE

        // SÍ PEDIMOS LA MATRÍCULA (es un dato, no un ID interno)
        std::string s_nro = input_box("Profesionales - Agregar", "Nro Matricula:", 10);
        std::string nombre = input_box("Profesionales - Agregar", "Nombre:", 50);
        if (nombre.empty()) return;
        std::string apellido = input_box("Profesionales - Agregar", "Apellido:", 50);
        if (apellido.empty()) return;
        std::string mail = input_box("Profesionales - Agregar", "Mail:", 50);

        // (Tu lógica para elegir especialidad)
        std::string s_esp_id = input_box("Profesionales - Agregar", "ID Especialidad:", 10);

        try {
            int nroMatricula = to_int(s_nro);
            Especialidad *esp = app.buscarEspecialidadPorId(to_int(s_esp_id));

            if (!esp) {
                message_center("Error", "Especialidad no encontrada");
                return;
            }

            std::string error;
            Profesional *nuevoProf = app.agregarProfesional(
                    nroMatricula, *esp, nombre, apellido, mail, error
            );

            if (nuevoProf) {
                std::string msg = "ID Asignado: " + std::to_string(nuevoProf->getId());
                message_center("Profesional agregado", msg);
            } else {
                message_center("Error", error);
            }
        }
        catch (const std::exception &e) {
            message_center("Error", "Datos invalidos");
        }
    }

    void ui_eliminar_profesional(EmpresaSanatorio &app) {
        std::string sid = input_box("Profesionales - Eliminar", "ID:", 10);

        try {
            int id = to_int(sid);
            Profesional *prof = app.buscarProfesionalPorId(id);

            if (!prof) {
                message_center("Error", "Profesional inexistente");
                return;
            }

            std::vector<std::string> opciones = {
                    "Eliminar de sanatorios especificos",
                    "Eliminar de TODOS los sanatorios"
            };

            int eleccion = run_submenu("Eliminar Profesional", opciones);

            if (eleccion < 0) {
                message_center("Eliminar", "Cancelado");
                return;
            }

            if (eleccion == 0) {
                std::vector<std::string> sanatoriosConProf;
                std::vector<int> indicesSanatorios;

                Sanatorio **sanatorios = app.getSanatorios();
                int cantSan = app.getCantidadSanatorios();

                for (int i = 0; i < cantSan; ++i) {
                    if (sanatorios[i] && sanatorios[i]->tieneProfesional(id)) {
                        sanatoriosConProf.push_back(
                                std::to_string(indicesSanatorios.size() + 1) + ". " +
                                sanatorios[i]->getNombre()
                        );
                        indicesSanatorios.push_back(i);
                    }
                }

                if (sanatoriosConProf.empty()) {
                    message_center("Info", "Este profesional no trabaja en ningun sanatorio");
                    return;
                }

                list_box("Sanatorios donde trabaja", sanatoriosConProf);

                std::string seleccion = input_box(
                        "Eliminar",
                        "Numeros separados por coma o 'todos':",
                        50
                );

                if (!confirm_box("Confirmar", "Eliminar de los sanatorios seleccionados?")) {
                    message_center("Eliminar", "Cancelado");
                    return;
                }

                if (seleccion == "todos" || seleccion == "TODOS") {
                    for (int idx: indicesSanatorios) {
                        app.eliminarProfesionalDeSanatorio(idx, id);
                    }
                    message_center("Eliminar", "Profesional eliminado de todos los sanatorios");
                } else {
                    std::vector<int> numerosSeleccionados;
                    std::string numero;
                    seleccion += ',';

                    for (char c: seleccion) {
                        if (c == ',' || c == ' ') {
                            if (!numero.empty()) {
                                try {
                                    int n = to_int(numero) - 1;
                                    if (n >= 0 && n < (int) indicesSanatorios.size()) {
                                        numerosSeleccionados.push_back(indicesSanatorios[n]);
                                    }
                                    numero.clear();
                                }
                                catch (...) {}
                            }
                        } else {
                            numero += c;
                        }
                    }

                    for (int idx: numerosSeleccionados) {
                        app.eliminarProfesionalDeSanatorio(idx, id);
                    }

                    message_center("Eliminar",
                                   "Eliminado de " + std::to_string(numerosSeleccionados.size()) + " sanatorio(s)");
                }
            } else {
                if (!confirm_box("CONFIRMAR", "Eliminar de TODOS los sanatorios Y del sistema?")) {
                    message_center("Eliminar", "Cancelado");
                    return;
                }

                bool ok = app.eliminarProfesionalPorId(id);
                message_center("Eliminar", ok ? "Profesional eliminado completamente" : "Error al eliminar");
            }
        }
        catch (...) {
            message_center("Error", "ID invalido");
        }
    }

    void ui_listar_profesionales(EmpresaSanatorio &app) {
        auto v = app.listarProfesionalesTexto();
        v.empty() ? message_center("Profesionales", "No hay registros") : list_box("Profesionales", v);
    }

    void ui_configurar_disponibilidad_profesional(EmpresaSanatorio &app) {
        if (app.getCantidadSanatorios() == 0) {
            message_center("Error", "Primero debe agregar sanatorios");
            return;
        }

        std::string sidProf = input_box("Disponibilidad", "ID Profesional:", 10);

        try {
            int idProf = to_int(sidProf);
            Profesional *prof = app.buscarProfesionalPorId(idProf);

            if (!prof) {
                message_center("Error", "Profesional inexistente");
                return;
            }

            message_center("Info", "Profesional: " + prof->getNombre() + " " + prof->getApellido());

            std::vector<std::string> listaSanatorios;
            std::vector<int> indicesSanatorios;

            Sanatorio **sanatorios = app.getSanatorios();
            int cantSan = app.getCantidadSanatorios();

            for (int i = 0; i < cantSan; ++i) {
                if (sanatorios[i] && sanatorios[i]->tieneProfesional(idProf)) {
                    listaSanatorios.push_back(
                            std::to_string(listaSanatorios.size() + 1) + ". " +
                            sanatorios[i]->getNombre()
                    );
                    indicesSanatorios.push_back(i);
                }
            }

            if (listaSanatorios.empty()) {
                message_center("Error", "El profesional no trabaja en ningun sanatorio aun");
                return;
            }

            std::string sSan = input_box_with_list(
                    "Disponibilidad - Seleccione Sanatorio",
                    listaSanatorios,
                    "Numero de sanatorio:",
                    10
            );

            int numSan = to_int(sSan) - 1;
            if (numSan < 0 || numSan >= (int) indicesSanatorios.size()) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            int indiceSanatorio = indicesSanatorios[numSan];

            std::vector<std::string> dias = {
                    "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"
            };

            std::string sDia = input_box_with_list(
                    "Disponibilidad",
                    dias,
                    "Seleccione dia (1-7):",
                    10
            );

            int numDia = to_int(sDia) - 1;
            if (numDia < 0 || numDia >= 7) {
                message_center("Error", "Dia invalido");
                return;
            }

            std::string horaInicio = input_box("Disponibilidad", "Hora inicio (HH:MM):", 10);
            std::string horaFin = input_box("Disponibilidad", "Hora fin (HH:MM):", 10);

            int hI, mI, hF, mF;
            if (sscanf(horaInicio.c_str(), "%d:%d", &hI, &mI) != 2 ||
                sscanf(horaFin.c_str(), "%d:%d", &hF, &mF) != 2) {
                message_center("Error", "Formato de hora invalido (use HH:MM)");
                return;
            }

            if (!confirm_box("Confirmar", "Guardar disponibilidad?")) {
                message_center("Disponibilidad", "Cancelado");
                return;
            }

            prof->agregarDisponibilidad(indiceSanatorio, dias[numDia], hI, mI, hF, mF);

            message_center("Disponibilidad",
                           "Horario agregado correctamente en " + sanatorios[indiceSanatorio]->getNombre());
        }
        catch (...) {
            message_center("Error", "ID invalido");
        }
    }

    void ui_agendar_turno_por_profesional(EmpresaSanatorio &app) {
        // std::string sid = input_box("Turnos - Por Profesional", "ID Turno (numero unico):", 10);
        std::string s_pac_id = input_box("Turnos - Agendar", "ID Paciente:", 10);
        std::string nombrePac = input_box("Turnos - Por Profesional", "Nombre del Paciente:", 40);
        std::string apellidoPac = input_box("Turnos - Por Profesional", "Apellido del Paciente:", 40);

        const Paciente *pac = app.buscarPacientePorNombre(nombrePac, apellidoPac);
        if (!pac) {
            message_center("Error", "Paciente no encontrado con ese nombre y apellido");
            return;
        }

        auto listaProfesionales = app.listarProfesionalesTexto();
        if (listaProfesionales.empty()) {
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

        Profesional *prof = app.buscarProfesionalPorId(idProfesional);
        if (!prof) {
            message_center("Error", "ID de profesional invalido o no encontrado");
            return;
        }

        int idEspecialidad = prof->getEspecialidad().getId();

        auto sanatoriosOrdenados = app.buscarSanatoriosPorProfesional(
                idProfesional,
                pac->getLatitud(),
                pac->getLongitud()
        );

        if (sanatoriosOrdenados.empty()) {
            message_center("Error", "El profesional no trabaja en ningun sanatorio");
            return;
        }

        int indiceSanatorioSeleccionado = sanatoriosOrdenados[0].first;

        if (sanatoriosOrdenados.size() > 1) {
            std::vector<std::string> listaSanatorios;
            for (const auto &[idx, dist]: sanatoriosOrdenados) {
                Sanatorio *san = app.buscarSanatorioPorIndice(idx);
                if (san) {
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
            if (numSan < 0 || numSan >= (int) sanatoriosOrdenados.size()) {
                message_center("Error", "Numero de sanatorio invalido");
                return;
            }

            indiceSanatorioSeleccionado = sanatoriosOrdenados[numSan].first;
        } else {
            Sanatorio *san = app.buscarSanatorioPorIndice(indiceSanatorioSeleccionado);
            if (san) {
                char distStr[100];
                snprintf(distStr, sizeof(distStr), "%.2f km", sanatoriosOrdenados[0].second);
                message_center("Sanatorio Seleccionado",
                               san->getNombre() + " - " + std::string(distStr));
            }
        }

        auto turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorioSeleccionado, 14);

        if (turnosDisponibles.empty()) {
            turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorioSeleccionado, 28);

            if (turnosDisponibles.empty()) {
                message_center("Error", "No hay turnos disponibles en el proximo mes en este sanatorio");
                return;
            }

            message_center("Aviso", "No hay turnos en las proximas 2 semanas. Mostrando semanas 3-4");
        }

        std::vector<std::string> listaTurnos;
        for (const auto &[fecha, hora]: turnosDisponibles) {
            listaTurnos.push_back(
                    std::to_string(listaTurnos.size() + 1) + ". " + fecha + " a las " + hora
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
        if (numTurno < 0 || numTurno >= (int) turnosDisponibles.size()) {
            message_center("Error", "Numero de turno invalido");
            return;
        }

        std::string fechaSeleccionada = turnosDisponibles[numTurno].first;
        std::string horaSeleccionada = turnosDisponibles[numTurno].second;

        if (!confirm_box("Confirmar", "Agendar turno para " + fechaSeleccionada + " a las " + horaSeleccionada + "?")) {
            message_center("Turnos", "Cancelado");
            return;
        }

        // Esta validación está duplicada, la función app.agendarTurno ya la hace (y mejor)
        // if (!prof->reservarTurno(fechaSeleccionada, horaSeleccionada))
        // {
        //     message_center("Error", "No se pudo reservar el turno (ya ocupado)");
        //     return;
        // }

        std::string fh = fechaSeleccionada + " " + horaSeleccionada;
        std::string error;

        try {

            int nuevoId = app.agendarTurno(pac->getId(), idProfesional, indiceSanatorioSeleccionado,
                                           idEspecialidad, fh, 30, error);


            if (nuevoId > 0) // Si el ID es válido
            {
                std::string msg = "ID Asignado: " + std::to_string(nuevoId);
                message_center("Turno agendado con exito", msg);
            } else {
                message_center("Error", error.empty() ? "No se pudo agendar" : error);
            }
        }
        catch (...) {
            message_center("Error", "ID de turno invalido");
        }
    }

    void ui_agendar_turno_por_especialidad(EmpresaSanatorio &app) {
        // std::string sid = input_box("Turnos - Por Especialidad", "ID Turno (numero unico):", 10); // <-- LÍNEA ELIMINADA

        std::string nombrePac = input_box("Turnos - Por Especialidad", "Nombre del Paciente:", 40);
        std::string apellidoPac = input_box("Turnos - Por Especialidad", "Apellido del Paciente:", 40);

        const Paciente *pac = app.buscarPacientePorNombre(nombrePac, apellidoPac);
        if (!pac) {
            message_center("Error", "Paciente no encontrado con ese nombre y apellido");
            return;
        }

        if (pac->getLatitud() == 0.0 && pac->getLongitud() == 0.0) {
            message_center("Error", "Paciente sin coordenadas registradas");
            return;
        }

        auto listaEspecialidades = app.listarEspecialidadesTexto();
        if (listaEspecialidades.empty()) {
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

        Especialidad *esp = app.buscarEspecialidadPorId(idEspecialidad);
        if (!esp) {
            message_center("Error", "ID de especialidad invalido o no encontrado");
            return;
        }

        auto sanatorios = app.buscarSanatoriosPorEspecialidad(
                idEspecialidad,
                pac->getLatitud(),
                pac->getLongitud()
        );

        if (sanatorios.empty()) {
            message_center("Error", "No hay sanatorios con esa especialidad");
            return;
        }

        std::vector<std::string> listaSanatorios;
        for (const auto &[idx, dist]: sanatorios) {
            Sanatorio *san = app.buscarSanatorioPorIndice(idx);
            if (san) {
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
        if (numSan < 0 || numSan >= (int) sanatorios.size()) {
            message_center("Error", "Numero de sanatorio invalido");
            return;
        }

        int indiceSanatorio = sanatorios[numSan].first;

        std::vector<int> profesionalesIds = app.obtenerProfesionalesPorEspecialidadEnSanatorio(
                idEspecialidad,
                indiceSanatorio
        );

        if (profesionalesIds.empty()) {
            message_center("Error", "No hay profesionales de esa especialidad en el sanatorio");
            return;
        }

        std::vector<std::string> listaProfesionales;
        for (int idProf: profesionalesIds) {
            const Profesional *prof = app.buscarProfesionalPorId(idProf);
            if (prof) {
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

        Profesional *prof = app.buscarProfesionalPorId(idProfesional);
        if (!prof) {
            message_center("Error", "ID de profesional invalido o no encontrado");
            return;
        }

        auto turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorio, 14);

        if (turnosDisponibles.empty()) {
            turnosDisponibles = prof->obtenerTurnosDisponibles(indiceSanatorio, 28);

            if (turnosDisponibles.empty()) {
                message_center("Error", "No hay turnos disponibles en el proximo mes en este sanatorio");
                return;
            }

            message_center("Aviso", "No hay turnos en las proximas 2 semanas. Mostrando semanas 3-4");
        }

        std::vector<std::string> listaTurnos;
        for (const auto &[fecha, hora]: turnosDisponibles) {
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
        if (numTurno < 0 || numTurno >= (int) turnosDisponibles.size()) {
            message_center("Error", "Numero de turno invalido");
            return;
        }

        std::string fechaSeleccionada = turnosDisponibles[numTurno].first;
        std::string horaSeleccionada = turnosDisponibles[numTurno].second;

        if (!confirm_box("Confirmar", "Agendar turno para " + fechaSeleccionada + " a las " + horaSeleccionada + "?")) {
            message_center("Turnos", "Cancelado");
            return;
        }

        // Esta validación está duplicada, la función app.agendarTurno ya la hace
        // if (!prof->reservarTurno(fechaSeleccionada, horaSeleccionada))
        // {
        //     message_center("Error", "No se pudo reservar el turno (ya ocupado)");
        //     return;
        // }

        std::string fh = fechaSeleccionada + " " + horaSeleccionada;
        std::string err;

        try {
            // ANTES: bool ok = app.agendarTurno(to_int(sid), pac->getId(), ...
            // AHORA: Llamamos a la nueva función (que devuelve 'int') y sin el ID de turno.

            int nuevoId = app.agendarTurno(
                    pac->getId(), idProfesional, indiceSanatorio, idEspecialidad,
                    fh, prof->getDuracionTurno(), err);

            if (nuevoId > 0) // Si el ID es válido (ej. > 0), fue un éxito
            {
                std::string msg = "ID Asignado: " + std::to_string(nuevoId);
                message_center("Turno agendado con exito", msg);
            } else // Si es 0 o -1, falló
            {
                message_center("Error", err.empty() ? "No se pudo agendar" : err);
            }
        }
        catch (...) {
            // El 'catch' anterior era para 'to_int(sid)'.
            // Lo cambiamos a un error genérico.
            message_center("Error", "Ocurrio un error inesperado al agendar");
        }
    }

    void ui_agendar_turno(EmpresaSanatorio &app) {
        std::vector<std::string> opciones = {
                "Por Profesional especifico",
                "Por Especialidad (busca sanatorio mas cercano)"
        };

        int metodo = run_submenu("Agendar Turno - Metodo", opciones);

        if (metodo < 0) {
            message_center("Turnos", "Cancelado");
            return;
        }

        if (metodo == 0) {
            ui_agendar_turno_por_profesional(app);
        } else if (metodo == 1) {
            ui_agendar_turno_por_especialidad(app);
        }
    }

    // ... (includes previos)

    void ui_consultar_especialidad(EmpresaSanatorio &app)
    {
        // 1. LIMPIEZA DE BUFFER (La clave del problema)
        // Esto borra cualquier "Enter" que haya quedado del menú principal
        std::cin.sync();

        // Si cin.sync() no funciona en tu compilador, descomenta la siguiente linea:
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // 2. CONFIRMACIÓN VISUAL
        system("cls"); // Limpiar pantalla para que se vea limpio
        std::cout << "=== BIENVENIDO AL ASISTENTE DE IA ===" << std::endl;
        std::cout << "(Si ves esto, el menu funciono)" << std::endl;
        std::cout << "-------------------------------------" << std::endl;

        // 3. Pedir Síntomas
        // Nota: input_box internamente usa getline. Si el buffer está sucio, falla.
        std::string sintomas = input_box("Asistente IA", "Describa sus sintomas (ej. dolor de pecho):", 50);

        // Verificamos qué leyó realmente
        if (sintomas.empty()) {
            std::cout << "DEBUG: Se leyeron sintomas vacios. Presione Enter para volver..." << std::endl;
            system("pause");
            return;
        }

        message_center("Asistente IA", "Analizando... (Conectando con cerebro Python)");

        std::string recomendacion;
        bool exito = app.predecirEspecialidad(sintomas, recomendacion);

        if (exito) {
            std::string msg = "Basado en sus sintomas, se sugiere:\n\n >> " + recomendacion +
                              " <<\n\n¿Desea volver al menu?";

            // Mostramos el resultado
            message_center("Resultado IA", msg);
        } else {
            message_center("Error", recomendacion);
        }
    }

    void ui_cancelar_turno(EmpresaSanatorio &app)
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

        void ui_listar_turnos(EmpresaSanatorio &app)
        {
            auto v = app.listarTurnosTexto();
            v.empty() ? message_center("Turnos", "No hay registros") : list_box("Turnos", v);
        }

}