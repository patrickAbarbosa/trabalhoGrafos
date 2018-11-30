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
#include "Aresta.h"
#include "Solucao.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cfloat>

using namespace std;

int spaces(string &text)
{
    int n = 0;
    bool space = false;
    for(int i = 0; i < text.size(); i++)
    {
        if((text[i] == ' ' || text[i] == '\t') && space == false)
        {
            space = true;
            n++;
        }
        else if((text[i] != ' ' && text[i] != '\t') && space == true)
        {
            space = false;
        }
    }
    return n;
}

Grafo::Grafo(string diretorio, bool digrafo){
    carregar(diretorio, digrafo);
}

Grafo::Grafo()
{
    arestas = 0;
}

Grafo::~Grafo(){
    vertices.clear();
};

bool Grafo::carregar(string diretorio, bool digrafo)
{
    cout << "Di: " << digrafo << endl;
    vertices.clear();
    arestas = 0;
    this->digrafo = digrafo;
    arquivo.open(diretorio.c_str());
    if(!arquivo.is_open())
    {
        cout << "Nao foi possivel abrir o arquivo!" << endl;
        return false;
    }
    else
    {
        string line;
        bool nodeMode = true;
        bool paperMode = false;
        int n = 0;
        while(!arquivo.eof())
        {
            getline(arquivo, line);
            if(line.size() == 0)
                continue;
            if(line[0] == '#')
                continue;
            n = spaces(line);
            if(line == "node")
            {
                nodeMode = true;
                paperMode = true;
            }
            else if(line == "link")
            {
                nodeMode = false;
                paperMode = true;
            }
            else
            {
                if(n == 0)
                {
                    paperMode = false;
                    int sz = atoi(line.c_str());
                    for(int k = 1; k <= sz; k++)
                        adicionarVertice(k, 0);
                }
                else
                {
                    stringstream ss(line);
                    if(paperMode == false)
                    {
                        if(n == 1)
                        {
                            int n1, n2;
                            ss >> n1 >> n2;
                            adicionarAresta(n1, n2, 0);
                        }
                        if(n == 2)
                        {
                            int n1, n2;
                            float weight;
                            ss >> n1 >> n2 >> weight;
                            adicionarAresta(n1, n2, weight);
                        }
                        else
                        {
                            int n1, n2;
                            float weight;
                            ss >> n1 >> n2 >> weight;
                            adicionarAresta(n1, n2, weight);
                        }
                    }
                    else
                    {
                        if(nodeMode)
                        {
                            int id;
                            float h, type, weight;
                            ss >> id >> h >> type >> weight;
                            adicionarVertice(id, weight);
                        }
                        else
                        {
                            int id, n1, n2;
                            float weight;
                            ss >> id >> n1 >> n2 >> weight;
                            adicionarAresta(n1, n2, weight);
                        }
                    }
                }
            }
        }
        arquivo.close();
    }
    return true;
}

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
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end();){
        it->removerAresta(info);
        if(it->getInfo() == info)
        {
            it = vertices.erase(it);
        }
        else
            ++it;
    }
}

void Grafo::adicionarAresta(int infoA, int infoB, float peso){
    Vertice *extremidade = busca(infoB);
    Vertice *origem = busca(infoA);
    // infoB não encontrado
    if(extremidade == NULL || origem == NULL){
        return;
    }
    if(!digrafo)
        extremidade->adicionarAresta(origem, peso, true);
    origem->adicionarAresta(extremidade, peso);
    arestas++;
}

void Grafo::removerAresta(int infoA, int infoB){
    busca(infoA)->removerAresta(infoB);
    if(!digrafo)
        busca(infoB)->removerAresta(infoA);
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

bool Grafo::kRegular(int k)
{
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        if(vertice->getGrau() != k)
            return false;
    }
    return true;
}

void Grafo::vizinhanca(int info, bool fechada, list<int> &resultado)
{
    Vertice *v = busca(info);
    if(v == NULL)
    {
        cout << "Vértice inválido" << endl;
        return;
    }
    cout << "Vizinhança: ";
    if(fechada)
        cout << info << " ";
    resultado.clear();
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        cout << ", " << aresta->getExtremidade()->getInfo();
        resultado.push_back(aresta->getExtremidade()->getInfo());
    }
    cout << endl;
}

bool Grafo::completo()
{
    int n = getTam();
    n--;
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        if(vertice->getGrau() != n)
            return false;
    }
    return true;
}

