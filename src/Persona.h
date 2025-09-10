#include "iostream"
#include "string"

using namespace std;

class Persona{
protected:
    int id;
    string nombre;
    string apellido;
public:
    Persona(int id, string nombre, string apellido) : id(id), nombre(nombre), apellido(apellido){}
    virtual void modificarEstado() =0;
    virtual void mostrar()=0;

    ~Persona(){}
};