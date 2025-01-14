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

class HospitalManager
{
public:
    HospitalManager();
    ~HospitalManager();
    int getPaciemteIndex(int id);
    Paciente getPacienteById(int id);
    tm adicionarHoras(tm data, double horas);
    std::string ToDataDeChegadaESaidas(tm dataDeChegada, double tempoTotal);
    void run(string arquivo);
    void processEvents();
    void processQueue(Fila &fila, Procedimento &procedimento, Status novoStatus, double duracaoBase);
    double getMultiplicadorDuracao(Status status, Paciente paciente);
    void printResults();
    void readInput(string arquivo);
    double relogio;
    double duracaoTriagem, duracaoAtendimento, duracaoMedidasHospitalares, duracaoTestesDeLaboratorio, duracaoExamesDeImagem, duracaoMedicamentos;
    int capacidadeTriagem, capacidadeAtendimento, capacidadeMedidasHospitalares, capacidadeTestesDeLaboratorio, capacidadeExamesDeImagem, capacidadeMedicamentos;
    int numPacientes;
    Paciente *pacientes;
    Escalonador escalonador;
    Fila filaDeTriagem = Fila(true);
    Fila filaDeAtendimento, filaDeMedidasHospitalares, filaDeTestesDeLaboratorio, filaDeExamesDeImagem, filaDeMedicamentos;
};

#endif // HOSPITALMANAGER_HPP