//
// Created by igorphelype on 29/09/18.
//
#include <iostream>
#include <stddef.h>
#include <string>
#include "Grafo.h"
#include "Vertice.h"
#include <fstream>

using namespace std;

Grafo::Grafo(string diretorio){
    arquivo.open(diretorio.c_str());
    if(!arquivo.is_open())
        cout << "Nao foi possivel abrir o arquivo!" << endl;
    else
    {
        int a;
        int b;
        float c;
        int contaLinha = 0;

        arquivo >> tam;
        while(!arquivo.eof())
        {
            if(contaLinha < tam)
            {
                arquivo >> a >> c;
                adicionarVertice(a, c);
            }
            else
            {
                arquivo >> a >> b >> c;
                adicionarAresta(a, b, c);
            }
            contaLinha++;
        }
        arquivo.close();
    }
}

Grafo::Grafo(){}

Grafo::~Grafo(){
    vertices.clear();
};

void Grafo::adicionarVertice(int info, float peso){
    if(busca(info) != NULL){
        return;
    }
    Vertice novoVertice;
    novoVertice.setInfo(info);
    novoVertice.setPeso(peso);
    this->vertices.push_front(novoVertice);
}

void Grafo::removerVertice(int info){
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        it->removerAresta(info);
        if(it->getInfo() == info) {
            it = vertices.erase(it);
        }
    }
}

void Grafo::adicionarAresta(int infoA, int infoB, float peso){
    Vertice *extremidade = busca(infoB);
    Vertice *origem = busca(infoA);
    // infoB não encontrado
    if(extremidade == NULL && origem == NULL){
        return;
    }
    extremidade->adicionarAresta(origem, peso);
    origem->adicionarAresta(extremidade, peso);
}

void Grafo::removerAresta(int infoA, int infoB){
    busca(infoA)->removerAresta(infoB);

}
unsigned int Grafo::getTam(){
    return (unsigned  int) vertices.size();
}

unsigned int  Grafo::getGrau(int info){
    return (unsigned int) busca(info)->getGrau();
}

Vertice* Grafo::busca(int info){
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        if(it->getInfo() == info) {
            return &(*it);
        }
    }
    return NULL;
}

void Grafo::imprimir() {
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        it->imprimir();
    }
}

list<Vertice>::iterator Grafo::inicio(){
    return this->vertices.begin();
}

list<Vertice>::iterator Grafo::final(){
    return this->vertices.end();
}
