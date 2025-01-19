#ifndef PROCEDIMENTO_HPP
#define PROCEDIMENTO_HPP

// Classe que representa um procedimento com várias unidades
class Procedimento
{
private:
    bool *unidades;    // Array de booleanos que indica se a unidade está ocupada
    int totalUnidades; // Total de unidades disponíveis

public:
    // Construtor que inicializa o procedimento com n unidades
    Procedimento(int n);

    // Verifica se a unidade no índice especificado está ocupada
    bool isOcupada(int index);

    // Ocupa a unidade no índice especificado
    void ocupar(int index);

    // Desocupa a unidade no índice especificado
    void desocupar(int index);

    // Retorna o total de unidades
    int getTotalUnidades();

    // Destrutor que libera a memória alocada para as unidades
    ~Procedimento();

    // Retorna o índice da primeira unidade livre
    int getUnidadeLivre();

    // Retorna o número de unidades atualmente em uso
    int numUnidadesEmUso();
};

#endif