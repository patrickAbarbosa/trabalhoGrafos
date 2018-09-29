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
