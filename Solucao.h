#ifndef SOLUCAO_H_
#define SOLUCAO_H_

#include <list>

using namespace std;

// Define a solução para os algorítmos gulosos implementados
typedef struct
{
    // Toda solução tem uma lista de índices, que são as identificações dos vértices presentes na solução
    list<int> indices;
    // A solução também contém uma lista de ponteiros para os vértices presentes na solução
    list<Vertice*> vertices;
    // A solução contém uma lista de arestas presentes, uma vez que para o problema da árvore de Steiner não estamos interessados apenas nos nós
    list<Aresta*> arestas;
    // O custo da solução
    float custo;
    // O custo total da solução se nenhum vértice for utilizado
    float custoTotal;
    // O valor de alfa utilizado na solução para o caso randomizado e randomizado reativo
    float alpha;
    // A iteração que está solução foi encontrada
    int iteracao;
}SolucaoGuloso;

#endif
