#include <list>
#include <string>
#include <iostream>
#include <cstdlib>
#include "Grafo.h"
#include "Guloso.h"
#include <unistd.h>
#include <fstream>
#include <time.h>

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
    string saida = "";
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


    while ((opt = getopt(argc, argv, "n:b:a:o:i:r:s:l:e")) != -1)
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
            case 'i': // Arquivo de entrada
                arquivo = string(optarg);
                break;
            case 'o': // Opção no menu
                aux = atoi(optarg);
                direct = true;
                break;
            case 's': // Arquivo de saída
                saida = string(optarg);
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

    string arquivoNome = getFileName(arquivo);
    ofstream arquivoLog;

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
        string saidaNome = getFileName(saida);
        cout << "Saída: " << saidaNome << endl;
        ifstream infile(saida.c_str());
        bool exists = infile.good();
        infile.close();
        arquivoLog.open(saida.c_str(), ios::app);
        if(!exists)
        {
            arquivoLog << "ALG" << ", " << "INST" << ", " << "t[s]" << ", " << "alpha" << ", " << "custo" << ", " << "custo_total" << ", " << "percent_total" << ", " << "melhor_custo" << ", " << "relacao_melhor" << ", " << "iteracao_melhor" << ", " << "grafo_nos" << ", " << "solucao_nos" << ", " << "grafo_arestas" << ", " << "solucao_arestas" << endl;
        }
    }

    //cout << "Carregando " << arquivo << "..." << endl;
    Grafo grafo(arquivo);

    Guloso guloso;
    SolucaoGuloso solucao;

    int verticeA, verticeB;
    float peso;
    double secs;

    float alphaReativo[]
    {
        0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50
    };
    int nAlphas = 10;
    int i = 0;

    do
    {
        cout << "--------------- MENU ---------------"<< endl;
        cout << "[1]  - Adicionar vertice." << endl;
        cout << "[2]  - Remover vertice." << endl;
        cout << "[3]  - Adicionar aresta." << endl;
        cout << "[4]  - Remover aresta." << endl;
        cout << "[5]  - Imprimir Grafo." << endl;
        cout << "[6]  - Numero de vértices" << endl;
        cout << "[7]  - Melhor solucao Gulosa" << endl;
        cout << "[8]  - Melhor solucao Gulosa Randomizada" << endl;
        cout << "[9]  - Melhor solucao Gulosa Randomizada Reativa" << endl;
        cout << "[10] - Desenhar grafo" << endl;
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
                cout << "-- Adicionar Vertice" << endl;
                cout << "Digite vertice e seu peso: ";
                cin >> verticeA >> peso;
                cout << endl;
                grafo.adicionarVertice(verticeA, peso);
                break;
            case 2:
                cout << "-- Remover Vertice" <<endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                grafo.removerVertice(verticeA);
                break;
            case 3:
                cout << "-- Adicionar aresta ---" << endl;
                cout << "Digite o vertice A, o vertice B e o peso da aresta ";
                cin >> verticeA >> verticeB >>  peso;
                grafo.adicionarAresta(verticeA, verticeB, peso);
                break;
            case 4:
                cout << "-- Adicionar aresta ---" << endl;
                cout << "Digite o vertice A e o vertice B";
                cin >> verticeA >> verticeB;
                grafo.removerAresta(verticeA, verticeB);
                break;
            case 5:
                grafo.imprimir();
                break;
            case 6:
                cout <<"Quantidade de Vertices = " << grafo.getTam() << endl;
                break;
            case 7:
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcular(grafo, solucao);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoLog, "A1", arquivoNome, secs, solucao, grafo, best);
                }
                break;
            case 8:
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcularRandomizado(grafo, solucao, alpha, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoLog, "A2", arquivoNome, secs, solucao, grafo, best);
                }
                break;
            case 9:
                for(i = 0; i < loop; i++)
                {
                    cout << "Buscando solução " << (i+1) << "..." << endl;
                    now(start);
                    guloso.calcularRandomizadoReativo(grafo, solucao, alphaReativo, nAlphas, bloco, maxIte);
                    now(stop);
                    cout << endl << "--- Solucao Gulosa randomizada reativa ----" << endl;
                    cout << "Arquivo: " << arquivoNome << endl;
                    guloso.imprimir(solucao, grafo, best);
                    secs = printExecutionTime(start, stop);
                    log(arquivoLog, "A3", arquivoNome, secs, solucao, grafo, best);
                }
                break;
            case 10:
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
