#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED
#include "ListaAdjacente.h"

class ListaAdjacente;

class No
{
public:
    No();
    ~No();
    void setInfo(int info);
    int getInfo();
    No *getProx();
    void addAresta(No* vertice);
    void setProx(No *prox);
    No *getProx();

private:
    int info;
    int penalidade;
    No *prox;
    ListaAdjacente *a;

};


#endif // NO_H_INCLUDED
