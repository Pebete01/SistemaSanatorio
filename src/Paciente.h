#pragma once
#include <string>
#include <iosfwd> // forward decl de std::ostream
#include "Persona.h"

class Paciente : public Persona
{
private:
    std::string obraSocial;
    int numeroDeAfiliado;
    bool estadoPaciente;
    static int cantidad;

public:
    Paciente(int id, const std::string &nombre, const std::string &apellido,
             int numeroAfiliado, const std::string &obraSocial)
        : Persona(id, nombre, apellido),
          obraSocial(obraSocial),
          numeroDeAfiliado(numeroAfiliado),
          estadoPaciente(true) { ++cantidad; }

    void modificarEstado();
    void instanciasVivas();
    bool getEstado() const { return estadoPaciente; }
    void mostrar() override;
    int getNumeroDeAfiliado() const { return numeroDeAfiliado; }
    const std::string &getObraSocial() const { return obraSocial; }

    friend std::ostream &operator<<(std::ostream &os, const Paciente &p);
    bool operator<(const Paciente &o) const;

    ~Paciente() { --cantidad; }
};
