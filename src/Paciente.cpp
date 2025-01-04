#include "Paciente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
// Constructors and Destructor
Paciente::Paciente() : id(0), alta(0), prioridade(GREEN), tempoChegada(), numMedidasHospitalares(0), numTestesDeLaboratorio(0), numExamesDeImagem(0), numMedicamentos(0), tempoAtual(), tempoAtendimento(0.0), tempoEspera(0.0), status(NOT_ARRIVED) {}

Paciente::Paciente(int id, int alta, int prioridade, int ano, int mes, int dia, double hora, int numMedidasHospitalares, int numTestesDeLaboratorio, int numExamesDeImagem, int numMedicamentos) : id(id), alta(alta), prioridade(static_cast<Prioridade>(prioridade)), numMedidasHospitalares(numMedidasHospitalares), numTestesDeLaboratorio(numTestesDeLaboratorio), numExamesDeImagem(numExamesDeImagem), numMedicamentos(numMedicamentos), tempoAtendimento(0.0), tempoEspera(0.0), status(NOT_ARRIVED), tempoAtualEmHoras(0.0)
{
    tm data = {};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;
    data.tm_hour = static_cast<int>(hora);
    data.tm_min = 0;
    data.tm_sec = 0;
    data.tm_isdst = -1;
    time_t tempo = mktime(&data);

    this->tempoChegada = *localtime(&tempo);
    this->tempoAtual = this->tempoChegada;
}

Paciente::~Paciente() {}

// Getters
int Paciente::getId() { return id; }
int Paciente::getAlta() { return alta; }
Prioridade Paciente::getPrioridade() { return prioridade; }
tm Paciente::getTempoChegada() { return tempoChegada; }
int Paciente::getNumMedidasHospitalares() { return numMedidasHospitalares; }
int Paciente::getNumTestesDeLaboratorio() { return numTestesDeLaboratorio; }
int Paciente::getNumExamesDeImagem() { return numExamesDeImagem; }
int Paciente::getNumMedicamentos() { return numMedicamentos; }
tm Paciente::getTempoAtual() { return tempoAtual; }
double Paciente::getTempoAtendimento() { return tempoAtendimento; }
double Paciente::getTempoEspera() { return tempoEspera; }
Status Paciente::getStatus() { return status; }

// Setters
void Paciente::setId(int id) { this->id = id; }
void Paciente::setAlta(int alta) { this->alta = alta; }
void Paciente::setPrioridade(Prioridade prioridade) { this->prioridade = prioridade; }
void Paciente::setTempoChegada(tm tempoChegada) { this->tempoChegada = tempoChegada; }
void Paciente::setNumMedidasHospitalares(int numMedidasHospitalares) { this->numMedidasHospitalares = numMedidasHospitalares; }
void Paciente::setNumTestesDeLaboratorio(int numTestesDeLaboratorio) { this->numTestesDeLaboratorio = numTestesDeLaboratorio; }
void Paciente::setNumExamesDeImagem(int numExamesDeImagem) { this->numExamesDeImagem = numExamesDeImagem; }
void Paciente::setNumMedicamentos(int numMedicamentos) { this->numMedicamentos = numMedicamentos; }
void Paciente::setTempoAtual(tm tempoAtual) { this->tempoAtual = tempoAtual; }
void Paciente::setTempoAtendimento(double tempoAtendimento) { this->tempoAtendimento = tempoAtendimento; }
void Paciente::setTempoEspera(double tempoEspera) { this->tempoEspera = tempoEspera; }
void Paciente::setStatus(Status status) { this->status = status; }

double Paciente::getTempoDeChegadaEmHoras()
{
    return tempoChegada.tm_hour + tempoChegada.tm_mday * 24 + tempoChegada.tm_mon * 30 * 24 + tempoChegada.tm_year * 365 * 24;
}

double Paciente::getTempoDeAtualEmHoras()
{
    return tempoAtual.tm_hour + tempoAtual.tm_mday * 24 + tempoAtual.tm_mon * 30 * 24 + tempoAtual.tm_year * 365 * 24;
}

void Paciente::addTempoEspera(double tempo)
{
    tempoEspera += tempo;
}

void Paciente::addTempoAtendimento(double tempo)
{
    tempoAtendimento += tempo;
}

double Paciente::getTempoAtualEmHoras()
{
    return tempoAtualEmHoras;
}

void Paciente::setTempoAtualEmHoras(double tempoAtualEmHoras)
{
    this->tempoAtualEmHoras = tempoAtualEmHoras;
}
