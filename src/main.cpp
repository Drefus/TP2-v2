#include "HospitalManager.hpp"

int main(int argc, char *argv[])
{
    // Verifica se o número de argumentos é menor que 2
    if (argc < 2)
    {
        // Exibe uma mensagem de uso correto do programa
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo>" << std::endl;
        return 1; // Retorna 1 para indicar erro
    }

    // Obtém o nome do arquivo a partir dos argumentos
    std::string filename = argv[1];
    HospitalManager manager;    // Cria uma instância de HospitalManager
    manager.executar(filename); // Executa o método 'executar' passando o nome do arquivo
    return 0;                   // Retorna 0 para indicar sucesso
}