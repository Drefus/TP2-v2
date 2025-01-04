#ifndef PROCEDIMENTO_HPP
#define PROCEDIMENTO_HPP
class Procedimento
{
private:
    bool *unidades;
    int totalUnidades;

public:
    Procedimento(int n);
    bool isOcupada(int index);
    void ocupar(int index);
    void desocupar(int index);
    int getTotalUnidades();
    ~Procedimento();
    int getUnidadeLivre();
    int numUnidadesEmUso();
};
#endif