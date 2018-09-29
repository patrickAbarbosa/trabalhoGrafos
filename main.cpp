#include <iostream>
#include <list>
#include "Grafo.h"

using namespace std;

int main()
{
    Grafo g;
    g.adicionarVertice(1, 10);
    g.adicionarVertice(2, 20);
    g.adicionarVertice(3, 20);
    g.adicionarAresta(1, 2, 100);
    g.adicionarAresta(1, 3, 100);
    g.adicionarAresta(3, 2, 100);
    g.imprimir();
    return 0;
}