bool Grafo::bipartido()
{
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
        vertice->aux1 = -1;

    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        if(vertice->aux1 == -1)
        {
            if(!bipartido(&*vertice, 0))
                return false;
        }
    }
    return true;
}

bool Grafo::bipartido(Vertice *v, int proximo)
{
    v->aux1 = proximo;
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        if(aresta->getExtremidade()->aux1 == -1)
        {
            if(!bipartido(aresta->getExtremidade(), !proximo))
                return false;
        }
        else if(aresta->getExtremidade()->aux1 == proximo)
        {
            return false;
        }
    }
    return true;
}

void Grafo::sequenciaGraus(list<int> &resultado)
{
    resultado.clear();
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
        resultado.push_back(vertice->getGrau());
    resultado.sort();
    cout << "Sequência de graus: ";
    for(list<int>::iterator grau = resultado.begin(); grau != resultado.end(); grau++)
        cout << (*grau) << ", ";
    cout << endl;
}

bool Grafo::ordenacaoTopologica(list<int> &resultado)
{
    if(!digrafo)
    {
        cout << "Este grafo não é um digrafo" << endl;
        return false;
    }
    list<Vertice*> S, L;
    for(list<Vertice>::iterator A = inicio(); A != final(); A++)
    {
        A->aux1 = 0;
        A->aux2 = 0;
        for(list<Vertice>::iterator B = inicio(); B != final(); B++)
        {
            if(A == B)
                continue;
            for(list<Aresta>::iterator aresta = B->inicio(); aresta != B->final(); aresta++)
            {
                aresta->aux1 = 0;
                aresta->aux2 = 0;
                if(aresta->getExtremidade() == &*A)
                {
                    A->aux1++;
                }
            }
        }
        if(A->aux1 == 0)
            S.push_front(&*A);
    }

    while(S.size() > 0)
    {
        Vertice *n = *S.begin();
        S.erase(S.begin());
        L.push_back(n);
        for(list<Aresta>::iterator aresta = n->inicio(); aresta != n->final(); aresta++)
        {
            aresta->aux1 = 1;
            aresta->getExtremidade()->aux1--;
            if(aresta->getExtremidade()->aux1 == 0)
                S.push_front(aresta->getExtremidade());
        }
    }

    bool valid = true;
    for(list<Vertice>::iterator A = inicio(); A != final(); A++)
    {
        for(list<Aresta>::iterator aresta = A->inicio(); aresta != A->final(); aresta++)
        {
            if(aresta->aux1 == 0)
            {
                valid = false;
                break;
            }
        }
        if(!valid)
            break;
    }

    if(!valid)
    {
        cout << "Ordenação topológica: Inválida (Grafo com ciclos)" << endl;
        return false;
    }
    else
    {
        resultado.clear();
        cout << "Ordenação topológica: ";
        for(list<Vertice*>::iterator N = L.begin(); N != L.end(); N++)
        {
            cout << (*N)->getInfo() << ", ";
            resultado.push_back((*N)->getInfo());
        }
        cout << endl;
    }
    return true;
}

bool Grafo::fechoTransitivo(int V, bool direto, list<int> &resultado)
{
    if(!digrafo)
    {
        cout << "Este grafo não é um digrafo" << endl;
        return false;
    }
    Vertice *v = busca(V);
    if(v == NULL)
    {
        cout << "Vértice inválido" << endl;
        return false;
    }
    list<Vertice*> S;
    if(direto)
        fechoTransitivoDireto(v, S);
    else
        fechoTransitivoIndireto(v, S);

    resultado.clear();
    cout << "Conjunto: ";
    for(list<Vertice*>::iterator N = S.begin(); N != S.end(); N++)
    {
        cout << (*N)->getInfo() << ", ";
        resultado.push_back((*N)->getInfo());
    }
    cout << endl;

}

void Grafo::fechoTransitivoDireto(Vertice *v, list<Vertice*> &S)
{
    if(find(S.begin(), S.end(), v) != S.end())
        return;
    S.push_back(v);
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        fechoTransitivoDireto(aresta->getExtremidade(), S);
    }
}

void Grafo::fechoTransitivoIndireto(Vertice *v, list<Vertice*> &S)
{
    if(find(S.begin(), S.end(), v) != S.end())
        return;
    S.push_back(v);
    for(list<Vertice>::iterator B = inicio(); B != final(); B++)
    {
        if(v == &*B)
            continue;
        for(list<Aresta>::iterator aresta = B->inicio(); aresta != B->final(); aresta++)
        {
            if(aresta->getExtremidade() == v)
            {
                fechoTransitivoIndireto(aresta->getOrigem(), S);
            }
        }
    }
}

