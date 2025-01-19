#include "Paciente.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Construtores e Destrutor
Paciente::Paciente() : id(0), alta(0), prioridade(GREEN), tempoChegada(), numMedidasHospitalares(0), numTestesDeLaboratorio(0), numExamesDeImagem(0), numMedicamentos(0), tempoAtendimento(0.0), tempoEspera(0.0), status(NOT_ARRIVED) {}

Paciente::Paciente(int id, int alta, int prioridade, int ano, int mes, int dia, double hora, int numMedidasHospitalares, int numTestesDeLaboratorio, int numExamesDeImagem, int numMedicamentos) : id(id), alta(alta), prioridade(static_cast<Prioridade>(prioridade)), numMedidasHospitalares(numMedidasHospitalares), numTestesDeLaboratorio(numTestesDeLaboratorio), numExamesDeImagem(numExamesDeImagem), numMedicamentos(numMedicamentos), tempoAtendimento(0.0), tempoEspera(0.0), status(NOT_ARRIVED), tempoAtualEmHoras(0.0)
{
    // Inicializa a estrutura tm com a data e hora fornecidas
    tm data = {};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;
    data.tm_hour = static_cast<int>(hora);
    data.tm_min = 0;
    data.tm_sec = 0;
    data.tm_isdst = -1;
    time_t tempo = mktime(&data);

    // Converte time_t para tm e armazena em tempoChegada
    this->tempoChegada = *localtime(&tempo);
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
double Paciente::getTempoAtendimento() { return tempoAtendimento; }
double Paciente::getTempoEspera() { return tempoEspera; }
Status Paciente::getStatus() { return status; }

// Setters
void Paciente::setId(int id)
{
    if (id < 0)
    {
        throw std::invalid_argument("ID não pode ser negativo");
    }
    this->id = id;
}
void Paciente::setAlta(int alta)
{
    if (alta < 0)
    {
        throw std::invalid_argument("Alta não pode ser negativa");
    }
    this->alta = alta;
}
void Paciente::setPrioridade(Prioridade prioridade)
{
    if (prioridade < GREEN || prioridade > RED)
    {
        throw std::invalid_argument("Prioridade inválida");
    }
    this->prioridade = prioridade;
}
void Paciente::setTempoChegada(tm tempoChegada)
{
    if (tempoChegada.tm_year < 0 || tempoChegada.tm_mon < 0 || tempoChegada.tm_mday < 0 || tempoChegada.tm_hour < 0)
    {
        throw std::invalid_argument("Tempo de chegada inválido");
    }
    this->tempoChegada = tempoChegada;
}
void Paciente::setNumMedidasHospitalares(int numMedidasHospitalares)
{
    if (numMedidasHospitalares < 0)
    {
        throw std::invalid_argument("Número de medidas hospitalares não pode ser negativo");
    }
    this->numMedidasHospitalares = numMedidasHospitalares;
}
void Paciente::setNumTestesDeLaboratorio(int numTestesDeLaboratorio)
{
    if (numTestesDeLaboratorio < 0)
    {
        throw std::invalid_argument("Número de testes de laboratório não pode ser negativo");
    }
    this->numTestesDeLaboratorio = numTestesDeLaboratorio;
}
void Paciente::setNumExamesDeImagem(int numExamesDeImagem)
{
    if (numExamesDeImagem < 0)
    {
        throw std::invalid_argument("Número de exames de imagem não pode ser negativo");
    }
    this->numExamesDeImagem = numExamesDeImagem;
}
void Paciente::setNumMedicamentos(int numMedicamentos)
{
    if (numMedicamentos < 0)
    {
        throw std::invalid_argument("Número de medicamentos não pode ser negativo");
    }
    this->numMedicamentos = numMedicamentos;
}
void Paciente::setTempoAtendimento(double tempoAtendimento)
{
    if (tempoAtendimento < 0)
    {
        throw std::invalid_argument("Tempo de atendimento não pode ser negativo");
    }
    this->tempoAtendimento = tempoAtendimento;
}
void Paciente::setTempoEspera(double tempoEspera)
{
    if (tempoEspera < 0)
    {
        throw std::invalid_argument("Tempo de espera não pode ser negativo");
    }
    this->tempoEspera = tempoEspera;
}
void Paciente::setStatus(Status status)
{
    if (status < NOT_ARRIVED || status > HOSPITAL_DISCHARGED)
    {
        throw std::invalid_argument("Status inválido");
    }
    this->status = status;
}

// Calcula o tempo de chegada em horas
double Paciente::getTempoDeChegadaEmHoras()
{
    return tempoChegada.tm_hour + tempoChegada.tm_mday * 24 + tempoChegada.tm_mon * 30 * 24 + tempoChegada.tm_year * 365 * 24; // ano bissexto
}

// Adiciona tempo de espera ao paciente
void Paciente::addTempoEspera(double tempo)
{
    if (tempo < 0)
    {
        throw std::invalid_argument("Tempo de espera não pode ser negativo");
    }
    tempoEspera += tempo;
}

// Adiciona tempo de atendimento ao paciente
void Paciente::addTempoAtendimento(double tempo)
{
    if (tempo < 0)
    {
        throw std::invalid_argument("Tempo de atendimento não pode ser negativo");
    }
    tempoAtendimento += tempo;
}

// Retorna o tempo atual em horas
double Paciente::getTempoAtualEmHoras()
{
    return tempoAtualEmHoras;
}

// Define o tempo atual em horas
void Paciente::setTempoAtualEmHoras(double tempoAtualEmHoras)
{
    this->tempoAtualEmHoras = tempoAtualEmHoras;
}
