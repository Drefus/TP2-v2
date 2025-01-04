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

class HospitalManager
{
public:
    HospitalManager();
    ~HospitalManager();
    int getPaciemteIndex(int id);
    Paciente getPacienteById(int id);
    std::string ToDataDeChegadaESaidas(tm dataDeChegada, double tempoTotal);
    void run();
    void processEvents();
    void processQueue(Fila &fila, Procedimento &procedimento, Status novoStatus, double duracaoBase);
    double getMultiplicadorDuracao(Status status, Paciente paciente);
    void printResults();
    void readInput();
    double relogio;
    double duracaoTriagem, duracaoAtendimento, duracaoMedidasHospitalares, duracaoTestesDeLaboratorio, duracaoExamesDeImagem, duracaoMedicamentos;
    int capacidadeTriagem, capacidadeAtendimento, capacidadeMedidasHospitalares, capacidadeTestesDeLaboratorio, capacidadeExamesDeImagem, capacidadeMedicamentos;
    int numPacientes;
    Paciente *pacientes;
    Escalonador escalonador;
    Fila filaDeTriagem, filaDeAtendimento, filaDeMedidasHospitalares, filaDeTestesDeLaboratorio, filaDeExamesDeImagem, filaDeMedicamentos;
};

#endif // HOSPITALMANAGER_HPP