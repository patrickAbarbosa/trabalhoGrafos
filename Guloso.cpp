
#include "Guloso.h"

Guloso::Guloso()
{

}

Guloso::~Guloso()
{

}

list<int> Guloso::calcular(Grafo &grafo)
{
    list<int> S;
    if(grafo.getTam() == 0)
        return S;

    Vertice *selecao = NULL;

    int pesoTotal = 0;
    for(list<Vertice>::iterator it = grafo.inicio(); it != grafo.final(); it++)
    {
        pesoTotal += it->getPeso();
        if(selecao == NULL || it->getPeso() > selecao->getPeso())
            selecao = &*it;
    }

    S.push_back(selecao->getInfo());

    return S;
}
