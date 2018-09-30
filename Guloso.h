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
    public:
        Guloso();
        ~Guloso();

        list<int> calcular(Grafo &grafo);
};


#endif //GULOSO_H

