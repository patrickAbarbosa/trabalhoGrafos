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
    Vertice *extremidade;
public:
    Aresta(){};
    ~Aresta(){};

    void setInfo(Vertice* p){extremidade = p;}
    Vertice* getInfo(){return extremidade;}
    void setPeso(float peso){this->peso = peso;}
    float getPeso(){return peso;}
};


#endif // ARESTA_H_INCLUDED
