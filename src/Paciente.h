#pragma once
#include <string>
#include <iosfwd> // forward decl de std::ostream
#include "Persona.h"
#include "GeocodificadorAPI.h"

class Paciente : public Persona
{
private:
    std::string obraSocial;
    int numeroDeAfiliado;
    bool estadoPaciente;
    static int cantidad;
    std::string direccion;
    double lat;
    double lon;


public:
    Paciente(int id, const std::string &nombre, const std::string &apellido, const std::string& mail,const std::string &direccion,
             int numeroAfiliado, const std::string &obraSocial, double lat =0.0, double lon=0.0)
            : Persona(id, nombre, apellido, mail), // <-- Pasar mail al constructor base
              obraSocial(obraSocial),
              numeroDeAfiliado(numeroAfiliado),
              estadoPaciente(true), direccion(direccion),lat(lat), lon(lon){ ++cantidad; }

    void modificarEstado();
    void instanciasVivas();
    bool getEstado() const { return estadoPaciente; }
    void mostrar() override;
    int getNumeroDeAfiliado() const { return numeroDeAfiliado; }
    const std::string &getObraSocial() const { return obraSocial; }
    const std::string &getDireccion() const { return direccion;}
    double getLatitud() const { return lat; }
    double getLongitud() const { return lon; }

    friend std::ostream &operator<<(std::ostream &os, const Paciente &p);
    bool operator<(const Paciente &o) const;

    ~Paciente() { --cantidad; }
};
