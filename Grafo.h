//
// Created by igorphelype on 29/09/18.
//
#ifndef GRAFO_GRAFO_H
#define GRAFO_GRAFO_H

#include "Vertice.h"
#include <list>
#include <string>

using namespace std;

class Vertice;

class Grafo {
    private:
        int tam;
        string arquivo;
        list<Vertice> vertices;
    public:
        Grafo(string arquivo);
        Grafo();
        ~Grafo();
        Vertice* busca(int info);
        void adicionarVertice(int info, float peso);
        void removerVertice(int info);
        void adicionarAresta(int infoA, int infoB, float peso);
        void removerAresta(int infoA, int infoB);
        unsigned int getTam(); //vertices.size();
        unsigned int  getGrau(int info);
};


#endif //GRAFO_GRAFO_H
