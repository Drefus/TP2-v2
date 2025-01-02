#include "Procedimento.hpp"

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
    unidades[index] = false;
}

int Procedimento::getTotalUnidades()
{
    return totalUnidades;
}