#ifndef NOADJ_H_INCLUDED
#define NOADJ_H_INCLUDED
#include <cstddef>
#include "No.h"vou su


class No;

class NoAdj
{
public:
    NoAdj(){info = NULL; prox=NULL;}
    ~NoAdj(){}
    void setInfo(No* p){info = p;}
    No* getInfo(){return info;}
    NoAdj *getProx(){return prox;}
    void setPeso(int peso){return peso;}
    int getPeso(){return peso;}
    void setPeso(int peso){this->peso = peso};

private:
    No *info;
    NoAdj *prox;
    int peso;

};

#endif // NOADJ_H_INCLUDED
