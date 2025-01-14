#include "HospitalManager.hpp"
#include <fstream>

using namespace std;

HospitalManager::HospitalManager()
{
}

HospitalManager::~HospitalManager()
{
    delete[] pacientes;
}

int HospitalManager::getPaciemteIndex(int id)
{
    for (int i = 0; i < numPacientes; ++i)
    {
        if (pacientes[i].getId() == id)
        {
            return i;
        }
    }
    return -1;
}

Paciente HospitalManager::getPacienteById(int id)
{
    for (int i = 0; i < numPacientes; ++i)
    {
        if (pacientes[i].getId() == id)
        {
            return pacientes[i];
        }
    }
    return Paciente();
}

tm HospitalManager::adicionarHoras(struct tm data, double horas)
{
    // Converte a estrutura tm para time_t
    time_t tempo = mktime(&data);

    // Converte horas para segundos e adiciona ao tempo
    tempo += static_cast<time_t>(horas * 3600);

    // Converte de volta para tm
    struct tm novaData = *localtime(&tempo);

    // Arredonda os segundos para minutos
    if (novaData.tm_sec >= 30)
    {
        novaData.tm_min += 1;
    }
    novaData.tm_sec = 0;

    time_t formmatDate = mktime(&novaData);

    return *localtime(&formmatDate);
}

std::string HospitalManager::ToDataDeChegadaESaidas(tm dataDeChegada, double tempoTotal)
{
    std::stringstream ss, ss2;
    ss << std::put_time(&dataDeChegada, "%a %b ") << dataDeChegada.tm_mday << std::put_time(&dataDeChegada, " %H:%M:%S %Y");
    std::string dataDeChegadaStr = ss.str();
    dataDeChegada = adicionarHoras(dataDeChegada, tempoTotal);
    ss2 << std::put_time(&dataDeChegada, "%a %b ") << dataDeChegada.tm_mday << std::put_time(&dataDeChegada, " %H:%M:%S %Y");
    std::string dataDeSaidaStr = ss2.str();
    return dataDeChegadaStr + " " + dataDeSaidaStr;
}

void HospitalManager::run(string arquivo)
{
    readInput(arquivo);
    processEvents();
    printResults();
}

void HospitalManager::processEvents()
{
    Procedimento triagem(capacidadeTriagem);
    Procedimento atendimento(capacidadeAtendimento);
    Procedimento medidasHospitalares(capacidadeMedidasHospitalares);
    Procedimento testesDeLaboratorio(capacidadeTestesDeLaboratorio);
    Procedimento examesDeImagem(capacidadeExamesDeImagem);
    Procedimento medicamentos(capacidadeMedicamentos);

    while (!escalonador.vazio() || !filaDeTriagem.filaVazia() || !filaDeAtendimento.filaVazia() || !filaDeMedidasHospitalares.filaVazia() || !filaDeTestesDeLaboratorio.filaVazia() || !filaDeExamesDeImagem.filaVazia() || !filaDeMedicamentos.filaVazia())
    {
        if (!escalonador.vazio())
        {
            Evento evento = escalonador.retiraProximoEvento();
            relogio = evento.dataHora;
            int pacienteIndex = getPaciemteIndex(evento.pacienteId);
            Paciente *paciente = &pacientes[pacienteIndex];
            paciente->setTempoAtualEmHoras(relogio);

            switch (paciente->getStatus())
            {
            case NOT_ARRIVED:
                filaDeTriagem.enfileira(paciente);
                break;
            case IN_SCREENING:
                if (evento.numUnit != -1)
                    triagem.desocupar(evento.numUnit);
                filaDeAtendimento.enfileira(paciente);
                break;
            case IN_MEDICAL_CONSULTATION:
                if (evento.numUnit != -1)
                    atendimento.desocupar(evento.numUnit);
                if (paciente->getAlta() == 0)
                    filaDeMedidasHospitalares.enfileira(paciente);
                else
                    paciente->setStatus(HOSPITAL_DISCHARGED);
                break;
            case IN_MEDICAL_TREATMENT:
                if (evento.numUnit != -1)
                    medidasHospitalares.desocupar(evento.numUnit);
                filaDeTestesDeLaboratorio.enfileira(paciente);
                break;
            case IN_TEST:
                if (evento.numUnit != -1)
                    testesDeLaboratorio.desocupar(evento.numUnit);
                filaDeExamesDeImagem.enfileira(paciente);
                break;
            case IN_IMAGING_TEST:
                if (evento.numUnit != -1)
                    examesDeImagem.desocupar(evento.numUnit);
                filaDeMedicamentos.enfileira(paciente);
                break;
            case IN_MEDICINE:
                if (evento.numUnit != -1)
                    medicamentos.desocupar(evento.numUnit);
                paciente->setStatus(HOSPITAL_DISCHARGED);
                break;
            default:
                break;
            }
        }

        processQueue(filaDeTriagem, triagem, IN_SCREENING, duracaoTriagem);
        processQueue(filaDeAtendimento, atendimento, IN_MEDICAL_CONSULTATION, duracaoAtendimento);
        processQueue(filaDeMedidasHospitalares, medidasHospitalares, IN_MEDICAL_TREATMENT, duracaoMedidasHospitalares);
        processQueue(filaDeTestesDeLaboratorio, testesDeLaboratorio, IN_TEST, duracaoTestesDeLaboratorio);
        processQueue(filaDeExamesDeImagem, examesDeImagem, IN_IMAGING_TEST, duracaoExamesDeImagem);
        processQueue(filaDeMedicamentos, medicamentos, IN_MEDICINE, duracaoMedicamentos);
    }
}

