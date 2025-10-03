#pragma once
#include <string>
#include "Persona.h"
#include "Especialidad.h"

class Profesional : public Persona
{
private:
    bool estado;
    int numeroDeProfesional;
    Especialidad especialidad;
    static int numProfesionales;

public:
    Profesional(int numero,
                const Especialidad &esp,
                int id,
                const std::string &nombre,
                const std::string &apellido);

    ~Profesional() override;

    void modificarEstado();
    void mostrar() override;

    int getNumeroProfesional() const;
    const Especialidad &getEspecialidad() const;
    bool getEstado() const { return estado; }

    static void instanciasVivas();
};
