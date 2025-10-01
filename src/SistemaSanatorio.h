#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"
#include "Turno.h"
#include "Sanatorio.h"
using namespace std;

class EmpresaSanatorio{
private:
    Paciente **listaPacientes;
    Profesional **profesionales;
    Turno **turnos;
    Sanatorio **sanatorios;
    Especialidad **especialidades;
    int cantidadPacientes,cantidadEspecialdiades, cantidadSanatorios, cantidadTurnos, cantidadProfesionales;
    int capacidadPacientes,capacidadProfesionales, capacidadTurno, capacidadEspecialidad, capacidadSanatorios;

public:
    EmpresaSanatorio() : listaPacientes(NULL), profesionales(NULL), turnos(NULL), sanatorios(NULL){}

    void agregarPaciente(Paciente *p);
    void agrendarListaPaciente();
    void agrandarListaProfesionales();
    void agregarProfesional(Profesional * p);
    void agrandarListaTurnos();
    void agregarTurnos(Turno *p);
    void agrandarListaEspecialidad();
    void agregarEspecialidad(Especialidad *p);
    //faltan funciones para sanatorio (tomy)

    int validarEntero(const string &mensaje);
    string validarTexto(const string &mensaje);

    Paciente * nuevoPaciente();
    Especialidad * nuevaEspecialidad();
    Profesional * nuevoProfesional();
    Sanatorio *  nuevoSanatorio();

    void menu();
    void subMenuMostrar();
    void subMenuAgregar();
    void subMenuEliminar();




    ~EmpresaSanatorio(){
        delete[] listaPacientes, profesionales, turnos, sanatorios, especialidades;
    }
};