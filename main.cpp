#include <list>
#include <string>
#include <iostream>
#include <cstdlib>
#include "Grafo.h"
#include "Guloso.h"
#include <unistd.h>
#include <fstream>
#include <time.h>
#include <cfloat>

using namespace std;

// Esta função armazena o tempo atual
void now(struct timespec &t)
{
    clock_gettime( CLOCK_REALTIME, &t);
}

// Calculamos a diferença entre dois tempos e exibimos em segundos
double printExecutionTime(struct timespec start, struct timespec stop)
{
    double secs = ( stop.tv_sec - start.tv_sec )
          + ( ((double) stop.tv_nsec - start.tv_nsec )
            / (double)1000000000L);
    cout << "Tempo de execução: " << secs << " seconds";
    return secs;
}

// Dado o caminho de um arquivo, retornamos apenas o nome
string getFileName(const string& s) {

   char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos)
   {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}

// Coloca o resultado de uma solução gulosa em um arquivo csv
void log(ofstream &file, string algoritimo, string instancia, double secs, SolucaoGuloso &solucao, Grafo &grafo, float best = -1)
{
    if(!file.is_open())
        return;
    float ptotal = ((solucao.custo/solucao.custoTotal)*100.0f);
    float percent = 0;
    if(best > 0)
    {
        percent = ((solucao.custo - best)/best)*100.0f;
    }
    file << algoritimo << ", " << instancia << ", " << secs << ", " << solucao.alpha << ", " << solucao.custo << ", " << solucao.custoTotal << ", " << ptotal << "%, " << best << ", " << percent << "%, " << solucao.iteracao << ", " << grafo.getTam() << ", " << solucao.indices.size() << ", " << grafo.getArestas() << ", " << solucao.arestas.size() << endl;
}

