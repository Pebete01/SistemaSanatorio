#include "iostream"
#include "string"

using namespace std;

class Especialidad{
private:
    int idEspecialidad;
    string nombre;
public:
    Especialidad(int id, string nombre) : idEspecialidad(id), nombre(nombre) {}

    void mostrar();
    ~Especialidad() {}
};
