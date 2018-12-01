#ifndef ARESTA_H_INCLUDED
#define ARESTA_H_INCLUDED
#include "Vertice.h"
#include <list>

class Vertice;

class Aresta
{
private:
    // Peso da aresta
    float peso;
    // Vértice do origem da aresta
    Vertice *origem;
    // Vértice da outra extremidade da aresta
    Vertice *extremidade;
public:
    // Indica se a aresta deve ser mostrada quando imprimimos o grafo
    bool hide;
    // Variaveis auxiliares para os algorítmos desenvolvidos
    int aux1, aux2;
    // Construtor e destrutor da classe Aresta
    Aresta(){};
    ~Aresta(){};

    // Define o vértice extremidade da aresta
    void setExtremidade(Vertice* p){extremidade = p;}
    // Obtém o vértice extremidade da aresta
    Vertice* getExtremidade(){return extremidade;}
    // Define o vértice origem da aresta
    void setOrigem(Vertice* p){origem = p;}
    // Obtém o vértice origem da aresta
    Vertice* getOrigem(){return origem;}
    // Define o peso da aresta
    void setPeso(float peso){this->peso = peso;}
    // Obtém o peso da aresta
    float getPeso(){return peso;}
};


#endif // ARESTA_H_INCLUDED
