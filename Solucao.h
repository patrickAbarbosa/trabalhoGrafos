#ifndef SOLUCAO_H_
#define SOLUCAO_H_

#include <list>
#include "Grafo.h"

using namespace std;

typedef struct
{
    list<int> indices;
    list<Vertice*> vertices;
    list<Aresta*> arestas;
    float custo;
    float custoTotal;
    float alpha;
}SolucaoGuloso;

#endif
