//
// Created by AlexanderSb on 29/09/18.
//
#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <list>

using namespace std;

class Guloso
{
    private:
        list<int> gerarListSolucao(list<Vertice*> vertices);
        list<Vertice*> algoritmoGuloso(list<Vertice*> &C, int pesoTotal);
        Vertice* seleciona(list<Vertice*> &C);
        bool viavel(list<Vertice*> &S, Vertice *x);

    public:
        Guloso();
        ~Guloso();

        list<int> calcular(Grafo &grafo);
};


#endif //GULOSO_H

