//
// Created by igorphelype on 29/09/18.
//

#include <stddef.h>
#include <string>
#include "Grafo.h"
#include "Vertice.h"

using namespace std;

Grafo::Grafo(string arquivo){
    this->arquivo = arquivo;
}

Grafo::Grafo(){}
Grafo::~Grafo(){};

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
    // infoB não encontrado
    if(extremidade == NULL){
        return;
    }
    for(list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        if(it->getInfo() == infoA) {
            it->adicionarAresta(extremidade, peso);
        }
    }
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
