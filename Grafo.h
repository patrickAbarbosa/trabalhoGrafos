//
// Created by igorphelype on 29/09/18.
//
#ifndef GRAFO_GRAFO_H
#define GRAFO_GRAFO_H

#include "Vertice.h"
#include "Solucao.h"
#include <list>
#include <string>
#include <fstream>

using namespace std;

class Vertice;

class Grafo {
    private:
        int tam;
        fstream arquivo;
        list<Vertice> vertices;
    public:
        Grafo(string diretorio);
        Grafo();
        ~Grafo();
        void imprimir();
        Vertice* busca(int info);
        void adicionarVertice(int info, float peso);
        void removerVertice(int info);
        void adicionarAresta(int infoA, int infoB, float peso);
        void removerAresta(int infoA, int infoB);
        list<Vertice>::iterator inicio();
        list<Vertice>::iterator final();
        unsigned int getTam(); //vertices.size();
        unsigned int  getGrau(int info);

        void draw(string fileName, SolucaoGuloso *solucao = NULL);

};


#endif //GRAFO_GRAFO_H
