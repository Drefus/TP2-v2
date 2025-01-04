#ifndef PACIENTE_HPP
#define PACIENTE_HPP
#include <ctime>

enum Prioridade
{
    GREEN = 0,
    YELLOW = 1,
    RED = 2
};

enum Status
{
    NOT_ARRIVED = 0,
    SCREENING_QUEUE = 1,
    IN_SCREENING = 2,
    MEDICAL_CONSULTATION_QUEUE = 3,
    IN_MEDICAL_CONSULTATION = 4,
    MEDICAL_TREATMENT_QUEUE = 5,
    IN_MEDICAL_TREATMENT = 6,
    TEST_QUEUE = 7,
    IN_TEST = 8,
    IMAGING_TEST_QUEUE = 9,
    IN_IMAGING_TEST = 10,
    MEDICINE_QUEUE = 11,
    IN_MEDICINE = 12,
    HOSPITAL_DISCHARGED = 13
};

class Paciente
{
private:
    int id;
    int alta;
    Prioridade prioridade;
    tm tempoChegada;
    int numMedidasHospitalares;
    int numTestesDeLaboratorio;
    int numExamesDeImagem;
    int numMedicamentos;
    tm tempoAtual;
    double tempoAtendimento;
    double tempoEspera;
    Status status;
    double tempoAtualEmHoras;

public:
    Paciente();
    Paciente(int id, int alta, int prioridade, int ano, int mes, int dia, double hora, int numMedidasHospitalares, int numTestesDeLaboratorio, int numExamesDeImagem, int numMedicamentos);
    ~Paciente();

    // Getters
    int getId();
    int getAlta();
    Prioridade getPrioridade();
    tm getTempoChegada();
    int getNumMedidasHospitalares();
    int getNumTestesDeLaboratorio();
    int getNumExamesDeImagem();
    int getNumMedicamentos();
    tm getTempoAtual();
    double getTempoAtendimento();
    double getTempoEspera();
    Status getStatus();

    // Setters
    void setId(int id);
    void setAlta(int alta);
    void setPrioridade(Prioridade prioridade);
    void setTempoChegada(tm tempoChegada);
    void setNumMedidasHospitalares(int numMedidasHospitalares);
    void setNumTestesDeLaboratorio(int numTestesDeLaboratorio);
    void setNumExamesDeImagem(int numExamesDeImagem);
    void setNumMedicamentos(int numMedicamentos);
    void setTempoAtual(tm tempoAtual);
    void setTempoAtendimento(double tempoAtendimento);
    void setTempoEspera(double tempoEspera);
    void setStatus(Status status);

    double getTempoDeChegadaEmHoras();
    double getTempoDeAtualEmHoras();
    void addTempoEspera(double tempo);
    void addTempoAtendimento(double tempo);

    double getTempoAtualEmHoras();
    void setTempoAtualEmHoras(double tempoAtualEmHoras);
};

#endif