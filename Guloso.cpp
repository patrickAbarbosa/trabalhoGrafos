
#include "Guloso.h"
#include <iostream>
#include <cfloat>

Guloso::Guloso()
{

}

Guloso::~Guloso()
{

}

void Guloso::calcular(Grafo &grafo, SolucaoGuloso &solucao)
{
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

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
    solucao.alpha = 0;
    solucao.iteracao = 0;

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

    // Executa o algoritmo guloso
    algoritmoGuloso(arestas, solucao);
}

void Guloso::calcularRandomizado(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas)
{
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

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
    solucao.alpha = alpha;

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
        cout << "Epoca: " << i << endl;
        solucaoTemporaria.custoTotal = solucao.custoTotal;
        solucaoTemporaria.arestas.clear();
        solucaoTemporaria.vertices.clear();
        solucaoTemporaria.indices.clear();
        solucaoTemporaria.custo = 0;
        solucaoTemporaria.alpha = alpha;
        solucaoTemporaria.iteracao = i+1;

        algoritmoGulosoRandomizado(arestas, solucaoTemporaria, alpha);
        if(solucaoTemporaria.custo < solucao.custo)
        {
            copiaSolucao(solucaoTemporaria, solucao);
        }
    }
}

void Guloso::calcularRandomizadoReativo(Grafo &grafo, SolucaoGuloso &solucao, float *alpha, int nAlphas, int bloco, int epocas)
{
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

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
            solucao.alpha = 0;
            atualizaSolucao(solucao);
            return;
        }
    }
    for(int i = 0; i < nAlphas; i++)
    {
        if(alpha[i] < 0)
            alpha[i] = 0;
        else if(alpha[i] > 1)
            alpha[i] = 1;
    }

    // Executa o algoritmo Guloso Randomizado
    SolucaoGuloso solucaoTemporaria;
    int n = 0, roleta = 0;
    float *p = new float[nAlphas];
    float *q = new float[nAlphas];
    float *A = new float[nAlphas];
    float *a = new float[nAlphas];
    float sum;
    float sum_q;
    p[0] = 1.0f / nAlphas;
    A[0] = 0;
    a[0] = 0;
    for(int i = 1; i < nAlphas; i++)
    {
        p[i] = p[0];
        A[i] = 0;
        a[i] = 0;
    }

    for(int i = 1,  k = 1; i <= epocas; i++)
    {
        cout << "Epoca: " << i << endl;
        roleta = rand() % 100;
        sum = 0;
        n = 0;
        while(n < nAlphas)
        {
            sum += p[n] * 100.0f;
            if(sum > roleta)
                break;
            n++;
        }

        solucaoTemporaria.custoTotal = solucao.custoTotal;
        solucaoTemporaria.arestas.clear();
        solucaoTemporaria.vertices.clear();
        solucaoTemporaria.indices.clear();
        solucaoTemporaria.custo = 0;
        solucaoTemporaria.alpha = alpha[n];
        solucaoTemporaria.iteracao = i;

        algoritmoGulosoRandomizado(arestas, solucaoTemporaria, alpha[n]);
        if(solucaoTemporaria.custo < solucao.custo)
        {
            copiaSolucao(solucaoTemporaria, solucao);
        }
        cout << "Solucao: " << solucaoTemporaria.custo << " ( " << solucaoTemporaria.alpha << " )" << endl;
        cout << "Melhor solucao: " << solucao.custo << " ( " << solucao.alpha << " )"  << endl;
        a[n] += solucaoTemporaria.custo;

        if(i % bloco == 0)
        {
            sum_q = 0;
            for(int j = 0; j < nAlphas; j++)
            {
                //A[j] += a[j] / (float)bloco;
                A[j] = a[j] / i;
                //q[j] = A[j] > 0 ? solucao.custo / (A[j] / k) : 0;
                q[j] = A[j] > 0 ? solucao.custo / A[j] : FLT_MIN;
                sum_q += q[j];
                //a[j] = 0;
            }
            cout << "P: ";
            for(int j = 0; j < nAlphas; j++)
            {
                p[j] = q[j] / sum_q;
                cout << p[j] << ", ";
            }
            cout << endl;
            k++;
        }
    }
    delete[] p;
    delete[] q;
    delete[] A;
    delete[] a;
}

