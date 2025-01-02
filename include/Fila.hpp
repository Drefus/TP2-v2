#ifndef FILA_HPP
#define FILA_HPP

#include "Paciente.hpp"

struct Nodo
{
    Paciente paciente;
    Nodo *proximo;
};

class Fila
{
private:
    Nodo *frente;
    Nodo *tras;

public:
    Fila();
    ~Fila();

    void inicializa();
    void enfileira(Paciente &paciente);
    Paciente desenfileira();
    bool filaVazia();
    void finaliza();
};

#endif