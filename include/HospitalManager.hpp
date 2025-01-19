#ifndef HOSPITALMANAGER_HPP
#define HOSPITALMANAGER_HPP
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Fila.hpp"
#include "Paciente.hpp"
#include "Procedimento.hpp"
#include "Escalonador.hpp"
#include <cmath>

using namespace std;

// Classe que gerencia o hospital
class HospitalManager
{
public:
    // Construtor
    HospitalManager();
    // Destrutor
    ~HospitalManager();

    // Adiciona horas a uma data
    tm adicionarHoras(tm data, double horas);

    // Converte a data de chegada e saída para string
    std::string paraDataDeChegadaESaida(tm dataDeChegada, double tempoTotal);

    // Executa o processamento a partir de um arquivo
    void executar(string arquivo);

    // Processa os eventos
    void processarEventos();

    // Processa uma fila específica
    void processarFila(Fila &fila, Procedimento &procedimento, Status novoStatus, double duracaoBase);

    // Processa múltiplas filas
    void processarMultiFila(Fila fila[3], Procedimento &procedimento, Status novoStatus, double duracaoBase);

    // Obtém o multiplicador de duração baseado no status e paciente
    double obterMultiplicadorDuracao(Status status, Paciente paciente);

    // Imprime os resultados
    void imprimirResultados();

    // Lê a entrada de um arquivo
    void lerEntrada(string arquivo);

    // Verifica se as filas estão vazias
    bool filasVazias();

    // Relógio do sistema
    double relogio;

    // Durações de diferentes procedimentos
    double duracaoTriagem, duracaoAtendimento, duracaoMedidasHospitalares, duracaoTestesDeLaboratorio, duracaoExamesDeImagem, duracaoMedicamentos;

    // Capacidades de diferentes procedimentos
    int capacidadeTriagem, capacidadeAtendimento, capacidadeMedidasHospitalares, capacidadeTestesDeLaboratorio, capacidadeExamesDeImagem, capacidadeMedicamentos;

    // Número de pacientes
    int numPacientes;

    // Array de pacientes
    Paciente *pacientes;

    // Ponteiro para o escalonador
    Escalonador *escalonador = nullptr;

    // Filas de diferentes procedimentos
    Fila filaDeTriagem, filaDeAtendimento[3], filaDeMedidasHospitalares[3], filaDeTestesDeLaboratorio[3], filaDeExamesDeImagem[3], filaDeMedicamentos[3];
};

#endif