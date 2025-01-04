#include <iostream>
#include <string>
#include "Fila.hpp"
#include "Paciente.hpp"
#include "Procedimento.hpp"
#include "Escalonador.hpp"

using namespace std;

int getPaciemteIndex(Paciente *pacientes, int numPacientes, int id)
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

int main()
{
    double relogio = 0;
    Fila filaDeTriagem;
    Fila filaDeAtendimento;
    Fila filaDeMedidasHospitalares;
    Fila filaDeTestesDeLaboratorio;
    Fila filaDeExamesDeImagem;
    Fila filaDeMedicamentos;

    Paciente *pacientes;
    int numPacientes;
    double duracaoTriagem;
    double duracaoAtendimento;
    double duracaoMedidasHospitalares;
    double duracaoTestesDeLaboratorio;
    double duracaoExamesDeImagem;
    double duracaoMedicamentos;
    int capacidadeTriagem;
    int capacidadeAtendimento;
    int capacidadeMedidasHospitalares;
    int capacidadeTestesDeLaboratorio;
    int capacidadeExamesDeImagem;
    int capacidadeMedicamentos;

    Escalonador escalonador;

    cin >> duracaoTriagem >> capacidadeTriagem;
    cin >> duracaoAtendimento >> capacidadeAtendimento;
    cin >> duracaoMedidasHospitalares >> capacidadeMedidasHospitalares;
    cin >> duracaoTestesDeLaboratorio >> capacidadeTestesDeLaboratorio;
    cin >> duracaoExamesDeImagem >> capacidadeExamesDeImagem;
    cin >> duracaoMedicamentos >> capacidadeMedicamentos;
    Procedimento triagem(capacidadeTriagem);
    Procedimento atendimento(capacidadeAtendimento);
    Procedimento medidasHospitalares(capacidadeMedidasHospitalares);
    Procedimento testesDeLaboratorio(capacidadeTestesDeLaboratorio);
    Procedimento examesDeImagem(capacidadeExamesDeImagem);
    Procedimento medicamentos(capacidadeMedicamentos);
    cin >> numPacientes;
    pacientes = new Paciente[numPacientes];
    for (int i = 0; i < numPacientes; ++i)
    {
        int id, alta, prioridade, ano, mes, dia, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos;
        double hora;
        cin >> id >> alta >> ano >> mes >> dia >> hora >> prioridade >> numMedidasHospitalares >> numTestesDeLaboratorio >> numExamesDeImagem >> numMedicamentos;
        cout << id << " " << alta << " " << prioridade << " " << ano << " " << mes << " " << dia << " " << hora << " " << numMedidasHospitalares << " " << numTestesDeLaboratorio << " " << numExamesDeImagem << " " << numMedicamentos << endl;
        pacientes[i] = Paciente(id, alta, prioridade, ano, mes, dia, hora, numMedidasHospitalares, numTestesDeLaboratorio, numExamesDeImagem, numMedicamentos);
        Evento evento(pacientes[i].getTempoDeChegadaEmHoras(), pacientes[i].getId(), -1);
        escalonador.insereEvento(evento);
    }

    while (!escalonador.vazio() || !filaDeTriagem.filaVazia() || !filaDeAtendimento.filaVazia() || !filaDeMedidasHospitalares.filaVazia() || !filaDeTestesDeLaboratorio.filaVazia() || !filaDeExamesDeImagem.filaVazia() || !filaDeMedicamentos.filaVazia())
    {

        if (!escalonador.vazio())
        {
            Evento evento = escalonador.retiraProximoEvento();

            relogio = evento.dataHora;

            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, evento.pacienteId);
            Paciente paciente = pacientes[pacienteIndex];
            paciente.addTempoAtual(relogio - paciente.getTempoDeAtualEmHoras());
            pacientes[pacienteIndex] = paciente;
            if (paciente.getStatus() == NOT_ARRIVED)
            {
                int unidadeLivre = triagem.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    triagem.ocupar(unidadeLivre);
                    paciente.setStatus(IN_SCREENING);
                    Evento novoEvento(relogio + duracaoTriagem, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.addTempoAtendimento(duracaoTriagem);
                }
                else
                {
                    paciente.setStatus(SCREENING_QUEUE);

                    pacientes[pacienteIndex] = paciente;
                    filaDeTriagem.enfileira(paciente);
                }
            }
            else if (paciente.getStatus() == IN_SCREENING)
            {
                if (evento.numUnit != -1)
                {
                    triagem.desocupar(evento.numUnit);
                }

                int unidadeLivre = atendimento.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    atendimento.ocupar(unidadeLivre);
                    paciente.setStatus(IN_MEDICAL_CONSULTATION);
                    Evento novoEvento(relogio + +duracaoAtendimento, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.addTempoAtendimento(duracaoAtendimento);
                }
                else
                {
                    paciente.setStatus(MEDICAL_CONSULTATION_QUEUE);

                    filaDeAtendimento.enfileira(paciente);
                }
                pacientes[pacienteIndex] = paciente;
            }
            else if (paciente.getStatus() == IN_MEDICAL_CONSULTATION)
            {
                if (evento.numUnit != -1)
                {
                    atendimento.desocupar(evento.numUnit);
                }

                int unidadeLivre = medidasHospitalares.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    double duracao = duracaoMedidasHospitalares * paciente.getNumMedidasHospitalares();
                    medidasHospitalares.ocupar(unidadeLivre);
                    paciente.setStatus(IN_MEDICAL_TREATMENT);
                    Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.addTempoAtendimento(duracao);
                }
                else
                {
                    paciente.setStatus(MEDICAL_TREATMENT_QUEUE);

                    filaDeMedidasHospitalares.enfileira(paciente);
                }
                pacientes[pacienteIndex] = paciente;
            }
            else if (paciente.getStatus() == IN_MEDICAL_TREATMENT)
            {
                if (evento.numUnit != -1)
                {
                    medidasHospitalares.desocupar(evento.numUnit);
                }

                int unidadeLivre = testesDeLaboratorio.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    double duracao = duracaoTestesDeLaboratorio * paciente.getNumTestesDeLaboratorio();
                    testesDeLaboratorio.ocupar(unidadeLivre);
                    paciente.setStatus(IN_TEST);
                    Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.addTempoAtendimento(duracao);
                }
                else
                {
                    paciente.setStatus(TEST_QUEUE);

                    filaDeTestesDeLaboratorio.enfileira(paciente);
                }
                pacientes[pacienteIndex] = paciente;
            }
            else if (paciente.getStatus() == IN_TEST)
            {
                if (evento.numUnit != -1)
                {
                    testesDeLaboratorio.desocupar(evento.numUnit);
                }

                int unidadeLivre = examesDeImagem.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    double duracao = duracaoExamesDeImagem * paciente.getNumExamesDeImagem();
                    examesDeImagem.ocupar(unidadeLivre);
                    paciente.setStatus(IN_IMAGING_TEST);
                    Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.setTempoAtendimento(duracao + paciente.getTempoAtendimento());
                    paciente.addTempoAtual(duracao);
                }
                else
                {
                    paciente.setStatus(IMAGING_TEST_QUEUE);

                    filaDeExamesDeImagem.enfileira(paciente);
                }
                pacientes[pacienteIndex] = paciente;
            }
            else if (paciente.getStatus() == IN_IMAGING_TEST || paciente.getStatus() == MEDICINE_QUEUE)
            {
                if (evento.numUnit != -1)
                {
                    examesDeImagem.desocupar(evento.numUnit);
                }

                int unidadeLivre = medicamentos.getUnidadeLivre();
                if (unidadeLivre != -1)
                {
                    double duracao = duracaoMedicamentos * paciente.getNumMedicamentos();
                    medicamentos.ocupar(unidadeLivre);
                    paciente.setStatus(IN_MEDICINE);
                    Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivre);
                    escalonador.insereEvento(novoEvento);
                    paciente.addTempoAtendimento(duracao);
                }
                else
                {
                    paciente.setStatus(MEDICINE_QUEUE);

                    filaDeMedicamentos.enfileira(paciente);
                }
                pacientes[pacienteIndex] = paciente;
            }
            else if (paciente.getStatus() == IN_MEDICINE)
            {
                medicamentos.desocupar(evento.numUnit);
                paciente.setStatus(HOSPITAL_DISCHARGED);
                pacientes[pacienteIndex] = paciente;
            }

            if (paciente.getStatus() == IN_MEDICAL_CONSULTATION && paciente.getAlta() == 1)
            {
                paciente.setStatus(HOSPITAL_DISCHARGED);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    atendimento.desocupar(evento.numUnit);
                }
            }
            if (paciente.getStatus() == IN_MEDICAL_CONSULTATION && paciente.getNumMedidasHospitalares() == 0)
            {
                paciente.setStatus(IN_MEDICAL_TREATMENT);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    atendimento.desocupar(evento.numUnit);
                }
            }

            if (paciente.getStatus() == IN_MEDICAL_TREATMENT && paciente.getNumTestesDeLaboratorio() == 0)
            {
                paciente.setStatus(IN_TEST);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    atendimento.desocupar(evento.numUnit);
                }
            }
            if (paciente.getStatus() == IN_TEST && paciente.getNumTestesDeLaboratorio() == 0)
            {
                paciente.setStatus(IN_IMAGING_TEST);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    medidasHospitalares.desocupar(evento.numUnit);
                }
            }
            if (paciente.getStatus() == IN_IMAGING_TEST && paciente.getNumExamesDeImagem() == 0)
            {
                paciente.setStatus(IN_MEDICINE);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    testesDeLaboratorio.desocupar(evento.numUnit);
                }
            }
            if (paciente.getStatus() == IN_MEDICINE && paciente.getNumExamesDeImagem() == 0)
            {
                paciente.setStatus(HOSPITAL_DISCHARGED);
                pacientes[pacienteIndex] = paciente;
                if (evento.numUnit != -1)
                {
                    examesDeImagem.desocupar(evento.numUnit);
                }
            }
        }

        int unidadeLivreTriagem = triagem.getUnidadeLivre();
        while (!filaDeTriagem.filaVazia() && unidadeLivreTriagem != -1)
        {

            Paciente paciente = filaDeTriagem.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            triagem.ocupar(unidadeLivreTriagem);
            paciente.setStatus(IN_SCREENING);
            Evento novoEvento(relogio + duracaoTriagem, paciente.getId(), unidadeLivreTriagem);
            escalonador.insereEvento(novoEvento);
            paciente.addTempoAtendimento(duracaoTriagem);
            pacientes[pacienteIndex] = paciente;
            unidadeLivreTriagem = triagem.getUnidadeLivre();
        }

        int unidadeLivreAtendimento = atendimento.getUnidadeLivre();
        while (!filaDeAtendimento.filaVazia() && unidadeLivreAtendimento != -1)
        {
            Paciente paciente = filaDeAtendimento.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            atendimento.ocupar(unidadeLivreAtendimento);
            paciente.setStatus(IN_MEDICAL_CONSULTATION);
            Evento novoEvento(relogio + duracaoAtendimento, paciente.getId(), unidadeLivreAtendimento);
            escalonador.insereEvento(novoEvento);
            paciente.addTempoAtendimento(duracaoAtendimento);
            pacientes[pacienteIndex] = paciente;
            unidadeLivreAtendimento = atendimento.getUnidadeLivre();
        }

        int unidadeLivreMedidasHospitalares = medidasHospitalares.getUnidadeLivre();
        while (!filaDeMedidasHospitalares.filaVazia() && unidadeLivreMedidasHospitalares != -1)
        {
            Paciente paciente = filaDeMedidasHospitalares.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            medidasHospitalares.ocupar(unidadeLivreMedidasHospitalares);
            paciente.setStatus(IN_MEDICAL_TREATMENT);
            double duracao = duracaoMedidasHospitalares * paciente.getNumMedidasHospitalares();
            Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivreMedidasHospitalares);
            escalonador.insereEvento(novoEvento);
            paciente.addTempoAtendimento(duracao);
            pacientes[pacienteIndex] = paciente;
            unidadeLivreMedidasHospitalares = medidasHospitalares.getUnidadeLivre();
        }

        int unidadeLivreTestesDeLaboratorio = testesDeLaboratorio.getUnidadeLivre();
        while (!filaDeTestesDeLaboratorio.filaVazia() && unidadeLivreTestesDeLaboratorio != -1)
        {
            Paciente paciente = filaDeTestesDeLaboratorio.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            testesDeLaboratorio.ocupar(unidadeLivreTestesDeLaboratorio);
            paciente.setStatus(IN_TEST);
            double duracao = duracaoTestesDeLaboratorio * paciente.getNumTestesDeLaboratorio();
            Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivreTestesDeLaboratorio);
            escalonador.insereEvento(novoEvento);
            paciente.addTempoAtendimento(duracao);
            pacientes[pacienteIndex] = paciente;
            unidadeLivreTestesDeLaboratorio = testesDeLaboratorio.getUnidadeLivre();
        }

        int unidadeLivreExamesDeImagem = examesDeImagem.getUnidadeLivre();
        while (!filaDeExamesDeImagem.filaVazia() && unidadeLivreExamesDeImagem != -1)
        {
            Paciente paciente = filaDeExamesDeImagem.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            examesDeImagem.ocupar(unidadeLivreExamesDeImagem);
            paciente.setStatus(IN_IMAGING_TEST);
            double duracao = duracaoExamesDeImagem * paciente.getNumExamesDeImagem();
            Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivreExamesDeImagem);
            escalonador.insereEvento(novoEvento);
            paciente.setTempoAtendimento(duracao + paciente.getTempoAtendimento());
            pacientes[pacienteIndex] = paciente;
            unidadeLivreExamesDeImagem = examesDeImagem.getUnidadeLivre();
        }

        int unidadeLivreMedicamentos = medicamentos.getUnidadeLivre();
        while (!filaDeMedicamentos.filaVazia() && unidadeLivreMedicamentos != -1)
        {
            Paciente paciente = filaDeMedicamentos.desenfileira();
            double atraso = relogio - paciente.getTempoDeAtualEmHoras();
            paciente.addTempoEspera(atraso);
            int pacienteIndex = getPaciemteIndex(pacientes, numPacientes, paciente.getId());
            medicamentos.ocupar(unidadeLivreMedicamentos);
            paciente.setStatus(IN_MEDICINE);
            double duracao = duracaoMedicamentos * paciente.getNumMedicamentos();
            Evento novoEvento(relogio + duracao, paciente.getId(), unidadeLivreMedicamentos);
            escalonador.insereEvento(novoEvento);
            paciente.addTempoAtendimento(duracao);
            pacientes[pacienteIndex] = paciente;
            unidadeLivreMedicamentos = medicamentos.getUnidadeLivre();
        }
    }

    for (int i = 0; i < numPacientes; ++i)
    {
        double tempoTotal = pacientes[i].getTempoAtendimento() + pacientes[i].getTempoEspera();
        string dataDeEntrada = to_string(pacientes[i].getTempoChegada().year) + " " + to_string(pacientes[i].getTempoChegada().month) + " " + to_string(pacientes[i].getTempoChegada().day) + " " + to_string(pacientes[i].getTempoChegada().hour);
        string dataDeSaida = to_string(pacientes[i].getTempoAtual().year) + " " + to_string(pacientes[i].getTempoAtual().month) + " " + to_string(pacientes[i].getTempoAtual().day) + " " + to_string(pacientes[i].getTempoAtual().hour);
        cout << pacientes[i].getId() << " " << dataDeEntrada << " " << dataDeSaida << " " << tempoTotal << " " << pacientes[i].getTempoAtendimento() << " " << pacientes[i].getTempoEspera() << endl;
    }

    delete[] pacientes;
    return 0;
}