#ifndef FILA_HPP
#define FILA_HPP

#include "Paciente.hpp"

// Estrutura que representa um nodo da fila
struct Nodo
{
    Paciente *paciente; // Ponteiro para o paciente
    Nodo *proximo;      // Ponteiro para o próximo nodo na fila
};

// Classe que representa uma fila de pacientes
class Fila
{
private:
    Nodo *frente; // Ponteiro para o primeiro nodo da fila
    Nodo *tras;   // Ponteiro para o último nodo da fila

public:
    Fila();  // Construtor da fila
    ~Fila(); // Destrutor da fila

    void inicializa();                  // Inicializa a fila
    void enfileira(Paciente *paciente); // Adiciona um paciente à fila
    Paciente *desenfileira();           // Remove e retorna o paciente da frente da fila
    bool filaVazia();                   // Verifica se a fila está vazia
    void finaliza();                    // Finaliza a fila, liberando a memória
};

#endif