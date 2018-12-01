#include <iostream>
#include "Vertice.h"

using namespace std;

Vertice::Vertice()
{
    // Define os vértices inicialmente com peso 1
    peso = 1;
}

Vertice::~Vertice()
{
    // Ao destruir um vértice, destroi-se suas arestas limpando a lista de arestas
    arestas.clear();
}


void Vertice::setInfo(int info)
{
    // Seta a identificação para o vértice
    this->info = info;
}

int Vertice::getInfo()
{
    // retorna a identificação para o vértice
    return info;
}

void Vertice::setPeso(float peso)
{
    // Seta o peso para o vértice
    this->peso = peso;
}

float Vertice::getPeso()
{
    // retorna o peso para o vértice
    return peso;
}

void Vertice::adicionarAresta(Vertice *extremidade, float peso, bool hide)
{
    // Cria-se uma nova aresta
    Aresta aresta;
    // Configura o peso da nova aresta
    aresta.setPeso(peso);
    // Configura a origem da nova aresta como o vértice atual
    aresta.setOrigem(this);
    // Configura a extremidade da nova aresta como o vértice de destino
    aresta.setExtremidade(extremidade);
    // Configura a visibilidade da aresta ao imprimir
    aresta.hide = hide;
    // Adiciona a aresta à lista de arestas do vértice
    arestas.push_front(aresta);
}

void Vertice::removerAresta(Vertice *extremidade)
{
    // Para remover uma aresta, itera entre todas as arestas do vértice
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); )
    {
        // Sempre que a extremidade da aresta atual for igual à extremidade que desejamos remover,
        // limpamos esta aresta da lista
        if(it->getExtremidade() == extremidade)
            it = arestas.erase(it);
        else
            it++;
    }
}

void Vertice::removerAresta(int extremidade)
{
    // Para remover uma aresta, itera entre todas as arestas do vértice
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); )
    {
        // Sempre que a extremidade da aresta atual tiver a identificação
        // igual à identificação da extremidade que desejamos remover, limpamos esta aresta da lista
        if(it->getExtremidade()->info == extremidade)
            it = arestas.erase(it);
        else
            it++;
    }
}

int Vertice::getGrau()
{
    // Retorna o grau do vértice, que será igual ao tamanho da lista de vértices
    return arestas.size();
}

void Vertice::imprimir()
{
    // Para imprimir o vértice, iteramos sobre a lista de arestas
    for(list<Aresta>::iterator it = arestas.begin(); it != arestas.end(); it++)
    {
        // Se a aresta atual não for oculta, imprimimos esta aresta na tela
        if(!it->hide)
            cout << "(" << info << ", " << peso << ") (" << it->getExtremidade()->info << ", " << it->getExtremidade()->peso << ") " << it->getPeso() << endl;
    }
}

list<Aresta>::iterator Vertice::inicio(){
    // Retorna um iterador que aponta para o início da lista de arestas
    return arestas.begin();
}
list<Aresta>::iterator Vertice::final(){
    // Retorna um iterador que aponta para o final da lista de arestas
    return arestas.end();
}
