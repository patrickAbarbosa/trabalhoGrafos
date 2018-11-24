//
// Created by igorphelype on 29/09/18.
//
#include <iostream>
#include <stddef.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include "Grafo.h"
#include "Vertice.h"
#include "Solucao.h"
#include "Aresta.h"
#include <fstream>

using namespace std;

Grafo::Grafo(string diretorio){
    arestas = 0;
    arquivo.open(diretorio.c_str());
    if(!arquivo.is_open())
        cout << "Nao foi possivel abrir o arquivo!" << endl;
    else
    {
        string line;
        bool nodeMode = true;
        while(!arquivo.eof())
        {
            getline(arquivo, line);
            if(line.size() == 0)
                continue;
            if(line[0] == '#')
                continue;
            if(line == "node")
                nodeMode = true;
            else if(line == "link")
                nodeMode = false;
            else
            {
                stringstream ss(line);
                if(nodeMode)
                {
                    int id;
                    float h, type, weight;
                    ss >> id >> h >> type >> weight;
                    adicionarVertice(id, weight);
                }
                else
                {
                    int n1, n2;
                    float weight, stype;
                    ss >> n1 >> n2 >> weight >> stype;
                    adicionarAresta(n1, n2, weight);
                }
            }
        }
        arquivo.close();
    }
}

Grafo::Grafo()
{
    arestas = 0;
}

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
    if(extremidade == NULL || origem == NULL){
        return;
    }
    extremidade->adicionarAresta(origem, peso);
    origem->adicionarAresta(extremidade, peso);
    arestas++;
}

void Grafo::removerAresta(int infoA, int infoB){
    busca(infoA)->removerAresta(infoB);
    arestas--;

}
unsigned int Grafo::getTam(){
    return (unsigned  int) vertices.size();
}

unsigned int  Grafo::getGrau(int info){
    return (unsigned int) busca(info)->getGrau();
}

unsigned int Grafo::getArestas(){
    return (unsigned  int) arestas;
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

void Grafo::draw(string fileName, SolucaoGuloso *solucao)
{
    cout << "Criando .dot" << endl;
    stringstream ss;
    ss << "strict graph {" << endl;
    ss << "{" << endl;
    ss << "node [style=filled];" << endl;
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        bool inSolution = false;
        ss << "\"" << vertice->getInfo() << " ( " << vertice->getPeso() << " )\" [";
        if(solucao != NULL)
        {
            for(list<Vertice*>::iterator sVertice = solucao->vertices.begin(); sVertice != solucao->vertices.end(); sVertice++)
            {
                if(*sVertice == &(*vertice))
                {
                    inSolution = true;
                    break;
                }
            }
        }
        if(inSolution)
        {
            ss << "fillcolor=red];" << endl;
        }
        else
            ss << "fillcolor=white];" << endl;
    }
    ss << "}" << endl;
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            ss << "\"" << aresta->getOrigem()->getInfo() << " ( " << aresta->getOrigem()->getPeso() << " )\" -- \"" << aresta->getExtremidade()->getInfo() << " ( " << aresta->getExtremidade()->getPeso() << " )\" ";
            ss << "[label=\"" << aresta->getPeso() << "\",weight=\"" << aresta->getPeso() << "\"";
            if(solucao != NULL)
            {
                for(list<Aresta*>::iterator sAresta = solucao->arestas.begin(); sAresta != solucao->arestas.end(); sAresta++)
                {
                    if(*sAresta == &(*aresta))
                    {
                        ss << ",color=red,penwidth=2.0";
                        break;
                    }
                }
            }
            ss << "];" << endl;
        }
    }
    ss << "}";

    cout << "Renderizando" << endl;
    string dotFileName = fileName+".dot";
    ofstream dot(dotFileName.c_str());
    dot << ss.rdbuf();
    dot.close();
    string command = "dot -x -Goverlap=scale -Tpng -o"+fileName+" "+dotFileName+"; xdg-open "+fileName;
    system(command.c_str());

}
