#include "HospitalManager.hpp"
#include <fstream>

using namespace std;

HospitalManager::HospitalManager()
{
}

HospitalManager::~HospitalManager()
{
    delete[] pacientes;
    delete escalonador;
}

// Adiciona horas a uma data e retorna a nova data
tm HospitalManager::adicionarHoras(struct tm data, double horas)
{
    if (horas < 0)
    {
        throw std::invalid_argument("Horas não podem ser negativas");
    }
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

    time_t dataFormatada = mktime(&novaData);

    return *localtime(&dataFormatada);
}

// Converte a data de chegada e o tempo total em uma string formatada
std::string HospitalManager::paraDataDeChegadaESaida(tm dataDeChegada, double tempoTotal)
{
    std::stringstream ss, ss2;
    ss << std::put_time(&dataDeChegada, "%a %b ") << dataDeChegada.tm_mday << std::put_time(&dataDeChegada, " %H:%M:%S %Y");
    std::string dataDeChegadaStr = ss.str();
    dataDeChegada = adicionarHoras(dataDeChegada, tempoTotal);
    ss2 << std::put_time(&dataDeChegada, "%a %b ") << dataDeChegada.tm_mday << std::put_time(&dataDeChegada, " %H:%M:%S %Y");
    std::string dataDeSaidaStr = ss2.str();
    return dataDeChegadaStr + " " + dataDeSaidaStr;
}

// Executa o processamento do arquivo de entrada
void HospitalManager::executar(string arquivo)
{
    if (arquivo.empty())
    {
        throw std::invalid_argument("Nome do arquivo não pode ser vazio");
    }
    lerEntrada(arquivo);
    processarEventos();
    imprimirResultados();
}

// Processa os eventos do hospital
void HospitalManager::processarEventos()
{
    // Inicializa os procedimentos com suas respectivas capacidades
    Procedimento triagem(capacidadeTriagem);
    Procedimento atendimento(capacidadeAtendimento);
    Procedimento medidasHospitalares(capacidadeMedidasHospitalares);
    Procedimento testesDeLaboratorio(capacidadeTestesDeLaboratorio);
    Procedimento examesDeImagem(capacidadeExamesDeImagem);
    Procedimento medicamentos(capacidadeMedicamentos);

    // Processa eventos enquanto houver eventos no escalonador ou filas não vazias
    while (!escalonador->vazio() || !filasVazias())
    {
        if (!escalonador->vazio())
        {
            Evento evento = escalonador->retirarProximoEvento();
            relogio = evento.dataHora;
            Paciente *paciente = evento.paciente;
            paciente->setTempoAtualEmHoras(relogio);

            // Processa o evento de acordo com o status do paciente
            switch (paciente->getStatus())
            {
            case NOT_ARRIVED:
                filaDeTriagem.enfileira(paciente);
                break;
            case IN_SCREENING:
                triagem.desocupar(evento.numUnidade);
                filaDeAtendimento[static_cast<int>(paciente->getPrioridade())].enfileira(paciente);
                break;
            case IN_MEDICAL_CONSULTATION:
                atendimento.desocupar(evento.numUnidade);
                if (paciente->getAlta() == 0)
                    filaDeMedidasHospitalares[static_cast<int>(paciente->getPrioridade())].enfileira(paciente);
                else
                    paciente->setStatus(HOSPITAL_DISCHARGED);
                break;
            case IN_MEDICAL_TREATMENT:
                medidasHospitalares.desocupar(evento.numUnidade);
                filaDeTestesDeLaboratorio[static_cast<int>(paciente->getPrioridade())].enfileira(paciente);
                break;
            case IN_TEST:
                testesDeLaboratorio.desocupar(evento.numUnidade);
                filaDeExamesDeImagem[static_cast<int>(paciente->getPrioridade())].enfileira(paciente);
                break;
            case IN_IMAGING_TEST:
                examesDeImagem.desocupar(evento.numUnidade);
                filaDeMedicamentos[static_cast<int>(paciente->getPrioridade())].enfileira(paciente);
                break;
            case IN_MEDICINE:
                medicamentos.desocupar(evento.numUnidade);
                paciente->setStatus(HOSPITAL_DISCHARGED);
                break;
            default:
                break;
            }
        }

        // Processa as filas de acordo com os procedimentos
        processarFila(filaDeTriagem, triagem, IN_SCREENING, duracaoTriagem);
        processarMultiFila(filaDeAtendimento, atendimento, IN_MEDICAL_CONSULTATION, duracaoAtendimento);
        processarMultiFila(filaDeMedidasHospitalares, medidasHospitalares, IN_MEDICAL_TREATMENT, duracaoMedidasHospitalares);
        processarMultiFila(filaDeTestesDeLaboratorio, testesDeLaboratorio, IN_TEST, duracaoTestesDeLaboratorio);
        processarMultiFila(filaDeExamesDeImagem, examesDeImagem, IN_IMAGING_TEST, duracaoExamesDeImagem);
        processarMultiFila(filaDeMedicamentos, medicamentos, IN_MEDICINE, duracaoMedicamentos);
    }
}

