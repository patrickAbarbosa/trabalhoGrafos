#include <iostream>
#include "Vertice.h"

using namespace std;

Vertice::Vertice()
{
    //Criando Vertice
    peso = 1;
}

Vertice::~Vertice()
{
    // Destruindo Vertice e suas arestas
    arestas.clear();
}

void Vertice::setInfo(int info)
{
    this->info = info;
}

int Vertice::getInfo()
{
    return info;
}

void Vertice::setPeso(float peso)
{
    this->peso = peso;
}

float Vertice::getPeso()
{
    return peso;
}

void Vertice::adicionarAresta(Vertice *extremidade, float peso)
{
    Aresta aresta;
    aresta.setPeso(peso);
    aresta.setInfo(extremidade);
    arestas.push_back(aresta);
}

void Vertice::removerAresta(Vertice *extremidade)
{
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); )
    {
        if(it->getInfo() == extremidade)
            it = arestas.erase(it);
        else
            it++;
    }
}

void Vertice::removerAresta(int extremidade)
{
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); )
    {
        if(it->getInfo()->info == extremidade)
            it = arestas.erase(it);
        else
            it++;
    }
}

int Vertice::getGrau()
{
    return arestas.size();
}

void Vertice::imprimir()
{
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); it++)
    {
        cout << "(" << info << ", " << peso << ") (" << it->getInfo()->info << ", " << it->getInfo()->peso << ") " << it->getPeso() << endl;
    }
}
