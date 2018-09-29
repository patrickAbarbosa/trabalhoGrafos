#ifndef NOADJ_H_INCLUDED
#define NOADJ_H_INCLUDED
#include <cstddef>
#include "No.h"


class No;

class NoAdj
{
public:
    NoAdj(){info = NULL; prox=NULL;}
    ~NoAdj(){}

    void setInfo(No* p){info = p;}                // Altera o informacao "Id"
    No* getInfo(){return info;}                   // Retorna um ponteiro para o vertice adjacente
    NoAdj *getProx(){return prox;}                // Retorna o proximo adjacente
    void setProx(NoAdj *p){prox = p;}             // Altera o proximo
    void setPeso(float peso){this->peso = peso;} // Altera o peso da aresta
    int getPeso(){return peso;}


private:
    No     *info;  // Ponteiro para o vertice Adjacente
    NoAdj  *prox;  // Ponteiro para o proximo
    float peso;   // Peso da aresta entre os vertices

};

#endif // NOADJ_H_INCLUDED
