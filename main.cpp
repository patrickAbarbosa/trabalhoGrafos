#include <iostream>
#include <list>
#include "Grafo.h"
#include "Guloso.h"
//#include <chrono>

using namespace std;
//using namespace chrono;

int main()
{
    //__int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    Grafo a("grafoSimples.txt");
    cout << "Tamanho da lista " << a.getTam() << endl;
    a.imprimir();

    Guloso guloso;
    SolucaoGuloso solucao;
    guloso.calcular(a, solucao);
    cout << "Solucao --------" << endl;
    guloso.imprimir(solucao);

    //__int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //cout << "It took" << start - end << "ms";
    return 0;
}
