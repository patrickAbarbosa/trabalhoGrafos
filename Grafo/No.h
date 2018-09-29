#ifndef NO_H_INCLUDED
#define NO_H_INCLUDED
#include "ListaAdjacente.h"

class ListaAdjacente;

class No
{
public:
    No();
    ~No();
    void setInfo(int info); // Altera a informacao
    int getInfo(); // Retorna a informacao
    No *getProx(); // Retorna o proximo No
    void addAresta(No* vertice, float peso); // Adiciona aresta com vertice x e o peso da mesma
    void setProx(No *prox); // Altera o proximo

private:
    int info; // Informacao ( id )
    int penalidade; // Penalidade por nao passar no vertice
    No *prox; // Ponteiro para o proximo
    ListaAdjacente *lista; // Lista de vertices adjacentes e seu peso

};


#endif // NO_H_INCLUDED
