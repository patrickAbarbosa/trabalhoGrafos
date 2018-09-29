#include <iostream>
#include "Lista.h"
#include "No.h"

using namespace std;

Lista::Lista()
{
    primeiro = NULL;
    tam = 0;
}

// Apaga toda a lista de vertices
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

// Cria e adiciona um vertice a lista
void Lista::add(int info)
{
    if(primeiro == NULL)
    {
        primeiro = new No();
        primeiro->setInfo(info);
        tam++;
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
            No *aux = new No();
            aux->setInfo(info);
            aux->setProx(ant->getProx());
            ant->setProx(aux);
            tam++;
        }
    }
}

// Busca o o vertice que tenha o id passado e o exclui
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
        cout << "Nao ha vertices!" << endl;
}

// Busca e retorna o vertice na posicao k
No* Lista::get(int k)
{
    No *p = primeiro;

    for(int i = 0; p != NULL && i+1 != k; p = p->getProx())
        i++;

    return p;
}

// Retorna o numero de vertices
int Lista::getTam()
{
    return tam;
}
