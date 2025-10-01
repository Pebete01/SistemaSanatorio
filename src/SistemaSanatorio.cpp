#include "iostream"
#include "string"
#include "Profesional.h"
#include "Paciente.h"
#include "Especialidad.h"
#include "Turno.h"
#include "Sanatorio.h"
#include "SistemaSanatorio.h"
#include "algorithm"
#include "limits"
using namespace std;

void EmpresaSanatorio::agrendarListaPaciente() {
    int capacidadNuevaPacientes = (capacidadPacientes == 0) ? 4 : capacidadPacientes * 2;
    Paciente **listaNueva = new Paciente *[capacidadNuevaPacientes];

    for (int i = 0; i < cantidadPacientes; ++i)
    {
        listaNueva[i] = listaPacientes[i];
    }
    delete[] listaPacientes;
    listaPacientes = listaNueva;
    capacidadPacientes = capacidadNuevaPacientes;
}

void EmpresaSanatorio::agregarPaciente(Paciente *p) {
    if (capacidadPacientes == cantidadPacientes) agrendarListaPaciente();
    listaPacientes[cantidadPacientes++] = p;
}
void EmpresaSanatorio::agrandarListaEspecialidad() {
    int capacidadNueva = (capacidadEspecialidad == 0) ? 4 : capacidadEspecialidad * 2;
    Especialidad **listaNueva = new Especialidad * [capacidadNueva];

    for (int i = 0; i < capacidadEspecialidad; ++i)
    {
        listaNueva[i] = especialidades[i];
    }
    delete[] especialidades;
    especialidades = listaNueva;
    capacidadEspecialidad = capacidadNueva;
}

void EmpresaSanatorio::agregarEspecialidad(Especialidad *p) {
    if(capacidadEspecialidad == cantidadEspecialdiades) agrandarListaEspecialidad();
    especialidades[cantidadEspecialdiades++] = p;
}

void EmpresaSanatorio::agrandarListaProfesionales() {
    int capacidadNuevaProfesionales = (capacidadProfesionales == 0) ? 4 : capacidadProfesionales * 2;
    Profesional **listaNueva = new Profesional * [capacidadNuevaProfesionales];

    for (int i = 0; i < cantidadProfesionales; ++i)
    {
        listaNueva[i] = profesionales[i];
    }
    delete[] profesionales;
    profesionales = listaNueva;
    capacidadProfesionales = capacidadNuevaProfesionales;
}

void EmpresaSanatorio::agregarProfesional(Profesional *p) {
    if(cantidadProfesionales == capacidadProfesionales) agrandarListaProfesionales();
    profesionales[cantidadProfesionales++] = p;
}

void EmpresaSanatorio::agrandarListaTurnos() {
    int capacidadNuevaTurnos = (capacidadTurno == 0) ? 4 : capacidadTurno * 2;
    Turno **listaNueva = new Turno * [capacidadNuevaTurnos];

    for (int i = 0; i < cantidadTurnos; ++i)
    {
        listaNueva[i] = turnos[i];
    }
    delete[] turnos;
    turnos = listaNueva;
    capacidadTurno = capacidadNuevaTurnos;
}

