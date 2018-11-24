//
// Created by AlexanderSb on 29/09/18.
//
#ifndef GULOSO_H
#define GULOSO_H

#include "Grafo.h"
#include "Solucao.h"
#include <list>

using namespace std;

class Guloso
{
    private:
        void algoritmoGuloso(list<Aresta*> &C, SolucaoGuloso &solucao);
        void algoritmoGulosoRandomizado(list<Aresta*> &C, SolucaoGuloso &solucao, float alpha);
        void geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao);
        void imprimirCandidatos(list<Aresta*> &C);
        void imprimirSolucao(SolucaoGuloso &solucao);
        void atualizaSolucao(SolucaoGuloso &solucao);
        void copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst);
        static bool ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo);
        static bool ordenaCandidatos(Aresta *primeiro, Aresta *segundo);
    public:
        Guloso();
        ~Guloso();

        void calcular(Grafo &grafo, SolucaoGuloso &solucao);
        void calcularRandomizado(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas);
        void calcularRandomizadoReativo(Grafo &grafo, SolucaoGuloso &solucao, float *alpha, int nAlphas, int bloco, int epocas);
        void imprimir(SolucaoGuloso &solucao, Grafo &grafo, float best = -1);
};


#endif //GULOSO_H

