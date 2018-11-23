#include <list>
#include <string>
#include <iostream>
#include "Grafo.h"
#include "Guloso.h"
#include <chrono>

using namespace std;
using namespace chrono;

__int64_t now() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void printExecutionTime(__int64_t start, __int64_t end) {
    cout << "Tempo de execução: " << ((double) (end - start)/1000000.0) << " seconds";
}

int main(int argc, char *argv[])
{
    string arquivo = "grafoSimples.txt";
    if(argc > 1)
        arquivo = string(argv[1]);

    cout << "Carregando " << arquivo << "..." << endl;
    Grafo grafo(arquivo);

    Guloso guloso;
    SolucaoGuloso solucao;

    int aux;
    int verticeA, verticeB;
    float peso;

    float alphaReativo[]
    {
        0.1, 0.2, 0.3, 0.4, 0.5
    };
    int nAlphas = 5;

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
        cin >> aux;
        cout << endl;
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
                cout << "--- Solucao Gulosa ----" << endl;
                guloso.imprimir(solucao);
                printExecutionTime(start, end);
                break;
            case 8:
                cout << "Buscando solução..." << endl;
                start = now();
                guloso.calcularRandomizado(grafo, solucao, 0.2, 1000);
                end = now();
                cout << "--- Solucao Gulosa randomizada ----" << endl;
                guloso.imprimir(solucao);
                printExecutionTime(start, end);
                break;
            case 9:
                cout << "Buscando solução..." << endl;
                start = now();
                guloso.calcularRandomizadoReativo(grafo, solucao, alphaReativo, nAlphas, 10, 2000);
                end = now();
                cout << "--- Solucao Gulosa randomizada reativa ----" << endl;
                guloso.imprimir(solucao);
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
    }while(aux != 0);
    return 0;
}
