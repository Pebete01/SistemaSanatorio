//
// Created by tomyl on 10/9/2025.
//
#include "iostream"
#include "string"
#include "Sanatorio.h"
#include "list"
using namespace std;

void Sanatorio::agrandarListaPaciente()
{
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

void Sanatorio::agregarPaciente(Paciente *p)
{
    if (capacidadPacientes == cantidadPacientes) agrandarListaPaciente();
    listaPacientes[cantidadPacientes++] = p;
}

void Sanatorio::agrandarListaProfesionales() {
    int capacidadNuevaProfesionales = (capacidadProfe == 0) ? 4 : capacidadProfe * 2;
    Profesional **listaNueva = new Profesional * [capacidadNuevaProfesionales];

    for (int i = 0; i < cantidadPacientes; ++i)
    {
        listaNueva[i] = listaProfesionales[i];
    }
    delete[] listaProfesionales;
    listaProfesionales = listaNueva;
    capacidadProfe = capacidadNuevaProfesionales;
}

void Sanatorio::agregarProfesional(Profesional * p) {
    if(cantidadProfe == capacidadProfe) agrandarListaProfesionales();
    listaProfesionales[cantidadProfe++] = p;
}

void Sanatorio::agrandarListaTurnos(){
    int capacidadNuevaTurnos = (capacidadTurno == 0) ? 4 : capacidadTurno * 2;
    Turno **listaNueva = new Turno * [capacidadNuevaTurnos];

    for (int i = 0; i < cantidadTurno; ++i)
    {
        listaNueva[i] = listaTurno[i];
    }
    delete[] listaTurno;
    listaTurno = listaNueva;
    capacidadTurno = capacidadNuevaTurnos;
}
void Sanatorio::agregarTurnos(Turno *p) {
    if(capacidadTurno == cantidadTurno) agrandarListaTurnos();
    listaTurno[cantidadTurno++] = p;
}
void Sanatorio::agrandarListaEspecialidad() {
    int capacidadNueva = (capacidadEspecialidad == 0) ? 4 : capacidadEspecialidad * 2;
    Especialidad **listaNueva = new Especialidad * [capacidadNueva];

    for (int i = 0; i < cantidadEspecialidad; ++i)
    {
        listaNueva[i] = listaEspecialidades[i];
    }
    delete[] listaEspecialidades;
    listaEspecialidades = listaNueva;
    capacidadEspecialidad = capacidadNueva;
}
void Sanatorio::agregarEspecialidad(Especialidad *p) {
    if(capacidadEspecialidad == cantidadEspecialidad) agrandarListaEspecialidad();
    listaEspecialidades[cantidadEspecialidad++] = p;
}

void Sanatorio::sacarProfesional() {
    int codigo;
    bool validacion = false;
    bool noEncontrado = true;

    while(!validacion){
        cout<<"Ingrese el numero de profesional"<<endl;
        cin >> codigo;

        if(cin.fail()){
            cout << "Entrada inválida. Debe ingresar un número entero." << endl;
            cin.clear();
        }
        else {
            validacion = true;
        }
    }

    for (int i = 0; i < cantidadProfe; i++) {

        if(listaProfesionales[i]->getNumueroProfesional() == codigo && listaProfesionales[i] != nullptr){
            noEncontrado = false;
            delete listaProfesionales[i];
            listaProfesionales[i] = nullptr;

            for (int j = i; j < cantidadProfe - 1; j++) {
                listaProfesionales[j] = listaProfesionales[j + 1];
            }
            listaProfesionales[cantidadProfe - 1] = nullptr;
            cantidadProfe--;
        }


    }



    if(noEncontrado){
        cout << "Profesional no encontrado en sanatorio "<<Sanatorio::nombre << endl;
    } else{
        cout << "Profesional eliminado en "<<Sanatorio::nombre << endl;
    }

}

void Sanatorio::sacarEspecialidad() {
    int codigo;
    bool validacion = false;
    bool noEncontrado = true;

    while(!validacion){
        cout<<"Ingrese el id de especialidad"<<endl;
        cin >> codigo;

        if(cin.fail()){
            cout << "Entrada inválida. Debe ingresar un número entero." << endl;
            cin.clear();
        }
        else {
            validacion = true;
        }
    }

    for (int i = 0; i < cantidadEspecialidad; i++) {

        if(listaEspecialidades[i]->getNumueroEspecialidad() == codigo && listaEspecialidades[i] != nullptr){
            noEncontrado = false;
            delete listaEspecialidades[i];
            listaEspecialidades[i] = nullptr;

            for (int j = i; j < cantidadEspecialidad - 1; j++) {
                listaEspecialidades[j] = listaEspecialidades[j + 1];
            }
            listaEspecialidades[cantidadEspecialidad - 1] = nullptr;
            cantidadEspecialidad--;
        }
    }

    if(noEncontrado){
        cout << "Especialidad no encontrada en sanatorio "<<Sanatorio::nombre << endl;
    } else{
        cout << "Especialidad eliminada en "<<Sanatorio::nombre << endl;
    }

}

void Sanatorio::mostrarSanatorio() {
    cout<<"Sanatorio "<<nombre<<endl;
    cout<<"\nEspecialidades atendidas "<<endl;
    for (int i = 0; i < cantidadEspecialidad; i++) {
        listaEspecialidades[i]->mostrar();
    }
    cout<<"\nPacientes anotados "<<endl;
    for (int i = 0; i < cantidadPacientes; i++) {
        listaPacientes[i]->mostrar();
    }
    cout<<"\nProfesionales en el Sanatorio "<<endl;
    for (int i = 0; i < cantidadProfe; i++) {
        listaProfesionales[i]->mostrar();
    }
    cout<<"\nTurnos agendados"<<endl;
    for (int i = 0; i < cantidadTurno; i++) {
        listaTurno[i]->mostrar();
    }
}


