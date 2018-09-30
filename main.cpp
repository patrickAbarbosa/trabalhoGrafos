#include <iostream>
#include <list>
#include "Grafo.h"
#include "Guloso.h"
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{

    __int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    Grafo a("grafoA.txt");

    //__int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


    //Grafo a("grafo.txt");
    cout << "Tamanho da lista " << a.getTam() << endl;
    a.imprimir();

    Guloso guloso;
    list<int> solucao = guloso.calcular(a);
    cout << "Solucao" << endl;
    for (list<int>::iterator it = solucao.begin() ; it != solucao.end(); ++it)
    {
        cout << (*it) << ", ";
    }

    __int64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    cout << "It took" << start - end << "ms";
    return 0;
}
