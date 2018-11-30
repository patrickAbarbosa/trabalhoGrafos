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

void now(struct timespec &t)
{
    clock_gettime( CLOCK_REALTIME, &t);
}

double printExecutionTime(struct timespec start, struct timespec stop)
{
    double secs = ( stop.tv_sec - start.tv_sec )
          + ( ((double) stop.tv_nsec - start.tv_nsec )
            / (double)1000000000L);
    cout << "Tempo de execução: " << secs << " seconds";
    return secs;
}

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

    bool loadIn = false, loadOut = false;
    for (int i = optind; i < argc; i++)
    {
        if(loadIn == false)
        {
            // Arquivo de entrada
            arquivo = string(argv[i]);
            loadIn = true;
        }
        else if(loadOut == false)
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
    if(saida.size() > 0)
    {
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
    }

    //cout << "Carregando " << arquivo << "..." << endl;
    Grafo grafo(arquivo);

    Guloso guloso;
    SolucaoGuloso solucao;

    int verticeA, verticeB, K, T;
    float peso, D;
    double secs;
    list<int> resultados;

    float alphaReativo[]
    {
        0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50
    };
    int nAlphas = 10;
    int i = 0;

    do
    {
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
        cout << "[0] - Encerrar" << endl;
        if(direct)
        {
            cout << aux << endl;
            direct = false;
        }
        else
        {
            cin >> aux;
            cout << endl;
        }
        switch(aux)
        {
            case 1:
                cout << "-- Alterar tipo" << endl;
                cout << "1) Digrafo" << endl;
                cout << "2) Grafo" << endl;
                cout << "Tipo: ";
                cin >> T;
                cout << endl;
                grafo.carregar(arquivo, T == 1);
                solucao.indices.clear();
                solucao.vertices.clear();
                solucao.arestas.clear();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Tipo: " << (T == 1 ? "Digrafo" : "Grafo") << endl;
                }
                break;
            case 2:
                cout << "-- Adicionar Vertice" << endl;
                cout << "Digite vertice e seu peso: ";
                cin >> verticeA >> peso;
                cout << endl;
                grafo.adicionarVertice(verticeA, peso);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Adicionar vértice: " << verticeA << " ( " << peso << " )" << endl;
                }
                break;
            case 3:
                cout << "-- Remover Vertice" <<endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                grafo.removerVertice(verticeA);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Remover vértice: " << verticeA << endl;
                }
                break;
            case 4:
                cout << "-- Adicionar aresta" << endl;
                cout << "Digite o vertice A, o vertice B e o peso da aresta: ";
                cin >> verticeA >> verticeB >>  peso;
                grafo.adicionarAresta(verticeA, verticeB, peso);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Adicionar aresta: " << verticeA << " -> " << verticeB << " ( " << peso << " )" << endl;
                }
                break;
            case 5:
                cout << "-- Remover aresta" << endl;
                cout << "Digite o vertice A e o vertice B: ";
                cin >> verticeA >> verticeB;
                grafo.removerAresta(verticeA, verticeB);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Remover aresta: " << verticeA << " -> " << verticeB << endl;
                }
                break;
            case 6:
                grafo.imprimir();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Imprimir Grafo" << endl;
                }
                break;
            case 7:
                cout <<"Ordem = " << grafo.getTam() << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Ordem do Grafo: " << grafo.getTam() << endl;
                }
                break;
            case 8:
                cout << "-- Grau de um vértice" << endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                K = grafo.getGrau(verticeA);
                cout << "Grau: " << K << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grau de um vértice: " << verticeA << " ( " << K << " )" << endl;
                }
                break;
            case 9:
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
            case 10:
                cout << "-- Vizinhança aberta" << endl;
                cout << "Vértice: ";
                cin >> verticeA;
                grafo.vizinhanca(verticeA, false, resultados);
                if(arquivoLog.is_open())
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
            case 11:
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
            case 12:
                cout << "-- Completo" << endl;
                T = grafo.completo() ? 1 : 0;
                cout << (T == 1 ? "Sim" : "Não")  << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grafo completo: " << (T == 1 ? "Sim" : "Não") << endl;
                }
                break;
            case 13:
                cout << "-- Bipartido" << endl;
                T = grafo.bipartido() ? 1 : 0;
                cout << (T == 1 ? "Sim" : "Não")  << endl;
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Grafo bipartido: " << (T == 1 ? "Sim" : "Não") << endl;
                }
                break;
            case 14:
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
            case 15:
                if(grafo.ordenacaoTopologica(resultados))
                {
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
                    if(arquivoLog.is_open())
                    {
                        arquivoLog << "Ordenação topológica: Inválida" << endl;
                    }
                }
                break;
            case 16:
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
            case 17:
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
            case 18:
                grafo.kruskal();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "AGM por Kruskal" << endl;
                }
                break;
            case 19:
                grafo.prim();
                if(arquivoLog.is_open())
                {
                    arquivoLog << "AGM por Prim" << endl;
                }
                break;
            case 20:
                cout << "-- Caminho mínimo por Dijkstra" << endl;
                cout << "Vértice A: ";
                cin >> verticeA;
                cout << "Vértice B: ";
                cin >> verticeB;
                D = grafo.dijkstra(verticeA, verticeB);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Caminho mínimo por Dijkstra: " << D << endl;
                }
                break;
            case 21:
                cout << "-- Caminho mínimo por Floyd" << endl;
                cout << "Vértice A: ";
                cin >> verticeA;
                cout << "Vértice B: ";
                cin >> verticeB;
                D = grafo.floyd(verticeA, verticeB);
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Caminho mínimo por Floyd: " << D << endl;
                }
                break;
            case 22:
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução gulosa " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcular(grafo, solucao);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoCSV, "A1", arquivoNome, secs, solucao, grafo, best);
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso: " << D << " em " << secs << "s" << endl;
                }
                break;
            case 23:
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução gulosa randomizada " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcularRandomizado(grafo, solucao, alpha, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoCSV, "A2", arquivoNome, secs, solucao, grafo, best);
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso randomizado: " << D << " em " << secs << "s com alpha = " << alpha << endl;
                }
                break;
            case 24:
                D = FLT_MAX;
                secs = 0;
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução gulosa randomizada reativa " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcularRandomizadoReativo(grafo, solucao, alphaReativo, nAlphas, bloco, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada reativa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoCSV, "A3", arquivoNome, secs, solucao, grafo, best);
                    if(solucao.custo < D || i == 0)
                        D = solucao.custo;
                }
                if(arquivoLog.is_open())
                {
                    arquivoLog << "Algorítmo guloso randomizado reativo: " << D << " em " << secs << "s" << endl;
                }
                break;
            case 25:
                grafo.draw("grafo.png", &solucao);
                break;
            case 0:
                break;
            default:
                cout << "Opção inválida" << endl;
                break;
        }
        cout << endl << endl;
        if(autoExit)
            break;
    }while(aux != 0);
    if(arquivoLog.is_open())
        arquivoLog.close();
    return 0;
}
