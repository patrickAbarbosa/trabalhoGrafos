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
        // O arquivo utilizado para carregar o grafo
        fstream arquivo;
        // Formato do arquivo ( Artigo ou Aula )
        bool paperMode;
        // Lista de vértices do grafo
        list<Vertice> vertices;
        // quantidade de arestas do grafo
        int arestas;
        // indica se este grafo é um digrafo
        bool digrafo;
        // calcula o fecho transitivo direto recursivamente
        void fechoTransitivoDireto(Vertice *v, list<Vertice*> &S);
        // calcula o fecho transitivo indireto recursivamente
        void fechoTransitivoIndireto(Vertice *v, list<Vertice*> &S);
        // encontra a AGM por prim recursivamente
        void prim(Grafo &solucao, list<Aresta*> &S);
        // ordena duas arestas com base no seu peso
        static bool ordenaArestas(Aresta *a, Aresta *b);
        // Encontra o caminho mínimo por Dijkstra recursivamente
        void dijkstra(Vertice *v, float l);
        // Verifica se o grafo é bipartido recursivamente
        bool bipartido(Vertice *v, int proximo);
    public:
        // Constrói um grafo ou digrafo utilizando um arquivo de entrada
        Grafo(string diretorio, bool digrafo = false);
        // Contrói um grafo ou digrafo vazio
        Grafo(bool digrafo = false);
        // Destrutor do grafo
        ~Grafo();
        // Carrega os vértices e arestas de um grafo/digrafo à partir de um arquivo
        bool carregar(string diretorio, bool digrafo = false);
        // Salva o grafo em um arquivo
        bool salvar(string diretorio);
        // Imprime o grafo na tela
        void imprimir();
        // Encontra um vértice utilizando sua identificação
        Vertice* busca(int info);
        // Adiciona um novo vértice ao grafo
        void adicionarVertice(int info, float peso);
        // Remove um vértice do grafo
        void removerVertice(int info);
        // Adiciona uma aresta ao grafo
        void adicionarAresta(int infoA, int infoB, float peso);
        // Remove uma aresta do grafo
        void removerAresta(int infoA, int infoB);
        // Iterador para o início da lista de vértices
        list<Vertice>::iterator inicio();
        // Iterador para o final da lista de vértices
        list<Vertice>::iterator final();
        // Retorna a ordem do grafo
        unsigned int getTam();
        // Obtém o grau de um vértice
        unsigned int  getGrau(int info);
        // Obtém o grau de saída e entrada um vértice
        bool getGrau(int info, int *gIn, int *gOut);
        // obtém a quantidade de arestas do grafo
        unsigned int getArestas();
        // Verifica se um grafo é K-regular
        bool kRegular(int k);
        // obtém a vizinhança aberta ou fechada de um vértice
        void vizinhanca(int info, bool fechada, list<int> &resultado);
        // verifica se um grafo é completo
        bool completo();
        // verifica se um grafo é bipartido
        bool bipartido();
        // obtém a sequência de graus do grafo
        void sequenciaGraus(list<int> &resultado);
        // obtém a ordenaçã topológica do grafo
        bool ordenacaoTopologica(list<int> &resultado);
        // obtém o fecho transitivo direto ou indireto para um vértice
        bool fechoTransitivo(int V, bool direto, list<int> &resultado);
        // Encontra a AGM utilizando o algorítmo de Kruskal
        void kruskal();
        // Encontra a AGM utilizando o algorítmo de Prim
        void prim();
        // Obtém a distância entre dois vértices utilizando o algorítmo de dijkstra
        float dijkstra(int v1, int v2);
        // Obtém a distância entre dois vértices utilizando o algorítmo de floyd
        float floyd(int v1, int v2);

        // Gera a imagem de um grafo utilizando o GraphViz
        void draw(string fileName, SolucaoGuloso *solucao = NULL);

};


#endif //GRAFO_GRAFO_H
