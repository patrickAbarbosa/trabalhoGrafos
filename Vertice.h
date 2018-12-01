#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED
#include "Aresta.h"
#include <list>

using namespace std;

class Aresta;

class Vertice
{
public:
    // Variáveis auxiliares para os algorítmos desenvolvidos
    int aux1, aux2;
    float aux3;
    // Construtor e destrutor da classe Vertice
    Vertice();
    ~Vertice();
    // Define a identificação do vértice
    void setInfo(int info);
    // Obtém a identificação do vértice
    int getInfo();
    // Define o peso do vértice
    void setPeso(float Peso);
    // obtém o peso do vértice
    float getPeso();
    // Adiciona uma nova aresta ao vértice
    void adicionarAresta(Vertice *extremidade, float peso, bool hide = false);
    // remove uma aresta do vértice utilizando um ponteiro para o vértice da outra extremidade
    void removerAresta(Vertice *extremidade);
    // remove uma aresta do vértice utilizando a identificação do vértice da outra extremidade
    void removerAresta(int extremidade);
    // Obtém o grau do vértice
    int getGrau();
    // Imprime o vértice
    void imprimir();
    // Obtém um iterador para o ínicio da lista de arestas
    list<Aresta>::iterator inicio();
    // Obtém um iterador para o final da lista de arestas
    list<Aresta>::iterator final();

private:
    // informacao do vertice (Identificação e peso)
    int info;
    float peso;
    // lista das arestas deste vértice
    list<Aresta> arestas;
    //vetor de aresta
};

#endif // VERTICE_H_INCLUDED
