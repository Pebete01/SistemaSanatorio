#include "iostream"
#include "string"
#include "Especialidad.h"
using namespace std;

void Especialidad::mostrar()
{
    cout << "Especialidad: " << nombre << " id: " << id << endl;
}

int Especialidad::getNumueroEspecialidad() {
    return id;
}
