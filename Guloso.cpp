
#include "Guloso.h"

Guloso::Guloso()
{

}

Guloso::~Guloso()
{

}

list<int> Guloso::calcular(Grafo &grafo)
{
    if(grafo.getTam() == 0)
        return list<int>();

    // Cria conjunto de entrada vazio
    list<Vertice*> C;

    // Calcula o peso total e adiciona vértices ao conjunto de entrada
    int pesoTotal = 0;
    for(list<Vertice>::iterator it = grafo.inicio(); it != grafo.final(); it++)
    {
        C.push_back(&*it);
        pesoTotal += it->getPeso();
    }

    // Executa o algoritmo guloso
    list<Vertice*> S = algoritmoGuloso(C, pesoTotal);

    return gerarListSolucao(S);
}

list<int> Guloso::gerarListSolucao(list<Vertice*> vertices)
{
    list<int> ids;
    for(list<Vertice*>::iterator it = vertices.begin(); it != vertices.end(); it++)
    {
        ids.push_back((*it)->getInfo());
    }
    return ids;
}

list<Vertice*> Guloso::algoritmoGuloso(list<Vertice*> &C, int pesoTotal)
{
    list<Vertice*> S; // Solução vazia

    Vertice *x; // Vértice de seleção

    while(C.size() > 0)
    {
        x = seleciona(C);
        if(viavel(S, x))
            S.push_back(x);

    }

    return S;
}

Vertice* Guloso::seleciona(list<Vertice*> &C)
{
    list<Vertice*>::iterator x = C.end();
    for(list<Vertice*>::iterator it = C.begin(); it != C.end(); it++)
    {
        if(x == C.end() || (*x)->getPeso() < (*it)->getPeso())
            x = it;
    }
    Vertice *selectedX = *x;
    if(x != C.end())
        C.erase(x);
    return selectedX;
}

bool Guloso::viavel(list<Vertice*> &S, Vertice *x)
{
    if(S.size() == 0)
        return true;
    else
    {
        Vertice *last = S.back();
        for(list<Aresta>::iterator it = last->inicio(); it != last->final(); it++)
        {
            if(it->getInfo()->getInfo() == x->getInfo())
                return true;
        }
    }
    return false;
}
