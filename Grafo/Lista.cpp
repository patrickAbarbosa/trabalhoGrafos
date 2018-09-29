#include <iostream>
#include "Lista.h"
#include "No.h"

using namespace std;

Lista::Lista()
{
    primeiro = NULL;
    tam = 0;
}

Lista::~Lista()
{
    if(primeiro != NULL)
    {
        No *aux;
        while(primeiro != NULL)
        {
            aux = primeiro->getProx();
            delete primeiro;
            primeiro = aux;
        }
    }
}

void Lista::add(int info)
{
    if(primeiro == NULL)
    {
        primeiro = new No();
        primeiro->setInfo(info);
        primeiro->setProx(NULL);
    }
    else
    {
        No *ant;
        No *p = primeiro;
        while(p != NULL && p->getInfo() < info)
        {
            ant = p;
            p = p->getProx();
        }
        if(p->getInfo() != info)
        {
            p = new No();
            p->setInfo();
            p->setProx(ant->getProx());
            ant->setProx(p);
            tam++;
        }
    }
}

void Lista::remove(int info)
{
    if(primeiro != NULL)
    {
        No *ant;
        No *p = primeiro;

        while(p!=NULL && p->getInfo() != info)
        {
            ant = p;
            p = p->getProx();
        }
        if(p->getInfo() == info)
        {
            ant->setProx(p->getProx());
            delete p;
            tam--;
        }

        else
            cout << "Nao e possivel remover! Vertice nao encontrado." << endl;
    }
    else
        cout << "Nao a vertices!" << endl;
}

No* Lista::get(int k)
{
    No *p = primeiro;

    for(int i = 0; p != NULL && i+1 != k; p = p->getProx())
        i++;

    return p;
}

int Lista::getTam()
{
    return tam;
}
