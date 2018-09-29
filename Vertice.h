#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED
#include "Aresta.h"
#include <list>

using namespace std;

class Aresta;

class Vertice
{
public:
    Vertice();
    ~Vertice();
    void setInfo(int info);
    int getInfo();
    void setPeso(float Peso);
    float getPeso();

private:
    // informacao do vertice
    int info;
    float peso;
    // lista das arestas deste nó
    list<Aresta> arestas;
    //vetor de aresta
};

#endif // VERTICE_H_INCLUDED
