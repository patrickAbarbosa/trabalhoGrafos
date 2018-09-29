#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "No.h"

class Lista
{
public:
    Lista();
    ~Lista();
    void add(int info);
    void remove(int info);
    No* get(int k);
    int getTam();

private:
    No *primeiro;
    int tam;
};


#endif // LISTA_H_INCLUDED
