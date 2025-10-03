#pragma once
#include <string>

class Especialidad
{
private:
    int id;
    std::string nombre;
    bool estado{true};

public:
    explicit Especialidad(int id, const std::string &nombre);

    int getId() const noexcept;
    const std::string &getNombre() const noexcept;
    int getNumeroEspecialidad() const { return getId(); }
    bool getEstado() const noexcept;

    void toggleEstado() noexcept;
    void mostrar() const;
};
