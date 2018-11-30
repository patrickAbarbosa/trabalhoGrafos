#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED
#include "Aresta.h"
#include <list>

using namespace std;

class Aresta;

class Vertice
{
public:
    void *data;
    int aux1, aux2;
    float aux3;
    Vertice();
    ~Vertice();
    void setInfo(int info);
    int getInfo();
    void setPeso(float Peso);
    float getPeso();
    void adicionarAresta(Vertice *extremidade, float peso, bool hide = false);
    void removerAresta(Vertice *extremidade);
    void removerAresta(int extremidade);
    int getGrau();
    void imprimir();
    list<Aresta>::iterator inicio();
    list<Aresta>::iterator final();

private:
    // informacao do vertice
    int info;
    float peso;
    // lista das arestas deste nó
    list<Aresta> arestas;
    //vetor de aresta
};

#endif // VERTICE_H_INCLUDED
