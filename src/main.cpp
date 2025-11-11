#include "SistemaSanatorio.h"
#include "InterfazUsuario.h"
#include "menu.h"
#include "DatosMock.h"

#include <vector>
#include <string>

using namespace InterfazUsuario;

int main()
{
    setlocale(LC_ALL, "");

    EmpresaSanatorio app;
    cargarDatosMock(app);
    app.iniciarServicioNotificaciones();

    init_ui();

    std::vector<std::string> principal = {
            "Sanatorios",
            "Pacientes",
            "Profesionales",
            "Especialidades",
            "Turnos",
            "Salir"
    };

    for (;;)
    {
        int i = run_menu_titled("Empresa Sanatorio", principal);

        if (i < 0 || principal[i] == "Salir")
        {
            app.detenerServicioNotificaciones();
            shutdown_ui();
            break;
        }

        if (principal[i] == "Sanatorios")
        {
            for (;;)
            {
                int s = run_submenu("Sanatorios", {
                        "Agregar",
                        "Eliminar",
                        "Listar",
                        "Listar Especialidades",
                        "Listar Profesionales",
                        "Listar Pacientes"
                });

                if (s < 0) break;

                if (s == 0)
                    ui_agregar_sanatorio(app);
                else if (s == 1)
                    ui_eliminar_sanatorio(app);
                else if (s == 2)
                    ui_listar_sanatorios(app);
                else if (s == 3)
                    ui_listar_especialidades_sanatorio(app);
                else if (s == 4)
                    ui_listar_profesionales_sanatorio(app);
                else if (s == 5)
                    ui_listar_pacientes_sanatorio(app);
            }
        }
        else if (principal[i] == "Pacientes")
        {
            for (;;)
            {
                int s = run_submenu("Pacientes", {
                        "Agregar",
                        "Eliminar",
                        "Listar",
                        "Editar"
                });

                if (s < 0) break;

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
                    ui_editar_paciente(app);
            }
        }
        else if (principal[i] == "Profesionales")
        {
            int s = run_submenu("Profesionales", {
                    "Agregar",
                    "Eliminar",
                    "Listar",
                    "Configurar Disponibilidad"
            });

            if (s == 0)
                ui_agregar_profesional(app);
            else if (s == 1)
                ui_eliminar_profesional(app);
            else if (s == 2)
                ui_listar_profesionales(app);
            else if (s == 3)
                ui_configurar_disponibilidad_profesional(app);
        }
        else if (principal[i] == "Especialidades")
        {
            int s = run_submenu("Especialidades", {
                    "Agregar",
                    "Eliminar",
                    "Listar"
            });

            if (s == 0)
                ui_agregar_especialidad(app);
            else if (s == 1)
                ui_eliminar_especialidad(app);
            else if (s == 2)
                ui_listar_especialidades(app);
        }
        else if (principal[i] == "Turnos")
        {
            int s = run_submenu("Turnos", {
                    "Agendar",
                    "Cancelar",
                    "Listar"
            });

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