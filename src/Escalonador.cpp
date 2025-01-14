#include "Escalonador.hpp"
#include <stdexcept>

Escalonador::Escalonador() : heap(nullptr), capacidade(0), tamanho(0)
{
    inicializa();
}

Escalonador::~Escalonador()
{
    finaliza();
}

void Escalonador::inicializa()
{
    capacidade = 10;
    tamanho = 0;
    heap = new Evento[capacidade];
}

void Escalonador::insereEvento(Evento &evento)
{
    if (tamanho == capacidade)
    {
        expandHeap();
    }
    heap[tamanho] = evento;
    heapifyUp(tamanho);
    tamanho++;
}

Evento Escalonador::retiraProximoEvento()
{
    if (tamanho == 0)
    {
        throw std::runtime_error("Nenhum evento na fila");
    }
    Evento proximoEvento = heap[0];
    heap[0] = heap[tamanho - 1];
    tamanho--;
    heapifyDown(0);
    return proximoEvento;
}

void Escalonador::finaliza()
{
    delete[] heap;
    heap = nullptr;
    capacidade = 0;
    tamanho = 0;
}

bool Escalonador::vazio()
{
    return tamanho == 0;
}

void Escalonador::heapifyUp(int index)
{
    while (index > 0)
    {
        int parentIndex = (index - 1) / 2;
        if (heap[index].dataHora > heap[parentIndex].dataHora ||
            (heap[index].dataHora == heap[parentIndex].dataHora && heap[index].pacienteId >= heap[parentIndex].pacienteId))
        {
            break;
        }
        std::swap(heap[index], heap[parentIndex]);
        index = parentIndex;
    }
}

void Escalonador::heapifyDown(int index)
{
    while (true)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < tamanho && (heap[leftChild].dataHora < heap[smallest].dataHora ||
                                    (heap[leftChild].dataHora == heap[smallest].dataHora && heap[leftChild].pacienteId < heap[smallest].pacienteId)))
        {
            smallest = leftChild;
        }
        if (rightChild < tamanho && (heap[rightChild].dataHora < heap[smallest].dataHora ||
                                     (heap[rightChild].dataHora == heap[smallest].dataHora && heap[rightChild].pacienteId < heap[smallest].pacienteId)))
        {
            smallest = rightChild;
        }
        if (smallest == index)
        {
            break;
        }
        std::swap(heap[index], heap[smallest]);
        index = smallest;
    }
}

void Escalonador::expandHeap()
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