void HospitalManager::processQueue(Fila &fila, Procedimento &procedimento, Status novoStatus, double duracaoBase)
{
    int unidadeLivre = procedimento.getUnidadeLivre();
    while (!fila.filaVazia() && unidadeLivre != -1)
    {
        Paciente *paciente = fila.desenfileira();
        double atraso = relogio - paciente->getTempoAtualEmHoras();
        paciente->addTempoEspera(atraso);
        procedimento.ocupar(unidadeLivre);
        paciente->setStatus(novoStatus);
        double duracao = duracaoBase * getMultiplicadorDuracao(novoStatus, *paciente);
        Evento novoEvento(relogio + duracao, paciente->getId(), unidadeLivre);
        escalonador.insereEvento(novoEvento);
        paciente->addTempoAtendimento(duracao);
        unidadeLivre = procedimento.getUnidadeLivre();
    }
}

double HospitalManager::getMultiplicadorDuracao(Status status, Paciente paciente)
{
    switch (status)
    {
    case IN_MEDICAL_TREATMENT:
        return paciente.getNumMedidasHospitalares();
    case IN_TEST:
        return paciente.getNumTestesDeLaboratorio();
    case IN_IMAGING_TEST:
        return paciente.getNumExamesDeImagem();
    case IN_MEDICINE:
        return paciente.getNumMedicamentos();
    default:
        return 1.0;
    }
}

void HospitalManager::printResults()
{
    for (int i = 0; i < numPacientes; ++i)
    {
        double tempoTotal = pacientes[i].getTempoAtendimento() + pacientes[i].getTempoEspera();
        string dataDeChegadaESaida = ToDataDeChegadaESaidas(pacientes[i].getTempoChegada(), tempoTotal);
        cout << fixed << setprecision(2) << pacientes[i].getId() << " " << dataDeChegadaESaida << " " << tempoTotal << " " << pacientes[i].getTempoAtendimento() << " " << pacientes[i].getTempoEspera() << endl;
    }
}

void HospitalManager::readInput(string arquivo)
{
    relogio = 0;
    ifstream inputFile(arquivo);
    if (!inputFile.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << arquivo << endl;
        return;
    }

    inputFile >> duracaoTriagem >> capacidadeTriagem;
    inputFile >> duracaoAtendimento >> capacidadeAtendimento;
    inputFile >> duracaoMedidasHospitalares >> capacidadeMedidasHospitalares;
    inputFile >> duracaoTestesDeLaboratorio >> capacidadeTestesDeLaboratorio;
    inputFile >> duracaoExamesDeImagem >> capacidadeExamesDeImagem;
    inputFile >> duracaoMedicamentos >> capacidadeMedicamentos;
    inputFile >> numPacientes;
    pacientes = new Paciente[numPacientes];
    for (int i = 0; i < numPacientes; ++i)
    {
        int id, alta, prioridade, ano, mes, dia, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos;
        double hora;
        inputFile >> id >> alta >> ano >> mes >> dia >> hora >> prioridade >> numMedidasHospitalares >> numTestesDeLaboratorio >> numExamesDeImagem >> numMedicamentos;
        pacientes[i] = Paciente(id, alta, prioridade, ano, mes, dia, hora, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos);
        Evento evento(pacientes[i].getTempoDeChegadaEmHoras(), pacientes[i].getId(), -1);
        escalonador.insereEvento(evento);
    }
    inputFile.close();
}
