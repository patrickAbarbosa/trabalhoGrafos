#include "ListaAdjacente.h"
#include <iostream>

using namespace std;

ListaAdjacente::ListaAdjacente()
{
    primeiro = NULL;
    tam = 0;
}

// Apaga toda a lista de vertices adjacente
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

// Adiciona vertice em ordem crescente com a id

void ListaAdjacente::add(No *vertice, int peso)
{
    if(primeiro == NULL)
    {
        primeiro = new NoAdj();
        primeiro->setInfo(vertice);
        primeiro->setPeso(peso);
    }
    else
    {
        NoAdj *ant = NULL;
        NoAdj *p = primeiro;

        while(p != NULL && p->getInfo()->getInfo() < vertice->getInfo())
        {
            ant = p;
            p = p->getProx();
        }

        if(p->getInfo()->getInfo() != vertice->getInfo())
        {
            NoAdj* aux = new NoAdj();
            aux->setInfo(vertice);
            aux->setPeso(peso);
            if(p == primeiro)
            {
                aux->setProx(p);
                primeiro = aux;
            }
            else
            {
                aux->setProx(ant->getProx());
                ant->setProx(aux);
            }
            tam++;
        }
    }
}

// Busca pelo indice
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

