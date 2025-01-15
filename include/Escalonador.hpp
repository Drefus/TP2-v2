#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP
#include "Paciente.hpp"
struct Evento
{
    double dataHora;
    Paciente *paciente;
    int numUnit = -1;
    Evento(double dataHora = 0, Paciente *paciente = nullptr, int numUnit = -1) : dataHora(dataHora), paciente(paciente), numUnit(numUnit) {}
};
class Escalonador
{
private:
    Evento *heap;
    int capacidade;
    int tamanho;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void expandHeap();

public:
    Escalonador();
    ~Escalonador();

    void inicializa();
    void insereEvento(Evento &evento);
    Evento retiraProximoEvento();
    void finaliza();
    bool vazio();
};

#endif