void EmpresaSanatorio::agregarTurnos(Turno *p) {
    if(capacidadTurno == cantidadTurnos) agrandarListaTurnos();
    turnos[cantidadTurnos++] = p;
}
int EmpresaSanatorio::validarEntero(const string &mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        } else {
            cout << "Entrada inválida. Ingrese un número válido."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
string EmpresaSanatorio::validarTexto(const string &mensaje) {
    string valor;
    while (true) {
        cout << mensaje;
        getline(cin, valor);
        if (!valor.empty()) {
            return valor;
        }
        cout << "El texto no puede estar vacío."<<endl;
    }
}
Especialidad * EmpresaSanatorio::nuevaEspecialidad() {
    int id = validarEntero("Ingrese ID especialidad: ");
    string nombre = validarTexto("Ingrese nombre de Especialidad: ");
    return new Especialidad(id,nombre);
}
Paciente * EmpresaSanatorio::nuevoPaciente() {
    int id = validarEntero("Ingreses ID del paciente: ");
    int numAfiliado = validarEntero("Ingrese numero afililiado: ");
    string nombre = validarTexto("Ingrese nombre: ");
    string apellido = validarTexto("Ingrese apellido: ");
    string obraSocial = validarTexto("Ingrese obra Social: ");
    return  new Paciente(id,nombre,apellido,numAfiliado,obraSocial);;
}
Profesional * EmpresaSanatorio::nuevoProfesional() {
    int id = validarEntero("Ingrese ID del profesional: ");
    int numProfesional = validarEntero("Ingrese numero de profesional: ");
    //ver tema de especialidad como lo ingreso (Fijate de hacerlo con id de especialidad)
    string nombre = validarTexto("Ingrese nombre: ");
    string apellido = validarTexto("Ingrese apellido: ");

    //return new Profesional(numProfesional,,id,nombre,apellido);
}

Sanatorio * EmpresaSanatorio::nuevoSanatorio() {
    string nombre = validarTexto("Ingrese nombre del sanatorio: ");
    string ubicaion = validarTexto("ingrese ubicacion del sanatorio: ");
    return new Sanatorio(nombre,ubicaion);
}
void EmpresaSanatorio::subMenuEliminar() {
    string opcion;
    bool funcionando  = true;
    while (funcionando){
        cout << "\nOPCIONES PARA ELIMINAR\n";
        cout << "especialidades | pacientes | sanatorios | profesionales | volver"<<endl;
        cout << "Escriba una opcion: ";
        getline(cin,opcion);
        transform(opcion.begin(),opcion.end(),opcion.begin(), ::tolower);
        if(opcion == "pacientes"){

        } else if(opcion == "especialidades"){

        } else if(opcion == "sanatorios"){

        }else if(opcion == "profesionales"){

        }else if(opcion == "volver"){
            funcionando = false;
        }else{
            cout<<"Opcion no valida, por favor escriba una opcion"<<endl;
        }
    }
}

void EmpresaSanatorio::subMenuAgregar() {
    string opcion;
    bool funcionando  = true;
    while (funcionando){
        cout << "\nOPCIONES PARA AGREGAR\n";
        cout << "especialidades | pacientes | sanatorios | profesionales | volver"<<endl;
        cout << "Escriba una opcion: ";
        getline(cin,opcion);
        transform(opcion.begin(),opcion.end(),opcion.begin(), ::tolower);
        if(opcion == "pacientes"){
            agregarPaciente(nuevoPaciente());
        } else if(opcion == "especialidades"){
            agregarEspecialidad(nuevaEspecialidad());
        } else if(opcion == "sanatorios"){
            //una vez agregada funciones de sanatorio completar
        }else if(opcion == "profesionales"){
            agregarProfesional(nuevoProfesional());
        }else if(opcion == "volver"){
            funcionando = false;
        }else{
            cout<<"Opcion no valida, por favor escriba una opcion"<<endl;
        }
    }
}
void EmpresaSanatorio::subMenuMostrar() {
    string opcion;
    bool funcionando  = true;
    while (funcionando){
        cout << "\nOPCIONES PARA MOSTRAR\n";
        cout << "especialidades | pacientes | sanatorios | profesionales | volver"<<endl;
        cout << "Escriba una opcion: ";
        getline(cin,opcion);
        transform(opcion.begin(),opcion.end(),opcion.begin(), ::tolower);

        if(opcion == "pacientes"){
            for (int i = 0; i < cantidadPacientes; i++) {
                listaPacientes[i]->mostrar();
            }
        } else if(opcion == "especialidades"){
            for (int i = 0; i < cantidadEspecialdiades; i++) {
                especialidades[i]->mostrar();
            }
        } else if(opcion == "sanatorios"){
            for (int i = 0; i < cantidadSanatorios; ++i) {
                sanatorios[i]->mostrarSanatorio();
            }
        }else if(opcion == "profesionales"){
            for (int i = 0; i < cantidadProfesionales; ++i) {
                profesionales[i]->mostrar();
            }
        }else if(opcion == "volver"){
            funcionando = false;
        }else{
            cout<<"Opcion no valida, por favor escriba una opcion"<<endl;
        }
    }
}

void EmpresaSanatorio::menu() {
    bool funcionando = true;
    string opcion;

    while (funcionando){
        cout << "\n===== MENU =====\n";
        cout << "Opciones: agregar | eliminar | mostrar | salir"<<endl;
        cout << "Escriba una opcion: ";
        getline(cin, opcion);
        transform(opcion.begin(),opcion.end(),opcion.begin(), ::tolower);

        if(opcion == "agregar"){
            subMenuAgregar();
        } else if(opcion == "eliminar"){
            subMenuEliminar();
        }else if(opcion == "mostrar"){
            subMenuMostrar();
        }else if(opcion == "salir") {
            funcionando = false;
        } else{
            cout<<"Opcion no valida, por favor escriba una opcion"<<endl;
        }
    }
}