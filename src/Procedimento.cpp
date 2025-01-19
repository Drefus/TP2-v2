#include "Procedimento.hpp"
#include <iostream>
#include <stdexcept>
using namespace std;

// Construtor da classe Procedimento
Procedimento::Procedimento(int n) : totalUnidades(n)
{
    if (n <= 0)
    {
        throw std::invalid_argument("Número de unidades deve ser maior que zero");
    }
    // Aloca um array de booleanos para representar as unidades
    unidades = new bool[n];
    // Inicializa todas as unidades como desocupadas (false)
    for (int i = 0; i < n; ++i)
    {
        unidades[i] = false;
    }
}

// Destrutor da classe Procedimento
Procedimento::~Procedimento()
{
    // Libera a memória alocada para o array de unidades
    delete[] unidades;
}

// Retorna o índice da primeira unidade livre (desocupada)
int Procedimento::getUnidadeLivre()
{
    for (int i = 0; i < totalUnidades; ++i)
    {
        if (!unidades[i])
        {
            return i;
        }
    }
    // Retorna -1 se não houver unidades livres
    return -1;
}

// Retorna o número de unidades atualmente em uso (ocupadas)
int Procedimento::numUnidadesEmUso()
{
    int count = 0;
    for (int i = 0; i < totalUnidades; ++i)
    {
        if (unidades[i])
        {
            count++;
        }
    }
    return count;
}

// Verifica se uma unidade específica está ocupada
bool Procedimento::isOcupada(int index)
{
    if (index < 0 || index >= totalUnidades)
    {
        throw std::out_of_range("Índice fora do intervalo");
    }
    return unidades[index];
}

// Marca uma unidade específica como ocupada
void Procedimento::ocupar(int index)
{
    if (index < 0 || index >= totalUnidades)
    {
        throw std::out_of_range("Índice fora do intervalo");
    }
    unidades[index] = true;
}

// Marca uma unidade específica como desocupada
void Procedimento::desocupar(int index)
{
    if (index < 0 || index >= totalUnidades)
    {
        throw std::out_of_range("Índice fora do intervalo");
    }
    unidades[index] = false;
}

// Retorna o número total de unidades
int Procedimento::getTotalUnidades()
{
    return totalUnidades;
}