void Grafo::kruskal()
{
    list<Aresta*> S;
    for(list<Vertice>::iterator A = inicio(); A != final(); A++)
    {
        A->aux1 = A->getInfo();
        for(list<Aresta>::iterator aresta = A->inicio(); aresta != A->final(); aresta++)
        {
            aresta->aux1 = -1;
            S.push_back(&*aresta);
        }
    }


    S.sort(ordenaArestas);
    for(list<Aresta*>::iterator s1 = S.begin(); s1 != S.end(); s1++)
    {
        Vertice *v1 = (*s1)->getOrigem();
        Vertice *v2 = (*s1)->getExtremidade();
        if(v1->aux1 != v2->aux1)
        {
            (*s1)->aux1 = v1->aux1;
            for(list<Aresta*>::iterator s2 = S.begin(); s2 != S.end(); s2++)
            {
                if((*s2)->aux1 == v2->aux1)
                    (*s2)->aux1 = v1->aux1;
            }
            v2->aux1 = v1->aux1;
        }
    }

    map<int, list<Aresta*> > Fs;
    for(list<Aresta*>::iterator s = S.begin(); s != S.end(); s++)
    {
        if((*s)->aux1 >= 0)
        {
            Fs[(*s)->aux1].push_back(*s);
        }
    }
    if(Fs.size() > 1)
    {
        cout << "Este grafo contém um floresta de AGMs" << endl;
        cout << "Floresta -----" << endl;
    }
    bool showN = Fs.size() > 1;
    int n = 1;
    for(map<int, list<Aresta*> >::iterator it = Fs.begin(); it != Fs.end(); ++it, n++)
    {
        if(showN)
            cout << "AGM: " << n << endl;
        else
            cout << "AGM" << endl;
        list<Aresta*> &A = it->second;
        for(list<Aresta*>::iterator a = A.begin(); a != A.end(); a++)
        {
            cout << "( " << (*a)->getOrigem()->getInfo() << ", " << (*a)->getExtremidade()->getInfo() << " ), ";
        }
        n++;
    }
}

void Grafo::prim()
{
    Grafo solucao;
    solucao.digrafo = digrafo;
    Vertice *v = NULL;
    list<Aresta*> S;
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        vertice->aux1 = 0;
        if(v == NULL)
            v = &*vertice;
        else if(vertice->getGrau() > v->getGrau())
            v = &*vertice;
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            aresta->aux1 = 0;
            S.push_back(&*aresta);
        }
    }
    solucao.adicionarVertice(v->getInfo(), v->getPeso());
    prim(solucao, S);
    cout << "AGM" << endl;
    solucao.imprimir();
}

void Grafo::prim(Grafo &solucao, list<Aresta*> &S)
{
    Aresta* C = NULL;
    bool origem = false;
    for(list<Aresta*>::iterator s = S.begin(); s != S.end(); s++)
    {
        if((*s)->aux1 == 1)
            continue;
        Vertice *v1 = solucao.busca((*s)->getOrigem()->getInfo());
        Vertice *v2 = solucao.busca((*s)->getExtremidade()->getInfo());
        if(v1 == NULL && v2 == NULL)
        {
            continue;
        }
        else if(v1 != NULL && v2 != NULL)
        {
            (*s)->aux1 = 1;
        }
        else
        {
            origem = v1 != NULL;
            if(C == NULL)
                C = *s;
            else if((*s)->getPeso() < C->getPeso())
                C = *s;
        }
    }
    if(C == NULL)
        return;
    C->aux1 = 1;
    if(origem)
        solucao.adicionarVertice(C->getOrigem()->getInfo(), C->getOrigem()->getPeso());
    else
        solucao.adicionarVertice(C->getExtremidade()->getInfo(), C->getExtremidade()->getPeso());
    solucao.adicionarAresta(C->getOrigem()->getInfo(), C->getExtremidade()->getInfo(), C->getPeso());
    prim(solucao, S);
}

bool Grafo::ordenaArestas(Aresta *a, Aresta *b)
{
    return a->getPeso() > b->getPeso();
}