int main(int argc, char *argv[])
{
    string arquivo = "grafoSimples.txt";
    string saida = "output.txt";
    float alpha = 0.1;
    int maxIte = 2000;
    int bloco = 100;
    int opt;
    int aux;
    bool direct = false;
    bool autoExit = false;
    float best = -1;
    int loop = 1;
    struct timespec start, stop;


    // Analisamos os argumenos passados ao programa
    while ((opt = getopt(argc, argv, "n:b:a:m:r:l:e")) != -1)
    {
        switch (opt)
        {
            case 'n': // Número máximo de iterações
                maxIte = atoi(optarg);
                break;
            case 'b': // Tamanho do bloco
                bloco = atoi(optarg);
                break;
            case 'a': // Alpha
                alpha = atof(optarg);
                break;
            case 'r': // Referência, melhor solução
                best = atof(optarg);
                break;
            case 'm': // Opção no menu
                aux = atoi(optarg);
                direct = true;
                break;
            case 'l': // Quantidade de execuções
                loop = atoi(optarg);
                break;
            case 'e': // Sair ao executar algoritimo
                autoExit = true;
                break;
            default:
                fprintf(stderr, "Uso: %s -o opcaoMenu -s arquivoSaida -e sairAutomaticamente -e quantidadeExecucções -r Referência -n nIterações -a alpha -b bloco -i arquivoEntrada\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Verificamos se além dos parâmetros, também foi passado o nome do arquivo de entrada e de saída
    bool loadIn = false, loadOut = false;
    for (int i = optind; i < argc; i++)
    {
        if(loadIn == false) // O primeiro argumento que não é parâmetro é considerado como o nome do arquivo de entrada
        {
            // Arquivo de entrada
            arquivo = string(argv[i]);
            loadIn = true;
        }
        else if(loadOut == false) // O segundo argumento que não é parâmetro é considerado como o nome do arquivo de saída
        {
            // Arquivo de saída
            saida = string(argv[i]);
            loadOut = true;
        }
        else
            break;
    }

    string arquivoNome = getFileName(arquivo);
    ofstream arquivoLog, arquivoCSV;

    // Exibimos as informações de execução do programa
    cout << "Entrada: " << arquivoNome << endl;
    cout << "Iterações: " << maxIte << endl;
    cout << "Bloco: " << bloco << endl;
    cout << "Loop: " << loop << endl;
    cout << "Alpha: " << alpha << endl;
    cout << "Best: " << best << endl;
    if(direct)
        cout << "Menu: " << aux << endl;
    if(autoExit)
        cout << "AutoExit: ON" << endl;
    if(saida.size() > 0) // Se existe um arquivo de saída, abrimos este arquivo
    {
        // Abrios dois arquivos de saída, um de log e um csv
        // O arquivo de log armazena a saída para qualquer item do meno
        // O arquivo csv armazena as soluções gulosas encontradas
        string saidaCSV = saida+".csv";
        string saidaNome = getFileName(saida);
        string saidaCSVNome = getFileName(saidaCSV);
        cout << "Saída: " << saidaNome << endl;
        cout << "Saída CSV: " << saidaCSV << endl;
        ifstream infile(saidaCSV.c_str());
        bool exists = infile.good();
        infile.close();
        arquivoCSV.open(saidaCSV.c_str(), ios::app);
        if(!exists)
        {
            arquivoCSV << "ALG" << ", " << "INST" << ", " << "t[s]" << ", " << "alpha" << ", " << "custo" << ", " << "custo_total" << ", " << "percent_total" << ", " << "melhor_custo" << ", " << "relacao_melhor" << ", " << "iteracao_melhor" << ", " << "grafo_nos" << ", " << "solucao_nos" << ", " << "grafo_arestas" << ", " << "solucao_arestas" << endl;
        }
        arquivoLog.open(saida.c_str(), ios::app);
        arquivoLog << endl << "Carregando: " << saidaNome << endl;
    }

    // Criamos um grafo e carregamos o arquivo de entrada
    Grafo grafo(arquivo);

    // Criamos uma instância da classe para resolver o problema de Steiner com coleta
    Guloso guloso;
    // Criamos um solução vazia
    SolucaoGuloso solucao;

    int verticeA, verticeB, K, T, gIn, gOut;
    float peso, D;
    string grafoArquivoSaida;
    double secs;
    list<int> resultados;

    float alphaReativo[]
    {
        0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50
    };
    int nAlphas = 10;
    int i = 0;

    // Executamos o menu em um loop
    do
    {
        // Exibimos o menu
        cout << "--------------- MENU ---------------"<< endl;
        cout << "[1]  - Alterar entre grafo/digrafo" << endl;
        cout << "[2]  - Adicionar vertice." << endl;
        cout << "[3]  - Remover vertice." << endl;
        cout << "[4]  - Adicionar aresta." << endl;
        cout << "[5]  - Remover aresta." << endl;
        cout << "[6]  - Imprimir Grafo." << endl;
        cout << "[7]  - Ordem do grafo" << endl;
        cout << "[8]  - Grau de um vértice" << endl;
        cout << "[9]  - Verificar grafo K-regular" << endl;
        cout << "[10] - Vizinhança aberta" << endl;
        cout << "[11] - Vizinhança fechada" << endl;
        cout << "[12] - Completo" << endl;
        cout << "[13] - Bipartido" << endl;
        cout << "[14] - Sequência de graus" << endl;
        cout << "[15] - Ordenação topológica" << endl;
        cout << "[16] - Fecho transitivo direto" << endl;
        cout << "[17] - Fecho transitivo inverso" << endl;
        cout << "[18] - Árvore geradora mínima por Kruskal" << endl;
        cout << "[19] - Árvore geradora mínima por Prim" << endl;
        cout << "[20] - Caminho mínimo por Dijkstra" << endl;
        cout << "[21] - Caminho mínimo por Floyd" << endl;
        cout << "[22] - Melhor solucao Gulosa" << endl;
        cout << "[23] - Melhor solucao Gulosa Randomizada" << endl;
        cout << "[24] - Melhor solucao Gulosa Randomizada Reativa" << endl;
        cout << "[25] - Desenhar grafo" << endl;
        cout << "[26] - Salvar grafo" << endl;
        cout << "[0] - Encerrar" << endl;
        // Se a opção do menu foi passada por parÂmetro, executa esta opção
        if(direct)
        {
            cout << aux << endl;
            direct = false;
        }
        else // Se nenhuma opção foi passada por parâmetro, espera o usuário digitar uma opção
        {
            cout << "Opção: ";
            cin >> aux;
            cout << endl;
        }
        // Verifica a opção escolhida
        switch(aux)
        {
            case 1: // Se o usuário deseja alterar o tipo de grafo, solicita a escolha do tipo
                cout << "-- Alterar tipo" << endl;
                cout << "1) Digrafo" << endl;
                cout << "2) Grafo" << endl;
                cout << "Tipo: ";
                cin >> T;
                cout << endl;
                // Para mudar o tipo do grafo, recarregamos-o passando a opção de tipo solicitada
                grafo.carregar(arquivo, T == 1);
                // Como limpamos o grafo, devemos limpar a solução que dependia dele
                solucao.indices.clear();
                solucao.vertices.clear();
                solucao.arestas.clear();
                if(arquivoLog.is_open()) // logamos para o arquivo
                {
                    arquivoLog << "Tipo: " << (T == 1 ? "Digrafo" : "Grafo") << endl;
                }
                break;
            case 2: // Se o usuário deseja adicionar um vértice, aguardamos a entrada de um vértice e um peso
                cout << "-- Adicionar Vertice" << endl;
                cout << "Digite vertice e seu peso: ";
                cin >> verticeA >> peso;
                cout << endl;
                // Adicionamos um vértice com peso no grafo
                grafo.adicionarVertice(verticeA, peso);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Adicionar vértice: " << verticeA << " ( " << peso << " )" << endl;
                }
                break;
            case 3: // Se o usuário deseja remover um vértice, aguardamos a entrada de um vértice
                cout << "-- Remover Vertice" <<endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                grafo.removerVertice(verticeA);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Remover vértice: " << verticeA << endl;
                }
                break;
            case 4: // Se o usuário deseja adicionar uma aresta, aguardamos a entrada de dois vértices
                cout << "-- Adicionar aresta" << endl;
                cout << "Digite o vertice A, o vertice B e o peso da aresta: ";
                cin >> verticeA >> verticeB >>  peso;
                grafo.adicionarAresta(verticeA, verticeB, peso);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Adicionar aresta: " << verticeA << " -> " << verticeB << " ( " << peso << " )" << endl;
                }
                break;
            case 5: // Se o usuário deseja remover uma aresta, aguardamos a entrada de dois vértices
                cout << "-- Remover aresta" << endl;
                cout << "Digite o vertice A e o vertice B: ";
                cin >> verticeA >> verticeB;
                grafo.removerAresta(verticeA, verticeB);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Remover aresta: " << verticeA << " -> " << verticeB << endl;
                }
                break;
            case 6:// Se o usuário deseja imprimir um grafo
                grafo.imprimir();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Imprimir Grafo" << endl;
                }
                break;
            case 7:// Se o usuário deseja visualizar a ordem do grafo
                cout <<"Ordem = " << grafo.getTam() << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Ordem do Grafo: " << grafo.getTam() << endl;
                }
                break;
            case 8:// Se o usuário deseja visualizar o grau de um vértice
                cout << "-- Grau de um vértice" << endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                // Se o usuário digitou um vértice inválido, informamos
                if(!grafo.getGrau(verticeA, &gIn, &gOut))
                    cout << "Vértice inválido" << endl;
                else
                {
                    // Se o grau de entrada retornado for negativo, este é um grafo normal e portanto exibimos apenas o grau geral do vértice
                    if(gIn < 0)
                    {
                        cout << "Grau: " << gOut << endl;
                        if(arquivoLog.is_open())
                        {
                            arquivoLog << "Grau de um vértice: " << verticeA << " ( " << gOut << " )" << endl;
                        }
                    }
                    else // Se o grau de entrada retornado for positivo, este é um digrafo e portanto exibimos o grau de entrada e o de saida
                    {
                        cout << "Grau de entrada: " << gIn << endl;
                        cout << "Grau de saída: " << gOut << endl;
                        if(arquivoLog.is_open())
                        {
                            arquivoLog << "Grau de um vértice: " << verticeA << " ( " << gIn << ", " << gOut << " )" << endl;
                        }
                    }
                }
                break;
            case 9: // Se o usuário deseja verificar a K-regularidade do grafo, aguardamos K
                cout << "-- K-regular" << endl;
                cout << "K: ";
                cin >> K;
                T = grafo.kRegular(K) ? 1 : 0;
                cout << K << "-regular? " << (T == 1 ? "Sim" : "Não")  << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grafo " << K << "-regular: " << (T == 1 ? "Sim" : "Não") << endl;
                }
                break;
            case 10: // Se o usuário deseja visualizar a vizinhança aberta de um vértice, aguardamos a entrada de um vértice
                cout << "-- Vizinhança aberta" << endl;
                cout << "Vértice: ";
                cin >> verticeA;
                grafo.vizinhanca(verticeA, false, resultados);
                if(arquivoLog.is_open()) // Colocamos a solução no log
                {
                    arquivoLog << "Vizinhança aberta: ";
                    while(resultados.size() > 0)
                    {
                        int r = resultados.front();
                        arquivoLog << r << ", ";
                        resultados.pop_front();
                    }
                    arquivoLog << endl;
                }
                break;
            case 11:// Se o usuário deseja visualizar a vizinhança fechada de um vértice, aguardamos a entrada de um vértice
                cout << "-- Vizinhança fechada" << endl;
                cout << "Vértice: ";
                cin >> verticeA;
                grafo.vizinhanca(verticeA, true, resultados);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Vizinhança fechada: ";
                    while(resultados.size() > 0)
                    {
                        int r = resultados.front();
                        arquivoLog << r << ", ";
                        resultados.pop_front();
                    }
                    arquivoLog << endl;
                }
                break;
            case 12:// Se o usuário deseja visualizar se o grafo é completo
                cout << "-- Completo" << endl;
                T = grafo.completo() ? 1 : 0;
                cout << (T == 1 ? "Sim" : "Não")  << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grafo completo: " << (T == 1 ? "Sim" : "Não") << endl;
                }
                break;
            case 13:// Se o usuário deseja visualizar se o grafo é bipartido
                cout << "-- Bipartido" << endl;
                T = grafo.bipartido() ? 1 : 0;
                cout << (T == 1 ? "Sim" : "Não")  << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grafo bipartido: " << (T == 1 ? "Sim" : "Não") << endl;
                }
                break;
            case 14:// Se o usuário deseja visualizar a sequencia de graus do grafo, obtemos e logamos
                grafo.sequenciaGraus(resultados);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Sequência de graus: ";
                    while(resultados.size() > 0)
                    {
                        int r = resultados.front();
                        arquivoLog << r << ", ";
                        resultados.pop_front();
                    }
                    arquivoLog << endl;
                }
                break;
            case 15:// Se o usuário deseja visualizar a ordenação topológica do grafo
                // Tentamos obter a ordenação
                if(grafo.ordenacaoTopologica(resultados))
                {
                    // Se for possível, logamos
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Ordenação topológica: ";
                        while(resultados.size() > 0)
                        {
                            int r = resultados.front();
                            arquivoLog << r << ", ";
                            resultados.pop_front();
                        }
                        arquivoLog << endl;
                    }
                }
                else
                {
                    // Se não foi possível obter a ordenação, logamos que é inválido
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Ordenação topológica: Inválida" << endl;
                    }
                }
                break;
            case 16:// Se o usuário deseja visualizar o fecho transitivo direto de um vértice, aguardamos a entrada de um vértice
                cout << "-- Fecho transitivo direto" << endl;
                cout << "Vértice: ";
                cin >> verticeA;
                if(grafo.fechoTransitivo(verticeA, true, resultados))
                {
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Fecho transitivo direto: ";
                        while(resultados.size() > 0)
                        {
                            int r = resultados.front();
                            arquivoLog << r << ", ";
                            resultados.pop_front();
                        }
                        arquivoLog << endl;
                    }
                }
                else
                {
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Fecho transitivo direto: Inválido" << endl;
                    }
                }
                break;
            case 17:// Se o usuário deseja visualizar o fecho transitivo indireto de um vértice, aguardamos a entrada de um vértice
                cout << "-- Fecho transitivo indireto" << endl;
                cout << "Vértice: ";
                cin >> verticeA;
                if(grafo.fechoTransitivo(verticeA, false, resultados))
                {
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Fecho transitivo direto: ";
                        while(resultados.size() > 0)
                        {
                            int r = resultados.front();
                            arquivoLog << r << ", ";
                            resultados.pop_front();
                        }
                        arquivoLog << endl;
                    }
                }
                else
                {
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Fecho transitivo direto: Inválido" << endl;
                    }
                }
                break;
            case 18:// Se o usuário deseja obter uma AMG por kruskal, executamos o algorítmo
                grafo.kruskal();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "AGM por Kruskal" << endl;
                }
                break;
            case 19:// Se o usuário deseja obter uma AMG por Prim, executamos o algorítmo
                grafo.prim();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "AGM por Prim" << endl;
                }
                break;
            case 20:// Se o usuário deseja obter a distância mínima por Dijkstra, aguardamos a entrada de dois vértices
                cout << "-- Caminho mínimo por Dijkstra" << endl;
                cout << "Vértice A: ";
                cin >> verticeA;
                cout << "Vértice B: ";
                cin >> verticeB;
                D = grafo.dijkstra(verticeA, verticeB);
                if(arquivoLog.is_open()) // Logamos a distância
                {
                    arquivoLog << "Caminho mínimo por Dijkstra: " << D << endl;
                }
                break;
            case 21:// Se o usuário deseja obter a distância mínima por Floyd, aguardamos a entrada de dois vértices
                cout << "-- Caminho mínimo por Floyd" << endl;
                cout << "Vértice A: ";
                cin >> verticeA;
                cout << "Vértice B: ";
                cin >> verticeB;
                D = grafo.floyd(verticeA, verticeB);
                if(arquivoLog.is_open()) // Logamos a distância
                {
                    arquivoLog << "Caminho mínimo por Floyd: " << D << endl;
                }
                break;
            case 22: // Se o usuário deseja obter a solução gulosa
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++) // Executamos o algorimo a quantidade de vezes solicitada
                {
                    cout << "Buscando solução gulosa " << (i+1) << "..." << endl;
                    // Medimos o tempo de execução
                    now(start);
                    guloso.calcular(grafo, solucao);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    // Imprimimos a solução encontrada
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    // Logamos para o arquivo CSV
                    log(arquivoCSV, "A1", arquivoNome, secs, solucao, grafo, best);
                    // Armazenamos o melhor custo
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                // Logamos o melhor custo
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso: " << D << " em " << secs << "s" << endl;
                }
                break;
            case 23: // Se o usuário deseja obter a solução gulosa randomizada
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++)// Executamos o algorimo a quantidade de vezes solicitada
                {
                    cout << "Buscando solução gulosa randomizada " << (i+1) << "..." << endl;
                    // Medimos o tempo de execução
                    now(start);
                    guloso.calcularRandomizado(grafo, solucao, alpha, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    // Imprimimos a solução encontrada
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    // Logamos para o arquivo CSV
                    log(arquivoCSV, "A2", arquivoNome, secs, solucao, grafo, best);
                    // Armazenamos o melhor custo
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                // Logamos o melhor custo
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso randomizado: " << D << " em " << secs << "s com alpha = " << alpha << endl;
                }
                break;
            case 24: // Se o usuário deseja obter a solução gulosa randomizada reativa
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++)// Executamos o algorimo a quantidade de vezes solicitada
                {
                    cout << "Buscando solução gulosa randomizada reativa " << (i+1) << "..." << endl;
                    // Medimos o tempo de execução
                    now(start);
                    guloso.calcularRandomizadoReativo(grafo, solucao, alphaReativo, nAlphas, bloco, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada reativa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    // Imprimimos a solução encontrada
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    // Logamos para o arquivo CSV
                    log(arquivoCSV, "A3", arquivoNome, secs, solucao, grafo, best);
                    // Armazenamos o melhor custo
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                // Logamos o melhor custo
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso randomizado reativo: " << D << " em " << secs << "s" << endl;
                }
                break;
            case 25:// Se o usuário deseja desenhar o grafo, desenhamos em um arquivo chamado grafo.png e com uma possível solução
                grafo.draw("grafo.png", &solucao);
                break;
            case 26: // Se o usuário deseja salvar o grafo atual, aguarda o nome do arquivo de saída
                cout << "-- Salvar grafo" <<endl;
                cout << "Digite o nome do arquivo a salvar: ";
                cin >> grafoArquivoSaida;
                grafo.salvar(grafoArquivoSaida);
                if(arquivoLog.is_open()) // Logamos que o grafo foi salvo
                {
                    arquivoLog << "Grafo salvo: " << grafoArquivoSaida << endl;
                }
            case 0: // Se o usuário desejar sair, para o loop do menu
                break;
            default: // Se o usuário digitar algo inválido, ignoramos
                cout << "Opção inválida" << endl;
                break;
        }
        cout << endl << endl;
        if(autoExit) // Se no menu foi passado um parâmetro para sair automaticamente, paramos o loop do menu
            break;
    }while(aux != 0);
    // Se há um arquivo de log aberto, fechamos
    if(arquivoLog.is_open())
        arquivoLog.close();
    return 0;
}
