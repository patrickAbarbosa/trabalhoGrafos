#include <iostream>
#include <list>
#include "Grafo.h"
#include "Guloso.h"
//#include <chrono>

using namespace std;
//using namespace chrono;

void menu(Grafo *grafo, Guloso *guloso)
{

}

int main()
{
    //__int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    Grafo grafo("grafoA.txt");
    Guloso guloso;
    SolucaoGuloso solucao;


    int aux;
    int verticeA, verticeB;
    float peso;

    do{
        cout << "--------------- MENU ---------------"<< endl;
        cout << "[0]  3- Adicionar vertice." << endl;
        cout << "[1]  - Remover vertice." << endl;
        cout << "[2]  - Adicionar aresta." << endl;
        cout << "[3]  - Remover aresta." << endl;
        cout << "[4]  - Imprimir Grafo." << endl;
        cout << "[5]  - Melhor solucao Gulosa" << endl;
        cout << "[6]  - Numero de grafos" << endl;
        cout << "[-1] - Encerrar" << endl;
        cin >> aux;
        cout << endl;
        switch(aux){
        case 0:
            cout << "-- Adicionar Vertice" << endl;
            cout << "Digite vertice e seu peso: ";
            cin >> verticeA >> peso;
            cout << endl;
            grafo.adicionarVertice(verticeA, peso);

        case 1:
            cout << "-- Remover Vertice" <<endl;
            cout << "Digite o vertice: ";
            cin >> verticeA;
            grafo.removerVertice(verticeA);

        case 2:
            cout << "-- Adicionar aresta ---" << endl;
            cout << "Digite o vertice A, o vertice B e o peso da aresta ";
            cin >> verticeA >> verticeB >>  peso;
            grafo.adicionarAresta(verticeA, verticeB, peso);

        case 3:
            cout << "-- Adicionar aresta ---" << endl;
            cout << "Digite o vertice A e o vertice B";
            cin >> verticeA >> verticeB;
            grafo.removerAresta(verticeA, verticeB);

        case 4:
            grafo.imprimir();

        case 5:
            guloso.calcular(grafo, solucao);
            cout << "--- Solucao Gulosa ----" << endl;
            guloso.imprimir(solucao);
        case 6:
            cout <<"Quantidade de Vertices = " << grafo.getTam() << endl;
        }
        cout << endl << endl;;
    }while(aux != -1);

    //__int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //cout << "It took" << start - end << "ms";
    return 0;
}
