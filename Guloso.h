//
// Created by AlexanderSb on 29/09/18.
//
#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include <list>

using namespace std;

typedef struct
{
    list<int> indices;
    list<Vertice*> vertices;
    list<Aresta*> arestas;
    float custo;
    float custoTotal;
}SolucaoGuloso;

class Guloso
{
    private:
        void algoritmoGuloso(list<Aresta*> &C, SolucaoGuloso &solucao);
        void geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao);
        void imprimirCandidatos(list<Aresta*> &C);
        void imprimirSolucao(SolucaoGuloso &solucao);
        void atualizaSolucao(SolucaoGuloso &solucao);
        static bool ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo);
        static bool ordenaCandidatos(Aresta *primeiro, Aresta *segundo);
    public:
        Guloso();
        ~Guloso();

        void calcular(Grafo &grafo, SolucaoGuloso &solucao);
        void imprimir(SolucaoGuloso &solucao);
};


#endif //GULOSO_H

