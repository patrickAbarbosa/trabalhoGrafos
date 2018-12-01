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
        // Método responsável por executar o algorítmo guloso
        void algoritmoGuloso(list<Aresta*> &C, SolucaoGuloso &solucao);
        // Método responsável por executar o algorítmo guloso randomizado
        void algoritmoGulosoRandomizado(list<Aresta*> &C, SolucaoGuloso &solucao, float alpha);
        // Método responsável por gerar a lista de arestas candidatas à entrarem na solução
        void geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao);
        // Método que imprime os candidatos
        void imprimirCandidatos(list<Aresta*> &C);
        // Método que imprime uma solução
        void imprimirSolucao(SolucaoGuloso &solucao);
        // Método que atualiza uma solução, preenchendo todos os seus campos
        void atualizaSolucao(SolucaoGuloso &solucao);
        // Método que copia uma solução para outra
        void copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst);
        // Método que ordena a lista de candidatos para a solução vazia
        static bool ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo);
        // Método que ordena a lista de candidatos para a solução não vazia
        static bool ordenaCandidatos(Aresta *primeiro, Aresta *segundo);
    public:
        // Construtor e destrutor da classe Guloso
        Guloso();
        ~Guloso();

        // Método para encontrar a solução gulosa de um grafo
        void calcular(Grafo &grafo, SolucaoGuloso &solucao);
        // Método para encontrar a solução gulosa randomizada de um grafo
        void calcularRandomizado(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas);
        // Método para encontrar a solução gulosa randomizada reativa de um grafo
        void calcularRandomizadoReativo(Grafo &grafo, SolucaoGuloso &solucao, float *alpha, int nAlphas, int bloco, int epocas);
        // Método que imprime uma solução
        void imprimir(SolucaoGuloso &solucao, Grafo &grafo, float best = -1);
};


#endif //GULOSO_H

