#include <iostream>
#include <list>
#include <string>
#include "Grafo.h"
#include "Guloso.h"
//#include <chrono>

using namespace std;
//using namespace chrono;

int main(int argc, char *argv[])
{
    string arquivo = "grafo.txt";
    if(argc > 1)
        arquivo = string(argv[1]);

    cout << "Carregando " << arquivo << "..." << endl;
    Grafo a(arquivo);
    cout << "Número de vértices: " << a.getTam() << endl;
    //a.imprimir();

    cout << "Buscando solução..." << endl;
    Guloso guloso;
    SolucaoGuloso solucao;
    guloso.calcular(a, solucao);
    cout << "Solução" << endl;
    guloso.imprimir(solucao);
    return 0;
}
