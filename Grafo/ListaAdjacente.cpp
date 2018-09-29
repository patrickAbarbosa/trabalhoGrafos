#include "ListaAdjacente.h"
#include <iostream>

using namespace std;

ListaAdjacente::ListaAdjacente()
{
    primeiro = NULL;
}

ListaAdjacente::~ListaAdjacente()
{
    if(primeiro != NULL)
    {
        NoAdj *aux;
        while(primeiro != NULL)
        {
            aux = primeiro->getProx();
            delete primeiro;
            primeiro = aux;
        }
    }
}

void ListaAdjacente::add(No *vertice, int peso)
{
    if(primeiro == NULL)
    {
        primeiro = new NoAdj();
        primeiro->setInfo(vertice);


    }
}

bool testaPeso(int peso)
{
    if(peso < 1)
    {
        cout << "Peso da aresta invalido!" << endl;
        cout << "Peso alterado para 1" << endl;
        return 0;
    }
    return 1;
}

No* ListaAdjacente::get(int k)
{
    int i = 0;
    for(NoAdj *p = primeiro; p != NULL; p = p->getProx())
    {
        if(i == k)
            return p->getInfo();
        i++;
    }
    return NULL;
}

