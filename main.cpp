#include <iostream>
#include <list>
#include <string>
#include "Grafo.h"
#include "Guloso.h"
#include "GulosoRandomizado.h"
//#include <chrono>

using namespace std;
//using namespace chrono;

int main(int argc, char *argv[])
{
    string arquivo = "grafo.txt";
    if(argc > 1)
        arquivo = string(argv[1]);

    cout << "Carregando " << arquivo << "..." << endl;
    Grafo grafo(arquivo);

    Guloso guloso;
    GulosoRandomizado gulosoRandomizado;
    SolucaoGuloso solucao;

    int aux;
    int verticeA, verticeB;
    float peso;

    do
    {
        cout << "--------------- MENU ---------------"<< endl;
        cout << "[0]  - Adicionar vertice." << endl;
        cout << "[1]  - Remover vertice." << endl;
        cout << "[2]  - Adicionar aresta." << endl;
        cout << "[3]  - Remover aresta." << endl;
        cout << "[4]  - Imprimir Grafo." << endl;
        cout << "[5]  - Melhor solucao Gulosa" << endl;
        cout << "[6]  - Melhor solucao Gulosa Randomizada" << endl;
        cout << "[7]  - Numero de vértices" << endl;
        cout << "[-1] - Encerrar" << endl;
        cin >> aux;
        cout << endl;
        switch(aux)
        {
            case 0:
                cout << "-- Adicionar Vertice" << endl;
                cout << "Digite vertice e seu peso: ";
                cin >> verticeA >> peso;
                cout << endl;
                grafo.adicionarVertice(verticeA, peso);
                break;
            case 1:
                cout << "-- Remover Vertice" <<endl;
                cout << "Digite o vertice: ";
                cin >> verticeA;
                grafo.removerVertice(verticeA);
                break;
            case 2:
                cout << "-- Adicionar aresta ---" << endl;
                cout << "Digite o vertice A, o vertice B e o peso da aresta ";
                cin >> verticeA >> verticeB >>  peso;
                grafo.adicionarAresta(verticeA, verticeB, peso);
                break;
            case 3:
                cout << "-- Adicionar aresta ---" << endl;
                cout << "Digite o vertice A e o vertice B";
                cin >> verticeA >> verticeB;
                grafo.removerAresta(verticeA, verticeB);
                break;
            case 4:
                grafo.imprimir();
                break;
            case 5:
                cout << "Buscando solução..." << endl;
                guloso.calcular(grafo, solucao);
                cout << "--- Solucao Gulosa ----" << endl;
                guloso.imprimir(solucao);
                break;
            case 6:
                cout << "Buscando solução..." << endl;
                gulosoRandomizado.calcular(grafo, solucao, 0.1, 100);
                cout << "--- Solucao Gulosa ----" << endl;
                guloso.imprimir(solucao);
                break;
            case 7:
                cout <<"Quantidade de Vertices = " << grafo.getTam() << endl;
                break;
            case -1:
                break;
            default:
                cout << "Opção inválida" << endl;
                break;
        }
        cout << endl << endl;
    }while(aux != -1);
    return 0;
}
