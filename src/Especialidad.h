#include "iostream"
#include "string"

using namespace std;

class Especialidad
{
private:
    int id;
    string nombre;

public:
    Especialidad(int id, string nombre) : id(id), nombre(nombre) {}
    int getNumueroEspecialidad();
    void mostrar();
    ~Especialidad() {}
};
