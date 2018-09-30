#include <iostream>
#include <list>
#include "Grafo.h"
#include "Guloso.h"

using namespace std;

int main()
{
    Grafo a("grafo.txt");
    cout << "Tamanho da lista " << a.getTam() << endl;
    a.imprimir();
    Guloso guloso;
    list<int> solucao = guloso.calcular(a);
    cout << "Solucao" << endl;
    for (list<int>::iterator it = solucao.begin() ; it != solucao.end(); ++it)
    {
        cout << (*it) << ", ";
    }

    return 0;
}
