#pragma once

#include "SistemaSanatorio.h"

// MÓDULO DE INTERFAZ DE USUARIO
// Contiene todas las funciones de interacción con el usuario para el sistema


namespace InterfazUsuario {


    int to_int(const std::string &s);

    double calcularDistancia(double lat1, double lon1, double lat2, double lon2);

    // ==================== SANATORIOS ====================

    void ui_agregar_sanatorio(EmpresaSanatorio &app);
    void ui_listar_sanatorios(EmpresaSanatorio &app);

    // ==================== PACIENTES ====================

    void ui_agregar_paciente(EmpresaSanatorio &app);
    void ui_eliminar_paciente(EmpresaSanatorio &app);
    void ui_editar_paciente(EmpresaSanatorio &app);

    // ==================== ESPECIALIDADES ====================

    void ui_agregar_especialidad(EmpresaSanatorio &app);
    void ui_eliminar_especialidad(EmpresaSanatorio &app);
    void ui_listar_especialidades(EmpresaSanatorio &app);

    // ==================== PROFESIONALES ====================

    void ui_agregar_profesional(EmpresaSanatorio &app);
    void ui_eliminar_profesional(EmpresaSanatorio &app);
    void ui_listar_profesionales(EmpresaSanatorio &app);
    void ui_configurar_disponibilidad_profesional(EmpresaSanatorio &app);

    // ==================== TURNOS ====================

    void ui_agendar_turno(EmpresaSanatorio &app);
    void ui_agendar_turno_por_profesional(EmpresaSanatorio &app);
    void ui_agendar_turno_por_especialidad(EmpresaSanatorio &app);
    void ui_cancelar_turno(EmpresaSanatorio &app);
    void ui_listar_turnos(EmpresaSanatorio &app);

    void ui_eliminar_sanatorio(EmpresaSanatorio &app);
    void ui_listar_especialidades_sanatorio(EmpresaSanatorio &app);
    void ui_listar_profesionales_sanatorio(EmpresaSanatorio &app);
    void ui_listar_pacientes_sanatorio(EmpresaSanatorio &app);

} // namespace InterfazUsuario