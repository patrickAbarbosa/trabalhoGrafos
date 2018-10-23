
#include "GulosoRandomizado.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

GulosoRandomizado::GulosoRandomizado()
{
    srand (time(NULL));
}

GulosoRandomizado::~GulosoRandomizado()
{

}

void GulosoRandomizado::calcular(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas)
{
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;

    if(grafo.getTam() == 0)
        return;

    // Cria conjunto de entrada vazio
    list<Aresta*> arestas;

    // Calcula o peso total e adiciona vértices ao conjunto de entrada
    solucao.custoTotal = 0;
    for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
    {
        solucao.custoTotal += vertice->getPeso();
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            arestas.push_back(&*aresta);
        }
    }

    solucao.custo = solucao.custoTotal;

    if(arestas.size() == 0)
    {
        Vertice *v = NULL;
        for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
        {
            if(v == NULL || v->getPeso() > vertice->getPeso())
                v = &*vertice;
        }
        if(v != NULL)
        {
            solucao.vertices.push_back(v);
            atualizaSolucao(solucao);
            return;
        }
    }
    if(alpha < 0)
        alpha = 0;
    else if(alpha > 1)
        alpha = 1;

    // Executa o algoritmo Guloso Randomizado
    SolucaoGuloso solucaoTemporaria;
    for(int i = 0; i < epocas; i++)
    {
        solucaoTemporaria.custo = solucao.custoTotal;
        algoritmoGulosoRandomizado(arestas, solucaoTemporaria, alpha);
        if(solucaoTemporaria.custo < solucao.custo)
        {
            copiaSolucao(solucaoTemporaria, solucao);
        }
    }
}

void GulosoRandomizado::copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst)
{
    dst.indices = src.indices;
    dst.vertices = src.vertices;
    dst.arestas = src.arestas;
    dst.custo = src.custo;
    dst.custoTotal = src.custoTotal;
}

void GulosoRandomizado::algoritmoGulosoRandomizado(list<Aresta*> &arestas, SolucaoGuloso &solucao, float alpha)
{
    Aresta *x; // Aresta de seleção
    list<Aresta*> C;
    geraCandidatos(arestas, C, solucao);
    //imprimirCandidatos(C);
    int index = 0;
    int limite;


    while(C.size() > 0)
    {
        limite = C.size() * alpha;
        index = rand() % limite;

        std::list<Aresta*>::iterator it = C.begin();
        std::advance(it, index);
        x = *it;

        if(solucao.vertices.size() == 0)
        {
            if(x->getOrigem()->getPeso() > x->getExtremidade()->getPeso())
                solucao.vertices.push_back(x->getOrigem());
            else
                solucao.vertices.push_back(x->getExtremidade());
        }
        else
        {
            if( (x->getExtremidade()->getPeso() - x->getPeso()) < 0)
                break;
            else
            {
                solucao.vertices.push_back(x->getExtremidade());
                if(solucao.vertices.size() > 1)
                    solucao.arestas.push_back(x);
            }
        }
        //imprimirSolucao(solucao);

        geraCandidatos(arestas, C, solucao);
        //imprimirCandidatos(C);
    }

    atualizaSolucao(solucao);
}

void GulosoRandomizado::geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao)
{
    C.clear();
    if(solucao.vertices.size() == 0)
    {
        C = arestas;
        C.sort(ordenaPrimeirosCandidatos);
    }
    else
    {
        bool valido = true;
        for(list<Aresta*>::iterator aresta = arestas.begin(); aresta != arestas.end(); aresta++)
        {
            for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
            {
                if((*aresta)->getOrigem() == *vertice)
                {
                    valido = true;
                    for(list<Vertice*>::iterator extremidade = solucao.vertices.begin(); extremidade != solucao.vertices.end(); extremidade++)
                    {
                        if((*aresta)->getExtremidade() == *extremidade)
                        {
                            valido = false;
                            break;
                        }
                    }
                    if(valido)
                        C.push_back(*aresta);
                }
            }
        }
        C.sort(ordenaCandidatos);
    }

}

void GulosoRandomizado::imprimirCandidatos(list<Aresta*> &C)
{
    cout << "C = " << C.size() << endl;
    float peso = 0;
    for(list<Aresta*>::iterator aresta = C.begin(); aresta != C.end(); aresta++)
    {
        peso = (*aresta)->getExtremidade()->getPeso() - (*aresta)->getPeso();
        cout << "(" << (*aresta)->getOrigem()->getInfo() << ", " << (*aresta)->getOrigem()->getPeso() << ") (" << (*aresta)->getExtremidade()->getInfo() << ", " << (*aresta)->getExtremidade()->getPeso() << ") " << (*aresta)->getPeso() << " | " << peso << endl;
    }
}

void GulosoRandomizado::atualizaSolucao(SolucaoGuloso &solucao)
{
    float custo = solucao.custoTotal;
    for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
    {
        solucao.indices.push_back((*vertice)->getInfo());
        custo -= (*vertice)->getPeso();
    }
    for(list<Aresta*>::iterator aresta = solucao.arestas.begin(); aresta != solucao.arestas.end(); aresta++)
    {
        custo += (*aresta)->getPeso();
    }
    solucao.custo = custo;
}

void GulosoRandomizado::imprimirSolucao(SolucaoGuloso &solucao)
{
    cout << "S = " << solucao.vertices.size() << endl;
    for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
    {
        cout << (*vertice)->getInfo() << ", ";
    }
    cout << endl;
}

void GulosoRandomizado::imprimir(SolucaoGuloso &solucao)
{
    cout << "Custo: " << solucao.custo << endl;
    cout << "Vertices (" << solucao.indices.size() << "): ";
    for (list<int>::iterator it = solucao.indices.begin() ; it != solucao.indices.end(); ++it)
    {
        cout << (*it) << ", ";
    }
    cout << endl;
    cout << "Arestas: (" << solucao.arestas.size() << ")" << endl;
    for (list<Aresta*>::iterator it = solucao.arestas.begin() ; it != solucao.arestas.end(); ++it)
    {
        cout << (*it)->getOrigem()->getInfo() << " -> " << (*it)->getExtremidade()->getInfo() << " | " << (*it)->getPeso() << endl;
    }
}

bool GulosoRandomizado::ordenaCandidatos(Aresta *primeiro, Aresta *segundo)
{
    float pesoPrimeiro = primeiro->getExtremidade()->getPeso() - primeiro->getPeso();
    float pesoSegundo = segundo->getExtremidade()->getPeso() - segundo->getPeso();
    return pesoPrimeiro > pesoSegundo;
}

bool GulosoRandomizado::ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo)
{
    float pesoPrimeiro = (primeiro->getExtremidade()->getPeso() + primeiro->getOrigem()->getPeso()) - primeiro->getPeso();
    float pesoSegundo = (segundo->getExtremidade()->getPeso() + segundo->getOrigem()->getPeso()) - segundo->getPeso();
    return pesoPrimeiro > pesoSegundo;
}
