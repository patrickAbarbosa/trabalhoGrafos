#include <list>
#include <string>
#include <iostream>
#include <cstdlib>
#include "Grafo.h"
#include "Guloso.h"
#include <chrono>
#include <unistd.h>

using namespace std;
using namespace chrono;

__int64_t now() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void printExecutionTime(__int64_t start, __int64_t end) {
    cout << "Tempo de execução: " << ((double) (end - start)/1000000.0) << " seconds";
}

string getFileName(const string& s) {

   char sep = '/';

#ifdef _WIN32
   sep = '\\';
#endif

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}

int main(int argc, char *argv[])
{
    string arquivo = "grafoSimples.txt";
    float alpha = 0.1;
    int maxIte = 2000;
    int bloco = 100;
    int opt;
    int aux;
    bool direct = false;
    bool autoExit = false;
    float best = -1;


    while ((opt = getopt(argc, argv, "n:b:a:o:i:r:e")) != -1)
    {
        switch (opt)
        {
            case 'n':
                maxIte = atoi(optarg);
                break;
            case 'b':
                bloco = atoi(optarg);
                break;
            case 'a':
                alpha = atof(optarg);
                break;
            case 'r':
                best = atof(optarg);
                break;
            case 'i':
                arquivo = string(optarg);
                break;
            case 'o':
                aux = atoi(optarg);
                direct = true;
                break;
            case 'e':
                autoExit = true;
                break;
            default:
                fprintf(stderr, "Uso: %s -n nIterações -a alpha -b bloco -i arquivo\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    string arquivoNome = getFileName(arquivo);

    cout << "Arquivo: " << arquivoNome << endl;
    cout << "Iterações: " << maxIte << endl;
    cout << "Bloco: " << bloco << endl;
    cout << "Alpha: " << alpha << endl;
    cout << "Best: " << best << endl;
    if(direct)
        cout << "Menu: " << aux << endl;
    if(autoExit)
        cout << "AutoExit: ON" << endl;

    //cout << "Carregando " << arquivo << "..." << endl;
    Grafo grafo(arquivo);

    Guloso guloso;
    SolucaoGuloso solucao;

    int verticeA, verticeB;
    float peso;

    float alphaReativo[]
    {
        0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50
    };
    int nAlphas = 10;

    do
    {
        __int64_t start = 0, end = 0;
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
        cout << "[10]  - Desenhar grafo" << endl;
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
                cout << "Buscando solução..." << endl;
                start = now();
                guloso.calcular(grafo, solucao);
                end = now();
                cout << endl << "--- Solucao Gulosa ----" << endl;
                cout << "Arquivo: " << arquivoNome << endl;
                guloso.imprimir(solucao, best);
                printExecutionTime(start, end);
                break;
            case 8:
                cout << "Buscando solução..." << endl;
                start = now();
                guloso.calcularRandomizado(grafo, solucao, alpha, maxIte);
                end = now();
                cout << endl << "--- Solucao Gulosa randomizada ----" << endl;
                cout << "Arquivo: " << arquivoNome << endl;
                guloso.imprimir(solucao, best);
                printExecutionTime(start, end);
                break;
            case 9:
                cout << "Buscando solução..." << endl;
                start = now();
                guloso.calcularRandomizadoReativo(grafo, solucao, alphaReativo, nAlphas, bloco, maxIte);
                end = now();
                cout << endl << "--- Solucao Gulosa randomizada reativa ----" << endl;
                cout << "Arquivo: " << arquivoNome << endl;
                guloso.imprimir(solucao, best);
                printExecutionTime(start, end);
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
    return 0;
}
