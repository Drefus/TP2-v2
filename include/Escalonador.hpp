#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP
#include "Paciente.hpp"

// Estrutura que representa um evento
struct Evento
{
    double dataHora;     // Data e hora do evento
    Paciente *paciente;  // Ponteiro para o paciente associado ao evento
    int numUnidade = -1; // Número da unidade associada ao evento (valor padrão -1)

    // Construtor da estrutura Evento
    Evento(double dataHora = 0, Paciente *paciente = nullptr, int numUnidade = -1)
        : dataHora(dataHora), paciente(paciente), numUnidade(numUnidade) {}
};

// Classe que representa um escalonador de eventos
class Escalonador
{
private:
    Evento *heap;   // Ponteiro para o array que representa o heap de eventos
    int capacidade; // Capacidade máxima do heap
    int tamanho;    // Tamanho atual do heap

    // Função para ajustar o heap para cima a partir de um índice
    void heapifyParaCima(int index);

    // Função para ajustar o heap para baixo a partir de um índice
    void heapifyParaBaixo(int index);

public:
    // Construtor padrão
    Escalonador();

    // Construtor que inicializa o escalonador com uma capacidade específica
    Escalonador(int capacidade);

    // Destrutor
    ~Escalonador();

    // Função para inserir um novo evento no escalonador
    void inserirEvento(Evento &evento);

    // Função para retirar o próximo evento do escalonador
    Evento retirarProximoEvento();

    // Função para verificar se o escalonador está vazio
    bool vazio();
};

#endif