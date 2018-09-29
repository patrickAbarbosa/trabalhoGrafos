#ifndef LISTAADJACENTE_H_INCLUDED
#define LISTAADJACENTE_H_INCLUDED
#include "NoAdj.h"
#include "No.h"

class ListaAdjacente
{
public:
    ListaAdjacente();
    ~ListaAdjacente();
    void add(No *vertice);
    void remove(No *vertice);

private:
    NoAdj *primeiro;
};

#endif // LISTAADJACENTE_H_INCLUDED