float Grafo::dijkstra(int v1, int v2)
{
    Vertice *s = busca(v1);
    Vertice *e = busca(v2);
    if(s == NULL || e == NULL)
    {
        cout << "Vértices inválidos" << endl;
        return -1;
    }
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        vertice->aux3 = FLT_MAX;
        vertice->aux1 = -1;
    }
    s->aux3 = 0;
    dijkstra(s, 0);
    cout << "Distância mínima: " << e->aux3 << endl;
    return e->aux3;
}

void Grafo::dijkstra(Vertice *v, float l)
{
    Vertice *n = NULL;
    float d;
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        if(aresta->getExtremidade()->aux1 < 0)
        {
            aresta->getExtremidade()->aux3 = min(aresta->getExtremidade()->aux3, aresta->getPeso() + l);
            if(n == NULL)
            {
                n = aresta->getExtremidade();
                d = aresta->getPeso();
            }
            else if(aresta->getPeso() < d)
            {
                n = aresta->getExtremidade();
                d = aresta->getPeso();
            }
        }
    }
    if(n != NULL)
    {
        n->aux1 = 1;
        dijkstra(n, n->aux3);
    }
}

float Grafo::floyd(int v1, int v2)
{
    Vertice *s = busca(v1);
    Vertice *e = busca(v2);
    if(s == NULL || e == NULL)
    {
        cout << "Vértices inválidos" << endl;
        return -1;
    }
    float **A_1;
    float **A;

    A_1 = new float*[getTam()];
    A   = new float*[getTam()];
    for(int i = 0; i < getTam(); i++)
    {
        A_1[i] = new float[getTam()];
        A[i]   = new float[getTam()];
    }

    int i1 = 0;
    int i2 = 0;

    int i = 0;
    for(list<Vertice>::iterator a = inicio(); a != final(); a++, i++)
    {
        if(a->getInfo() == s->getInfo())
            i1 = i;
        if(a->getInfo() == e->getInfo())
            i2 = i;
        int j = 0;
        for(list<Vertice>::iterator b = inicio(); b != final(); b++, j++)
        {

            if(i == j)
                A_1[i][j] = 0;
            else
            {
                A_1[i][j] = FLT_MAX;
                for(list<Aresta>::iterator aresta = b->inicio(); aresta != b->final(); aresta++)
                {
                    if(aresta->getExtremidade()->getInfo() == a->getInfo())
                        A_1[i][j] = aresta->getPeso();
                }
            }
        }
    }

    for(int k = 0; k < getTam(); k++)
    {
        for(int i = 0; i < getTam(); i++)
        {
            for(int j = 0; j < getTam(); j++)
            {
                A[i][j] = min(A_1[i][j], A_1[i][k] + A_1[k][j]);
            }
        }
        for(int i = 0; i < getTam(); i++)
        {
            for(int j = 0; j < getTam(); j++)
            {
                A_1[i][j] = A[i][j];
            }
        }
    }

    /*
    cout << endl;
    for(int i = 0; i < getTam(); i++)
    {
        for(int j = 0; j < getTam(); j++)
        {
            cout << A_1[i][j] << ",\t";
        }
        cout << endl;
    }
    */

    float ret = A[i1][i2];
    cout << "Distância mínima: " << ret << endl;
    for(int i = 0; i < getTam(); i++)
    {
        delete[] A_1[i];
        delete[] A[i];
    }
    delete[] A_1;
    delete[] A;
    return ret;
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
    if(digrafo)
        ss << "strict digraph {" << endl;
    else
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
            ss << "\"" << aresta->getOrigem()->getInfo() << " ( " << aresta->getOrigem()->getPeso() << (digrafo ? " )\" -> \"" : " )\" -- \"") << aresta->getExtremidade()->getInfo() << " ( " << aresta->getExtremidade()->getPeso() << " )\" ";
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
    string command1 = "dot -x -Goverlap=scale -Tpng -o"+fileName+" "+dotFileName;
    int rc = system(command1.c_str());
    if(rc != 0)
    {
        cout << "GraphViz não instalado no sistema." << endl;
        cout << "Exemplo de instalação: sudo apt-get install graphviz" << endl;
        return;
    }

    string command2 = "xdg-open "+fileName;
    rc = system(command2.c_str());
    if(rc != 0)
    {
        cout << "A imagem foi gerada mas não foi possível abrí-la" << endl;
        cout << "Abra-a manualmente pelo caminho: " << fileName << endl;
        return;
    }
}
