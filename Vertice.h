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
    void adicionarAresta(Vertice *extremidade, float peso);
    void removerAresta(Vertice *extremidade);
    void removerAresta(int extremidade);
    int getGrau();
    void imprimir();

private:
    // informacao do vertice
    int info;
    float peso;
    // lista das arestas deste nó
    list<Aresta> arestas;
    //vetor de aresta
};

#endif // VERTICE_H_INCLUDED
