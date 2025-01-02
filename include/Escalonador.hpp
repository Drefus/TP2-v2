#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

struct Evento
{
    double dataHora;
    int pacienteId;
    int numUnit = -1;
    Evento(double dh = 0.0, int pid = 0, int te = 0) : dataHora(dh), pacienteId(pid), numUnit(te) {}
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