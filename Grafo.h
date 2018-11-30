//
// Created by igorphelype on 29/09/18.
//
#ifndef GRAFO_GRAFO_H
#define GRAFO_GRAFO_H

#include "Vertice.h"
#include <list>
#include <string>
#include <fstream>
#include "Solucao.h"
#include <map>

using namespace std;

class Vertice;

class Grafo {
    private:
        int tam;
        fstream arquivo;
        list<Vertice> vertices;
        int arestas;
        bool digrafo;
        void fechoTransitivoDireto(Vertice *v, list<Vertice*> &S);
        void fechoTransitivoIndireto(Vertice *v, list<Vertice*> &S);
        void prim(Grafo &solucao, list<Aresta*> &S);
        static bool ordenaArestas(Aresta *a, Aresta *b);
        void dijkstra(Vertice *v, float l);
        bool bipartido(Vertice *v, int proximo);
    public:
        Grafo(string diretorio, bool digrafo = false);
        Grafo();
        ~Grafo();
        bool carregar(string diretorio, bool digrafo = false);
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
        unsigned int getArestas();
        bool kRegular(int k);
        void vizinhanca(int info, bool fechada, list<int> &resultado);
        bool completo();
        bool bipartido();
        void sequenciaGraus(list<int> &resultado);
        bool ordenacaoTopologica(list<int> &resultado);
        bool fechoTransitivo(int V, bool direto, list<int> &resultado);
        void kruskal();
        void prim();
        float dijkstra(int v1, int v2);
        float floyd(int v1, int v2);


        void draw(string fileName, SolucaoGuloso *solucao = NULL);

};


#endif //GRAFO_GRAFO_H
