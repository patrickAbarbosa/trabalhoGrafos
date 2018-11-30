#ifndef ARESTA_H_INCLUDED
#define ARESTA_H_INCLUDED
#include "Vertice.h"
#include <list>
class Vertice;

class Aresta
{
private:
    // peso da aresta
    float peso;
    Vertice *origem;
    Vertice *extremidade;
public:
    bool hide;
    int aux1, aux2;
    Aresta(){};
    ~Aresta(){};

    void setExtremidade(Vertice* p){extremidade = p;}
    Vertice* getExtremidade(){return extremidade;}
    void setOrigem(Vertice* p){origem = p;}
    Vertice* getOrigem(){return origem;}
    void setPeso(float peso){this->peso = peso;}
    float getPeso(){return peso;}
};


#endif // ARESTA_H_INCLUDED
