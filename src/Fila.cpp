#include "Fila.hpp"
#include <stdexcept>

Fila::Fila() : frente(nullptr), tras(nullptr) {}

Fila::Fila(bool igorarPrioridade) : frente(nullptr), tras(nullptr), igorarPrioridade(igorarPrioridade)
{
}

Fila::~Fila()
{
    finaliza();
}

void Fila::inicializa()
{
    frente = nullptr;
    tras = nullptr;
}

void Fila::enfileira(Paciente *paciente)
{
    Nodo *novoNodo = new Nodo{paciente, nullptr};
    if (tras == nullptr)
    {
        frente = novoNodo;
        tras = novoNodo;
    }
    else if (igorarPrioridade)
    {
        tras->proximo = novoNodo;
        tras = novoNodo;
    }
    else
    {
        Nodo *atual = frente;
        Nodo *anterior = nullptr;
        while (atual != nullptr && atual->paciente->getPrioridade() >= paciente->getPrioridade())
        {
            anterior = atual;
            atual = atual->proximo;
        }
        if (anterior == nullptr)
        {
            novoNodo->proximo = frente;
            frente = novoNodo;
        }
        else
        {
            novoNodo->proximo = atual;
            anterior->proximo = novoNodo;
            if (atual == nullptr)
            {
                tras = novoNodo;
            }
        }
    }
}

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
        tras = nullptr;
    }
    Paciente *paciente = nodoRemovido->paciente;
    delete nodoRemovido;
    return paciente;
}

bool Fila::filaVazia()
{
    return frente == nullptr;
}

void Fila::finaliza()
{
    while (!filaVazia())
    {
        desenfileira();
    }
}