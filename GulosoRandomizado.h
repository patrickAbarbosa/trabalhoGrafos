//
// Created by AlexanderSb on 29/09/18.
//
#ifndef GULOSO_RANDOMIZADO_H
#define GULOSO_RANDOMIZADO_H

#include "Grafo.h"
#include "Solucao.h"
#include <list>

using namespace std;

class GulosoRandomizado
{
    private:
        void algoritmoGulosoRandomizado(list<Aresta*> &C, SolucaoGuloso &solucao, float alpha);
        void geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao);
        void imprimirCandidatos(list<Aresta*> &C);
        void imprimirSolucao(SolucaoGuloso &solucao);
        void atualizaSolucao(SolucaoGuloso &solucao);
        void copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst);
        static bool ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo);
        static bool ordenaCandidatos(Aresta *primeiro, Aresta *segundo);
    public:
        GulosoRandomizado();
        ~GulosoRandomizado();

        void calcular(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas);
        void imprimir(SolucaoGuloso &solucao);
};


#endif //GulosoRandomizado_H

