#include "Procedimento.hpp"
#include <iostream>
using namespace std;

Procedimento::Procedimento(int n) : totalUnidades(n)
{
    unidades = new bool[n];
    for (int i = 0; i < n; ++i)
    {
        unidades[i] = false;
    }
}

Procedimento::~Procedimento()
{
    delete[] unidades;
}

int Procedimento::getUnidadeLivre()
{
    for (int i = 0; i < totalUnidades; ++i)
    {
        if (!unidades[i])
        {
            return i;
        }
    }
    return -1;
}

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

bool Procedimento::isOcupada(int index)
{
    return unidades[index];
}

void Procedimento::ocupar(int index)
{
    unidades[index] = true;
}

void Procedimento::desocupar(int index)
{
    if (index >= 0 && index < totalUnidades)
        unidades[index] = false;
    else
        cout << "Unidade inexistente :" << index << endl;
}

int Procedimento::getTotalUnidades()
{
    return totalUnidades;
}