void Guloso::algoritmoGuloso(list<Aresta*> &arestas, SolucaoGuloso &solucao)
{
    Aresta *x; // Aresta de seleção
    list<Aresta*> C;
    geraCandidatos(arestas, C, solucao);
    //imprimirCandidatos(C);

    while(C.size() > 0)
    {
        x = C.front();
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
            {
                if( (x->getOrigem()->getPeso() - x->getPeso()) < 0)
                    break;
                else
                {
                    solucao.vertices.push_back(x->getExtremidade());
                    if(solucao.vertices.size() > 1)
                        solucao.arestas.push_back(x);
                }
            }
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

void Guloso::copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst)
{
    dst.indices = src.indices;
    dst.vertices = src.vertices;
    dst.arestas = src.arestas;
    dst.custo = src.custo;
    dst.alpha = src.alpha;
    dst.iteracao = src.iteracao;
}

void Guloso::algoritmoGulosoRandomizado(list<Aresta*> &arestas, SolucaoGuloso &solucao, float alpha)
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
        index = rand() % (limite + 1);

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
            {
                if( (x->getOrigem()->getPeso() - x->getPeso()) < 0)
                    break;
                else
                {
                    solucao.vertices.push_back(x->getExtremidade());
                    if(solucao.vertices.size() > 1)
                        solucao.arestas.push_back(x);
                }
            }
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
    //imprimir(solucao);
}

void Guloso::geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao)
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

void Guloso::imprimirCandidatos(list<Aresta*> &C)
{
    cout << "C = " << C.size() << endl;
    float peso = 0;
    for(list<Aresta*>::iterator aresta = C.begin(); aresta != C.end(); aresta++)
    {
        peso = (*aresta)->getExtremidade()->getPeso() - (*aresta)->getPeso();
        cout << "(" << (*aresta)->getOrigem()->getInfo() << ", " << (*aresta)->getOrigem()->getPeso() << ") (" << (*aresta)->getExtremidade()->getInfo() << ", " << (*aresta)->getExtremidade()->getPeso() << ") " << (*aresta)->getPeso() << " | " << peso << endl;
    }
}

void Guloso::atualizaSolucao(SolucaoGuloso &solucao)
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

void Guloso::imprimirSolucao(SolucaoGuloso &solucao)
{
    cout << "S = " << solucao.vertices.size() << endl;
    for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
    {
        cout << (*vertice)->getInfo() << ", ";
    }
    cout << endl;
}

void Guloso::imprimir(SolucaoGuloso &solucao, Grafo &grafo, float best)
{
    cout << "Custo: " << solucao.custo << endl;
    cout << "Custo Máximo: " << solucao.custoTotal << endl;
    cout << "Relação Custo Máximo " << ((solucao.custo/solucao.custoTotal)*100.0f) << "%" << endl;
    if(best > 0)
    {
        float percent = ((solucao.custo - best)/best)*100.0f;
        cout << "Relação Melhor Custo: " << percent << "%" << endl;
    }
    if(solucao.alpha > 0)
        cout << "Alpha: " << solucao.alpha << endl;
    cout << "Iteração: " << solucao.iteracao << endl;
    cout << "Vertices (" << grafo.getTam() << "): " << solucao.indices.size() << endl;
    /*
    for (list<int>::iterator it = solucao.indices.begin() ; it != solucao.indices.end(); ++it)
    {
        cout << (*it) << ", ";
    }
    */
    //cout << endl;
    cout << "Arestas (" << grafo.getArestas() << "): " << solucao.arestas.size() << endl;
    /*for (list<Aresta*>::iterator it = solucao.arestas.begin() ; it != solucao.arestas.end(); ++it)
    {
        cout << (*it)->getOrigem()->getInfo() << " -> " << (*it)->getExtremidade()->getInfo() << " | " << (*it)->getPeso() << endl;
    }*/
}

bool Guloso::ordenaCandidatos(Aresta *primeiro, Aresta *segundo)
{
    float pesoPrimeiro;
    if(primeiro->getExtremidade()->getPeso() > primeiro->getOrigem()->getPeso())
        pesoPrimeiro = primeiro->getExtremidade()->getPeso() - primeiro->getPeso();
    else
        pesoPrimeiro = primeiro->getOrigem()->getPeso() - primeiro->getPeso();
    float pesoSegundo;
    if(segundo->getExtremidade()->getPeso() > segundo->getOrigem()->getPeso())
        pesoSegundo= segundo->getExtremidade()->getPeso() - segundo->getPeso();
    else
        pesoSegundo= segundo->getOrigem()->getPeso() - segundo->getPeso();
    return pesoPrimeiro > pesoSegundo;
}

bool Guloso::ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo)
{
    float pesoPrimeiro = (primeiro->getExtremidade()->getPeso() + primeiro->getOrigem()->getPeso()) - primeiro->getPeso();
    float pesoSegundo = (segundo->getExtremidade()->getPeso() + segundo->getOrigem()->getPeso()) - segundo->getPeso();
    return pesoPrimeiro > pesoSegundo;
}
