#include "Escalonador.hpp"
#include <stdexcept>

// Construtor padrão
Escalonador::Escalonador() : heap(nullptr), capacidade(0), tamanho(0)
{
    capacidade = 10;
    heap = new Evento[capacidade];
}

// Construtor com capacidade especificada
Escalonador::Escalonador(int capacidade) : heap(nullptr), capacidade(capacidade), tamanho(0)
{
    heap = new Evento[capacidade];
}

// Destrutor
Escalonador::~Escalonador()
{
    delete[] heap;
}

// Insere um novo evento na heap
void Escalonador::inserirEvento(Evento &evento)
{
    if (evento.dataHora < 0)
    {
        throw std::invalid_argument("Data e hora do evento não podem ser negativas");
    }
    if (evento.paciente == nullptr)
    {
        throw std::invalid_argument("Paciente do evento não pode ser nulo");
    }
    // Verifica se a capacidade foi atingida e dobra a capacidade se necessário
    if (tamanho == capacidade)
    {
        capacidade *= 2;
        Evento *novoHeap = new Evento[capacidade];
        for (int i = 0; i < tamanho; i++)
        {
            novoHeap[i] = heap[i];
        }
        delete[] heap;
        heap = novoHeap;
    }
    // Adiciona o novo evento e ajusta a heap para cima
    heap[tamanho] = evento;
    heapifyParaCima(tamanho);
    tamanho++;
}

// Retira o próximo evento da heap
Evento Escalonador::retirarProximoEvento()
{
    // Verifica se a heap está vazia
    if (tamanho == 0)
    {
        throw std::runtime_error("Nenhum evento na fila");
    }
    // Remove o evento do topo e ajusta a heap para baixo
    Evento proximoEvento = heap[0];
    heap[0] = heap[tamanho - 1];
    tamanho--;
    heapifyParaBaixo(0);
    return proximoEvento;
}

// Verifica se a heap está vazia
bool Escalonador::vazio()
{
    return tamanho == 0;
}

// Ajusta a heap para cima
void Escalonador::heapifyParaCima(int indice)
{
    while (indice > 0)
    {
        int indicePai = (indice - 1) / 2;
        // Compara o evento atual com o pai e troca se necessário
        if (heap[indicePai].dataHora > heap[indice].dataHora ||
            (heap[indicePai].dataHora == heap[indice].dataHora &&
             heap[indicePai].paciente->getId() > heap[indice].paciente->getId()))
        {
            Evento temp = heap[indice];
            heap[indice] = heap[indicePai];
            heap[indicePai] = temp;
            indice = indicePai;
        }
        else
        {
            break;
        }
    }
}

// Ajusta a heap para baixo
void Escalonador::heapifyParaBaixo(int indice)
{
    int filhoEsquerdo, filhoDireito, menorIndice;
    Evento temp;

    while (indice * 2 + 1 < tamanho)
    {
        filhoEsquerdo = indice * 2 + 1;
        filhoDireito = indice * 2 + 2;
        menorIndice = indice;

        // Compara o evento atual com os filhos e troca se necessário
        if (filhoEsquerdo < tamanho && (heap[filhoEsquerdo].dataHora < heap[menorIndice].dataHora ||
                                        (heap[filhoEsquerdo].dataHora == heap[menorIndice].dataHora &&
                                         heap[filhoEsquerdo].paciente->getId() < heap[menorIndice].paciente->getId())))
        {
            menorIndice = filhoEsquerdo;
        }

        if (filhoDireito < tamanho && (heap[filhoDireito].dataHora < heap[menorIndice].dataHora ||
                                       (heap[filhoDireito].dataHora == heap[menorIndice].dataHora &&
                                        heap[filhoDireito].paciente->getId() < heap[menorIndice].paciente->getId())))
        {
            menorIndice = filhoDireito;
        }

        if (menorIndice != indice)
        {
            temp = heap[indice];
            heap[indice] = heap[menorIndice];
            heap[menorIndice] = temp;
            indice = menorIndice;
        }
        else
        {
            break;
        }
    }
}