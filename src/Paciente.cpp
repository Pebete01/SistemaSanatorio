//
// Created by tomyl on 10/9/2025.
//

#include "Paciente.h"
#include "iostream"
#include "string"
using namespace std;

void Paciente::modificarEstado() {
    if(!estadoPaciente){
        estadoPaciente = true;
    } else{
        estadoPaciente = false;
    }
}

void Paciente::mostrar() {
    cout<<"ID: "<<id<<" Sr/Sra: "<<nombre<<" "<<apellido<<" obra social: "<<obraSocial<<" plan: "<<numeroDeAfiliado<<endl;

}