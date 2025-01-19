#include "Fila.hpp"
#include <stdexcept>

// Construtor da classe Fila
Fila::Fila() : frente(nullptr), tras(nullptr) {}

// Destrutor da classe Fila
Fila::~Fila()
{
    finaliza();
}

// Inicializa a fila, definindo os ponteiros frente e tras como nulos
void Fila::inicializa()
{
    frente = nullptr;
    tras = nullptr;
}

// Adiciona um paciente na fila
void Fila::enfileira(Paciente *paciente)
{
    if (paciente == nullptr)
    {
        throw std::invalid_argument("Paciente não pode ser nulo");
    }
    Nodo *novoNodo = new Nodo{paciente, nullptr};
    if (tras == nullptr)
    {
        // Se a fila estiver vazia, o novo nodo será tanto o frente quanto o tras
        frente = novoNodo;
        tras = novoNodo;
    }
    else
    {
        // Caso contrário, adiciona o novo nodo no final da fila
        tras->proximo = novoNodo;
        tras = novoNodo;
    }
}

// Remove e retorna o paciente do início da fila
Paciente *Fila::desenfileira()
{
    if (filaVazia())
    {
        throw std::runtime_error("Fila vazia");
    }
    Nodo *nodoRemovido = frente;
    frente = frente->proximo;
    if (frente == nullptr)
    {
        // Se a fila ficar vazia após a remoção, atualiza o ponteiro tras para nulo
        tras = nullptr;
    }
    Paciente *paciente = nodoRemovido->paciente;
    delete nodoRemovido;
    return paciente;
}

// Verifica se a fila está vazia
bool Fila::filaVazia()
{
    return frente == nullptr;
}

// Libera todos os elementos da fila
void Fila::finaliza()
{
    while (!filaVazia())
    {
        delete desenfileira();
    }
}