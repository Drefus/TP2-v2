#ifndef PACIENTE_HPP
#define PACIENTE_HPP
#include <ctime>

// Enum para definir a prioridade do paciente
enum Prioridade
{
    GREEN = 0,  // Baixa prioridade
    YELLOW = 1, // Média prioridade
    RED = 2     // Alta prioridade
};

// Enum para definir o status do paciente
enum Status
{
    NOT_ARRIVED = 0,                // Não chegou
    SCREENING_QUEUE = 1,            // Na fila de triagem
    IN_SCREENING = 2,               // Em triagem
    MEDICAL_CONSULTATION_QUEUE = 3, // Na fila de consulta médica
    IN_MEDICAL_CONSULTATION = 4,    // Em consulta médica
    MEDICAL_TREATMENT_QUEUE = 5,    // Na fila de tratamento médico
    IN_MEDICAL_TREATMENT = 6,       // Em tratamento médico
    TEST_QUEUE = 7,                 // Na fila de testes
    IN_TEST = 8,                    // Em teste
    IMAGING_TEST_QUEUE = 9,         // Na fila de exames de imagem
    IN_IMAGING_TEST = 10,           // Em exame de imagem
    MEDICINE_QUEUE = 11,            // Na fila de medicamentos
    IN_MEDICINE = 12,               // Em medicamento
    HOSPITAL_DISCHARGED = 13        // Alta hospitalar
};

// Classe que representa um paciente
class Paciente
{
private:
    int id;                     // Identificador do paciente
    int alta;                   // Alta do paciente
    Prioridade prioridade;      // Prioridade do paciente
    tm tempoChegada;            // Tempo de chegada do paciente
    int numMedidasHospitalares; // Número de medidas hospitalares
    int numTestesDeLaboratorio; // Número de testes de laboratório
    int numExamesDeImagem;      // Número de exames de imagem
    int numMedicamentos;        // Número de medicamentos
    double tempoAtendimento;    // Tempo de atendimento
    double tempoEspera;         // Tempo de espera
    Status status;              // Status do paciente
    double tempoAtualEmHoras;   // Tempo atual em horas

public:
    // Construtores e destrutor
    Paciente();
    Paciente(int id, int alta, int prioridade, int ano, int mes, int dia, double hora, int numMedidasHospitalares, int numTestesDeLaboratorio, int numExamesDeImagem, int numMedicamentos);
    ~Paciente();

    // Getters
    int getId();                     // Retorna o ID do paciente
    int getAlta();                   // Retorna a alta do paciente
    Prioridade getPrioridade();      // Retorna a prioridade do paciente
    tm getTempoChegada();            // Retorna o tempo de chegada do paciente
    int getNumMedidasHospitalares(); // Retorna o número de medidas hospitalares
    int getNumTestesDeLaboratorio(); // Retorna o número de testes de laboratório
    int getNumExamesDeImagem();      // Retorna o número de exames de imagem
    int getNumMedicamentos();        // Retorna o número de medicamentos
    double getTempoAtendimento();    // Retorna o tempo de atendimento
    double getTempoEspera();         // Retorna o tempo de espera
    Status getStatus();              // Retorna o status do paciente

    // Setters
    void setId(int id);                                         // Define o ID do paciente
    void setAlta(int alta);                                     // Define a alta do paciente
    void setPrioridade(Prioridade prioridade);                  // Define a prioridade do paciente
    void setTempoChegada(tm tempoChegada);                      // Define o tempo de chegada do paciente
    void setNumMedidasHospitalares(int numMedidasHospitalares); // Define o número de medidas hospitalares
    void setNumTestesDeLaboratorio(int numTestesDeLaboratorio); // Define o número de testes de laboratório
    void setNumExamesDeImagem(int numExamesDeImagem);           // Define o número de exames de imagem
    void setNumMedicamentos(int numMedicamentos);               // Define o número de medicamentos
    void setTempoAtual(tm tempoAtual);                          // Define o tempo atual
    void setTempoAtendimento(double tempoAtendimento);          // Define o tempo de atendimento
    void setTempoEspera(double tempoEspera);                    // Define o tempo de espera
    void setStatus(Status status);                              // Define o status do paciente

    // Métodos adicionais
    double getTempoDeChegadaEmHoras();      // Retorna o tempo de chegada em horas
    void addTempoEspera(double tempo);      // Adiciona tempo de espera
    void addTempoAtendimento(double tempo); // Adiciona tempo de atendimento

    double getTempoAtualEmHoras();                       // Retorna o tempo atual em horas
    void setTempoAtualEmHoras(double tempoAtualEmHoras); // Define o tempo atual em horas
};

#endif