// Processa uma fila de pacientes para um procedimento específico
void HospitalManager::processarFila(Fila &fila, Procedimento &procedimento, Status novoStatus, double duracaoBase)
{
    if (duracaoBase < 0)
    {
        throw std::invalid_argument("Duração base não pode ser negativa");
    }
    int unidadeLivre = procedimento.getUnidadeLivre();
    while (!fila.filaVazia() && unidadeLivre != -1)
    {
        Paciente *paciente = fila.desenfileira();
        double atraso = relogio - paciente->getTempoAtualEmHoras();
        paciente->addTempoEspera(atraso);
        procedimento.ocupar(unidadeLivre);
        paciente->setStatus(novoStatus);
        double duracao = duracaoBase * obterMultiplicadorDuracao(novoStatus, *paciente);
        Evento novoEvento(relogio + duracao, paciente, unidadeLivre);
        escalonador->inserirEvento(novoEvento);
        paciente->addTempoAtendimento(duracao);
        unidadeLivre = procedimento.getUnidadeLivre();
    }
}

// Processa múltiplas filas de pacientes para um procedimento específico
void HospitalManager::processarMultiFila(Fila fila[3], Procedimento &procedimento, Status novoStatus, double duracaoBase)
{
    if (duracaoBase < 0)
    {
        throw std::invalid_argument("Duração base não pode ser negativa");
    }
    int unidadeLivre = procedimento.getUnidadeLivre();
    while (unidadeLivre != -1)
    {
        Paciente *paciente = nullptr;
        if (!fila[2].filaVazia())
        {
            paciente = fila[2].desenfileira();
        }
        else if (!fila[1].filaVazia())
        {
            paciente = fila[1].desenfileira();
        }
        else if (!fila[0].filaVazia())
        {
            paciente = fila[0].desenfileira();
        }
        else
        {
            break;
        }
        if (paciente == nullptr)
            break;
        double atraso = relogio - paciente->getTempoAtualEmHoras();
        paciente->addTempoEspera(atraso);
        procedimento.ocupar(unidadeLivre);
        paciente->setStatus(novoStatus);
        double duracao = duracaoBase * obterMultiplicadorDuracao(novoStatus, *paciente);
        Evento novoEvento(relogio + duracao, paciente, unidadeLivre);
        escalonador->inserirEvento(novoEvento);
        paciente->addTempoAtendimento(duracao);
        unidadeLivre = procedimento.getUnidadeLivre();
    }
}

// Obtém o multiplicador de duração de um procedimento com base no status do paciente
double HospitalManager::obterMultiplicadorDuracao(Status status, Paciente paciente)
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

// Imprime os resultados do processamento dos pacientes
void HospitalManager::imprimirResultados()
{
    for (int i = 0; i < numPacientes; ++i)
    {
        double tempoTotal = pacientes[i].getTempoAtendimento() + pacientes[i].getTempoEspera();
        string dataDeChegadaESaida = paraDataDeChegadaESaida(pacientes[i].getTempoChegada(), tempoTotal);
        cout << fixed << setprecision(2) << pacientes[i].getId() << " " << dataDeChegadaESaida << "  " << tempoTotal << "  " << pacientes[i].getTempoAtendimento() << "  " << pacientes[i].getTempoEspera() << endl;
    }
}

// Lê os dados de entrada do arquivo e inicializa os pacientes e o escalonador
void HospitalManager::lerEntrada(string arquivo)
{
    relogio = 0;
    ifstream inputFile(arquivo);
    if (!inputFile.is_open())
    {
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
    escalonador = new Escalonador(numPacientes);
    for (int i = 0; i < numPacientes; ++i)
    {
        int id, alta, prioridade, ano, mes, dia, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos;
        double hora;
        inputFile >> id >> alta >> ano >> mes >> dia >> hora >> prioridade >> numMedidasHospitalares >> numTestesDeLaboratorio >> numExamesDeImagem >> numMedicamentos;
        pacientes[i] = Paciente(id, alta, prioridade, ano, mes, dia, hora, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos);
        Evento evento(pacientes[i].getTempoDeChegadaEmHoras(), &pacientes[i], -1);
        escalonador->inserirEvento(evento);
    }
    inputFile.close();
}

// Verifica se todas as filas estão vazias
bool HospitalManager::filasVazias()
{
    return filaDeTriagem.filaVazia() && filaDeAtendimento[0].filaVazia() && filaDeAtendimento[1].filaVazia() && filaDeAtendimento[2].filaVazia() &&
           filaDeMedidasHospitalares[0].filaVazia() && filaDeMedidasHospitalares[1].filaVazia() && filaDeMedidasHospitalares[2].filaVazia() &&
           filaDeTestesDeLaboratorio[0].filaVazia() && filaDeTestesDeLaboratorio[1].filaVazia() && filaDeTestesDeLaboratorio[2].filaVazia() &&
           filaDeExamesDeImagem[0].filaVazia() && filaDeExamesDeImagem[1].filaVazia() && filaDeExamesDeImagem[2].filaVazia() &&
           filaDeMedicamentos[0].filaVazia() && filaDeMedicamentos[1].filaVazia() && filaDeMedicamentos[2].filaVazia();
}
