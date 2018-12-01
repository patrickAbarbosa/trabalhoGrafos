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

// Conta espaços vazios em uma string
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
    // Se no construtor do grafo for informado o arquivo, carrega-se este arquivo e configuramos o tipo do grafo
    carregar(diretorio, digrafo);
}

Grafo::Grafo(bool digrafo)
{
    // Iniciamos um grafo vazio do tipo grafo ou digrafo
    arestas = 0;
    paperMode = false;
    this->digrafo = digrafo;
}

Grafo::~Grafo(){
    // Ao destruir o grafo, limpamos seus vértices e arestas
    vertices.clear();
    arestas = 0;
};

bool Grafo::carregar(string diretorio, bool digrafo)
{
    // Antes de carregar um grafo, apagamos os vértices e arestas atuais
    vertices.clear();
    arestas = 0;
    // Configuramos o tipo do grafo
    this->digrafo = digrafo;
    // Abrimos o arquivo de leitura
    arquivo.open(diretorio.c_str());
    if(!arquivo.is_open()) // Se não for possível abrir o arquivo, retornamos
    {
        cout << "Nao foi possivel abrir o arquivo!" << endl;
        return false;
    }
    else // Se foi possível abrir o arquivo
    {
        /*
            Iremos carregar arquivos de dois formatos diferentes, o tipo Artigo é o típo utilizado nos artigos,
            o tipo Aula é o tipo utilizado nas instâncias das aulas
        */
        string line; // Esta string irá armazenar a linha atual do arquivo
        bool nodeMode = true; // Para o tipo de arquivos Artigo, definimos se estamos na fase de carregar vértices ou arestas
        bool paperMode = false; // Indica se o arquivo atual é do tipo Artigo ou Aula
        int n = 0; // Quantidade de espaços vazios na linha atual
        while(!arquivo.eof()) // Enquanto houver dados para ler no arquivo
        {
            getline(arquivo, line); // Obtemos a próxima linha do arquivo
            if(line.size() == 0) // Se a linha é vazia, ignoramos
                continue;
            if(line[0] == '#') // Se a linha começar com # ela é um comentário, portanto, ignoramos
                continue;
            n = spaces(line); // conta a quantidade de espaços na linha atual
            if(line == "node") // Se a linha atual tiver o texto "node", as próximas linhas serão vértices e o arquivo é do tipo Artigo
            {
                nodeMode = true;
                paperMode = true;
            }
            else if(line == "link")  // Se a linha atual tiver o texto "link", as próximas linhas serão arestas e o arquivo é do tipo Artigo
            {
                nodeMode = false;
                paperMode = true;
            }
            else
            {
                if(n == 0) // Se não houver espaços em branco na linha, ela é um número e o arquivo é do tipo Aula
                {
                    paperMode = false;
                    // Criamos a quantidade de nós requisitada no arquivo
                    int sz = atoi(line.c_str());
                    for(int k = 1; k <= sz; k++)
                        adicionarVertice(k, 0);
                }
                else
                {
                    // Criamos uma stream com a linha atual, assim podemos ler cada parte da string de forma mais simples
                    stringstream ss(line);
                    if(paperMode == false) // Se o arquivo é do tipo Aula
                    {
                        if(n == 1) // Se há 1 espaço vazio na linha, temos dois números e esta linha indica uma aresta não ponderada
                        {
                            int n1, n2;
                            ss >> n1 >> n2; // Obtemos os vértices de origem e extremidade da aresta
                            adicionarAresta(n1, n2, 0);
                        }
                        else // Se há mais de 1 espaço vazio na linha, temos 3 ou mais números e esta linha indica uma aresta ponderada
                        {
                            int n1, n2;
                            float weight;
                            ss >> n1 >> n2 >> weight;// Obtemos os vértices de origem e extremidade da aresta, bem como seu peso
                            adicionarAresta(n1, n2, weight);
                        }
                    }
                    else // Se o arquivo é do tipo Artigo
                    {
                        if(nodeMode) // Se devemos carregar um nó do arquivo
                        {
                            int id;
                            float h, type, weight;
                            ss >> id >> h >> type >> weight; // Carregamos a identificação, o parâmetro h, o tipo o peso.
                            adicionarVertice(id, weight); // No nosso grafo só utilizamos a identificação e o peso
                        }
                        else // Se devemos carregar uma aresta do arquivo
                        {
                            int id, n1, n2;
                            float weight;
                            ss >> id >> n1 >> n2 >> weight; // Obtemos a identificação da aresta, o vértice inicial, o vértice final e o peso
                            adicionarAresta(n1, n2, weight); // No nosso grafo só utilizamos o nó de início, final e o peso
                        }
                    }
                }
            }
        }
        // Depois de ler todo o arquivo, podemos fechá-lo
        arquivo.close();
        this->paperMode = paperMode;
    }
    return true;
}

bool Grafo::salvar(string diretorio)
{
    // Abrimos um arquivo de escrita
    ofstream arquivo;
    arquivo.open(diretorio.c_str());
    if(!arquivo.is_open()) // Se não for possível abrir o arquivo, retornamos
    {
        cout << "Nao foi possivel abrir o arquivo!" << endl;
        return false;
    }
    else // Se foi possível abrir o arquivo
    {
        // Se o grafo foi carregado no modo aula, escrevemos a quantidade de vértices no arquivo
        if(!paperMode)
        {
            arquivo << getTam() << endl;
        }
        else // Se o grafo foi carregado no modo Artigo, escrevemos o peso e identificação de cada vértice, bem como os textos de identificação
        {
            arquivo << "node" << endl;
            for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); it++)
            {
                arquivo << it->getInfo() << "\t0\t0\t" << it->getPeso() << endl;
            }
            arquivo << "link" << endl;
        }
        // Para cada vértice no grafo percorremos sua lista de arestas e escrevemos no arquivo
        int n = 0;
        for(list<Vertice>::iterator V = inicio(); V != final(); V++)
        {
            for(list<Aresta>::iterator aresta = V->inicio(); aresta != V->final(); aresta++)
            {
                if(aresta->hide)
                    continue;
                if(paperMode)
                    arquivo << n << "\t" << V->getInfo() << "\t" << aresta->getExtremidade()->getInfo() << "\t" << aresta->getPeso() << endl;
                else
                    arquivo << V->getInfo() << "\t" << aresta->getExtremidade()->getInfo() << "\t" << aresta->getPeso() << endl;
                n++;
            }
        }
        // Fechamos o arquivo
        arquivo.close();
    }
    return true;
}

void Grafo::adicionarVertice(int info, float peso){
    // Antes de adicionar um vértice ao grafo, verificamos se ele já existe
    if(busca(info) != NULL){
        // Se o vértice já existe não fazemos nada
        return;
    }
    // Criamos um novo vértice
    Vertice novoVertice;
    // Configuramos a identificação do vértice
    novoVertice.setInfo(info);
    // Configuramos o peso do vértice
    novoVertice.setPeso(peso);
    // Adicionamos o vértice na lista de vértices do grafo
    this->vertices.push_front(novoVertice);
}

void Grafo::removerVertice(int info){
    // Para remover um vértice do grafo, iteramos entre todos os vértices
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end();){
        // Removemos todas as arestas de qualquer vértice que aponte para o vértice que desejamos remover
        it->removerAresta(info);
        // Se o vértice atual for o que se deseja remover, o apagamos da lista
        if(it->getInfo() == info)
        {
            it = vertices.erase(it);
        }
        else
            ++it;
    }
}

void Grafo::adicionarAresta(int infoA, int infoB, float peso){
    // Para adicionar uma aresta ao grafo, inicialmente encontramos os vérticem com as identificações desejadas para a aresta
    Vertice *extremidade = busca(infoB);
    Vertice *origem = busca(infoA);
    // Se um dos vértices não pode ser encontrado'então não é possível adicionar esta aresta, apenas retornamos
    if(extremidade == NULL || origem == NULL){
        return;
    }

    // Adicionamos uma aresta do vértice de origem para o vértice da extremidade
    origem->adicionarAresta(extremidade, peso);
    // Se este grafo não é um digrafo, adicionamos uma aresta identica da extremidade para a origem, mas esta aresta não aparece quando imprimimos o grafo
    if(!digrafo)
        extremidade->adicionarAresta(origem, peso, true);
    // Incrementamos a quantidade de arestas do grafo
    arestas++;
}

void Grafo::removerAresta(int infoA, int infoB){
    // Para remover uma aresta, encontramos o vértice desejado
    Vertice *origem = busca(infoA);
    if(origem == NULL) // Se o vértice não foi encontrado, retornamos
        return;
    // Removemos a aresta do vértice de origem para o vértice da extremidade
    origem->removerAresta(infoB);
    // Se o grafo não é um digrafo, devemos remover a aresta identica da extremidade para a origem
    if(!digrafo)
    {
        // Encontramos o vértice da extremidade
        Vertice *extremidade = busca(infoB);
        if(extremidade != NULL) // Se o vértice da extremidade foi encontrado
            extremidade->removerAresta(infoA); // Removemos a aresta da extremidade para a origem
    }
    // Decrementamos a quantidade de arestas
    arestas--;

}
unsigned int Grafo::getTam(){
    // Retornamos a ordem do grafo utilizando o tamanho da lista de vétices do grafo
    return (unsigned  int) vertices.size();
}

unsigned int  Grafo::getGrau(int info){
    // Para retornar o grau de um vértice, encontramos este vértice
    Vertice *v = busca(info);
    if(v == NULL)// Se o vértice não foi encontrado, retornamos um grau nulo
        return 0;
    // Retornamos o grau para o vértice encontrado
    return (unsigned int) v->getGrau();
}

bool Grafo::getGrau(int info, int *gIn, int *gOut)
{
    // Para retornar o grau de um vértice, encontramos este vértice
    Vertice *v = busca(info);
    if(v == NULL)// Se o vértice não foi encontrado, retornamos falha
        return false;
    *gOut = v->getGrau();
    if(!digrafo)
        *gIn = -1;
    else
    {
        int g = 0;
        for(list<Vertice>::iterator B = inicio(); B != final(); B++)
        {
            for(list<Aresta>::iterator aresta = B->inicio(); aresta != B->final(); aresta++)
            {
                if(aresta->getExtremidade() == v)
                {
                    g++;
                }
            }
        }
        *gIn = g;
    }
}

unsigned int Grafo::getArestas(){
    // Retornamos a quantidade de arestas do grafo
    return (unsigned  int) arestas;
}

bool Grafo::kRegular(int k)
{
    // Para verificar se o grafo é K-regular, iteremos entre todos os vértices do grafo
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        // Se o grau de qualquer vértice for diferente de K, ele não é K-regular
        if(vertice->getGrau() != k)
            return false;
    }
    // Se nenhum vértice tem grau diferente de K, ele é K-regular
    return true;
}

void Grafo::vizinhanca(int info, bool fechada, list<int> &resultado)
{
    // Para verificar a vizinhança, encontramos o vértice desejado
    Vertice *v = busca(info);
    if(v == NULL) // Se o vértice não foi encontrado, retornamos
    {
        cout << "Vértice inválido" << endl;
        return;
    }
    resultado.clear();
    cout << "Vizinhança: ";
    if(fechada) // Se a vizinhança é fechada, o vértice faz parte da sua vizinhança
    {
        cout << info << " ";
        resultado.push_back(info);
    }
    // Para cada aresta do vértice selecionado, sua extremidade faz parte de sua vizinhança
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        cout << ", " << aresta->getExtremidade()->getInfo();
        resultado.push_back(aresta->getExtremidade()->getInfo());
    }
    cout << endl;
}

bool Grafo::completo()
{
    // Para um grafo completo, todos os nós devem ter o mesmo grau e ele deve ser igual à ordem do grafo subtraido de uma unidade
    int n = getTam();
    n--;
    // Para cada vértice do grafo
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        // Se o grau de qualquer vértice for diferente de n, este grafo não é completo
        if(vertice->getGrau() != n)
            return false;
    }
    return true;
}

bool Grafo::bipartido()
{
    // Para verificar se o grafo é bipartido, utilizamos a variável auxiliar 1 do vértice para marcá-lo
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
        vertice->aux1 = -1;

    // Percorremos toda a lista de vértices
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        // Se o vértice atual não foi marcado ainda, marcamos ele e seus adjacentes com a marcação 0 recursivamente
        if(vertice->aux1 == -1)
        {
            if(!bipartido(&*vertice, 0)) // Se na recursividade houve conflito entre a marcação de um nó, este grafo não é bipartido
                return false;
        }
    }
    return true;
}

bool Grafo::bipartido(Vertice *v, int proximo)
{
    // Marcamos o vértice atual com a marcação 'proximo'
    v->aux1 = proximo;
    // Para cada aresta deste vértice
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        // Se extremidade da aresta não foi marcada ainda
        if(aresta->getExtremidade()->aux1 == -1)
        {
            // marcamos ele e seus adjacentes com o complementar de 'proximo' recursivamente
            if(!bipartido(aresta->getExtremidade(), !proximo)) // Se na recursividade houve conflito entre a marcação de um nó, este grafo não é bipartido
                return false;
        }
        else if(aresta->getExtremidade()->aux1 == proximo) // Se a extremidade já está marcada com a mesma marcação do vértice atual, o grafo'não é bipartido
        {
            return false;
        }
    }
    return true;
}

void Grafo::sequenciaGraus(list<int> &resultado)
{
    resultado.clear();
    // Para a sequência de graus, adicionamos a grau de cada vértice do grafo à uma lista
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
        resultado.push_back(vertice->getGrau());
    // Ordenamos a lista de graus de forma à obter uma sequência do menor para o maior
    resultado.sort();
    cout << "Sequência de graus: ";
    for(list<int>::iterator grau = resultado.begin(); grau != resultado.end(); grau++)
        cout << (*grau) << ", ";
    cout << endl;
}

bool Grafo::ordenacaoTopologica(list<int> &resultado)
{
    // Para obter a ordenação topológica, devemos ter obrigatóriamente um digrafo, portanto retornamos se este grafo não for um
    if(!digrafo)
    {
        cout << "Este grafo não é um digrafo" << endl;
        return false;
    }
    // Criamos duas listas, a lista L armazena os vértices ordenados e a lista S armazena os nós sem arestas de entrada
    list<Vertice*> S, L;
    // Colocamos todos os vértices sem arestas de entrada em S
    // Utilizamos a variável auxiliar 1 para contar a quantidade de arestas de entrada de cada vértice
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

    // Para cada vértice sem arestas, removemos de S e colocamos em L
    while(S.size() > 0)
    {
        Vertice *n = *S.begin();
        S.erase(S.begin());
        L.push_back(n);
        // Para cada aresta do vértice n, marcadmos esta aresta como visitada
        for(list<Aresta>::iterator aresta = n->inicio(); aresta != n->final(); aresta++)
        {
            // Para o vértice da extremidade desta aresta, decrementamos a quantidade de arestas (aux1)
            aresta->aux1 = 1;
            aresta->getExtremidade()->aux1--;
            // Se o vértice da extremidade não tem mais arestas, colocamos-o em S para ser analisado
            if(aresta->getExtremidade()->aux1 == 0)
                S.push_front(aresta->getExtremidade());
        }
    }

    // Se alguma aresta do grafo não foi visitada, significa que o grafo tem ao menos um ciclo e a ordenação topológica é invalida
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
    // O fecho transitivo direto ou indireto só pode ser analisado em digrafos
    if(!digrafo)
    {
        cout << "Este grafo não é um digrafo" << endl;
        return false;
    }
    // Se o vértice requisitado não existe, retornamos
    Vertice *v = busca(V);
    if(v == NULL)
    {
        cout << "Vértice inválido" << endl;
        return false;
    }
    // Armazenamos o resultado do fecho transitivo direto ou indireto na lista de vértices S
    list<Vertice*> S;
    if(direto)
        fechoTransitivoDireto(v, S); // procura o conjunto de todos os vértices que podem ser atingidos por algum caminho iniciando em v
    else
        fechoTransitivoIndireto(v, S); // procura o  conjunto de todos os vértices a partir dos quais se pode atingir v por algum caminho.

    // Apresentamos o resultado
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
    // Se o vértice já existe na solução, retornamos
    if(find(S.begin(), S.end(), v) != S.end())
        return;
    // Adicionamos o vértice atual na solução
    S.push_back(v);
    // Percorremos a lista de arestas deste vértice e analisamos os outros vértices que podem ser atingidos partindo de v
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        fechoTransitivoDireto(aresta->getExtremidade(), S);
    }
}

void Grafo::fechoTransitivoIndireto(Vertice *v, list<Vertice*> &S)
{
    // Se o vértice já existe na solução, retornamos
    if(find(S.begin(), S.end(), v) != S.end())
        return;
    // Adicionamos o vértice atual na solução
    S.push_back(v);
    // Para todos os vértices do grafo, verificamos se existe algum que tem aresta para o vértice v
    for(list<Vertice>::iterator B = inicio(); B != final(); B++)
    {
        if(v == &*B)
            continue;
        for(list<Aresta>::iterator aresta = B->inicio(); aresta != B->final(); aresta++)
        {
            if(aresta->getExtremidade() == v)
            {
                fechoTransitivoIndireto(aresta->getOrigem(), S); // Se houver uma aresta de algum vértice para v, analisamos a origem desta aresta
            }
        }
    }
}

void Grafo::kruskal()
{
    // Criamos uma lista de todas as arestas do grafo
    list<Aresta*> S;
    for(list<Vertice>::iterator A = inicio(); A != final(); A++)
    {
        // Cada vértice é uma árvore um único vértice
        A->aux1 = A->getInfo();
        for(list<Aresta>::iterator aresta = A->inicio(); aresta != A->final(); aresta++)
        {
            aresta->aux1 = -1; // Marcamos todas as arestas como não pertencentes à nenhum árvore
            S.push_back(&*aresta);
        }
    }

    // Ordenamos as arestas por seus pesos
    S.sort(ordenaArestas);
    // Para todas as arestas da lista
    for(list<Aresta*>::iterator s1 = S.begin(); s1 != S.end(); s1++)
    {
        // Verificamos se a origem e a extremidade pertencem à mesma árvore
        Vertice *v1 = (*s1)->getOrigem();
        Vertice *v2 = (*s1)->getExtremidade();
        // Se os dois vértices não estão na mesma árvore, une as árvores
        if(v1->aux1 != v2->aux1)
        {
            // Faz com que esta aresta pertença àrvore do vértice de origem
            (*s1)->aux1 = v1->aux1;
            // Faz com que cada aresta da árvore no vértice da extremidade pertença à arvore do vértice de destino
            for(list<Aresta*>::iterator s2 = S.begin(); s2 != S.end(); s2++)
            {
                if((*s2)->aux1 == v2->aux1)
                    (*s2)->aux1 = v1->aux1;
            }
            // Faz com que o vértice da extremidade pertença à árvore do vértice de origem
            v2->aux1 = v1->aux1;
        }
    }

    // Mapeia todas as AGM encontradas
    map<int, list<Aresta*> > Fs;
    for(list<Aresta*>::iterator s = S.begin(); s != S.end(); s++)
    {
        if((*s)->aux1 >= 0)
        {
            Fs[(*s)->aux1].push_back(*s);
        }
    }
    // Verifica se mais de uma AGM foi encontrada
    if(Fs.size() > 1)
    {
        cout << "Este grafo contém um floresta de AGMs" << endl;
        cout << "Floresta -----" << endl;
    }
    bool showN = Fs.size() > 1;
    int n = 1;
    // Exibe as AGMs
    for(map<int, list<Aresta*> >::iterator it = Fs.begin(); it != Fs.end(); ++it, n++)
    {
        if(showN)
            cout << "AGM: " << n << endl;
        else
            cout << "AGM" << endl;
        list<Aresta*> &A = it->second;
        for(list<Aresta*>::iterator a = A.begin(); a != A.end(); a++)
        {
            cout << "( " << (*a)->getOrigem()->getInfo() << ", " << (*a)->getOrigem()->getPeso() << " ) ( " << (*a)->getExtremidade()->getInfo() << ", " << (*a)->getExtremidade()->getPeso() << " ) " << (*a)->getPeso() << endl;
        }
    }
}

void Grafo::prim()
{
    // Criamos um novo grafo para armazenar AGM por Prim
    Grafo solucao;
    solucao.digrafo = digrafo; // Se o grafo original for um digrafo, este também será
    Vertice *v = NULL;
    // Criamos uma lista com todas as arestas do grafo
    list<Aresta*> S;
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        // Selecionamos o vértice de maior grau
        vertice->aux1 = 0;
        if(v == NULL)
            v = &*vertice;
        else if(vertice->getGrau() > v->getGrau())
            v = &*vertice;
        // Adicionamos todas as erestas na lista de arestas
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            // Marcamos esta aresta como não visitada
            aresta->aux1 = 0;
            S.push_back(&*aresta);
        }
    }
    // Adicionamos o vértice de maior grau na solução
    solucao.adicionarVertice(v->getInfo(), v->getPeso());
    // Procuramos a AGM recursivamente
    prim(solucao, S);
    // Exibimos a solução
    cout << "AGM" << endl;
    solucao.imprimir();
}

void Grafo::prim(Grafo &solucao, list<Aresta*> &S)
{
    Aresta* C = NULL;
    bool origem = false;
    // Para todas as arestas da lista de arestas
    for(list<Aresta*>::iterator s = S.begin(); s != S.end(); s++)
    {
        if((*s)->aux1 == 1) // A a aresta já foi visitada, ignoramos a mesma
            continue;
        Vertice *v1 = solucao.busca((*s)->getOrigem()->getInfo());
        Vertice *v2 = solucao.busca((*s)->getExtremidade()->getInfo());
        // Se os dois vértices que esta aresta liga não estão na solução, ignoramos a mesma
        if(v1 == NULL && v2 == NULL)
        {
            continue;
        }
        else if(v1 != NULL && v2 != NULL) // Se os dois vértices que esta aresta liga estão na solução, marcamos-a como visitada
        {
            (*s)->aux1 = 1;
        }
        else // Se apenas um dos vértices está na solução
        {
            // Salva a indicação de qual vértice da aresta deve ser adicionada na solução
            origem = v1 != NULL;
            // Selecionamos esta aresta para entrar na solução se não há outra selecionada ou se esta tem o peso menor que a atualmente selecionada
            if(C == NULL)
                C = *s;
            else if((*s)->getPeso() < C->getPeso())
                C = *s;
        }
    }
    if(C == NULL) // Se nenhuma aresta foi selecionada para entrar na solução, terminamos
        return;
    C->aux1 = 1; // Marcamos esta aresta como visitada
    // Adicionamos na solução o vértice da aresta selecionada que não estava na solução
    if(origem)
        solucao.adicionarVertice(C->getOrigem()->getInfo(), C->getOrigem()->getPeso());
    else
        solucao.adicionarVertice(C->getExtremidade()->getInfo(), C->getExtremidade()->getPeso());
    // Adicionamos a aresta selecionada na solução
    solucao.adicionarAresta(C->getOrigem()->getInfo(), C->getExtremidade()->getInfo(), C->getPeso());
    // Procuramos a pŕoxima aresta a entrar na solução
    prim(solucao, S);
}

bool Grafo::ordenaArestas(Aresta *a, Aresta *b)
{
    // Retorna true se a primeira aresta tiver o peso menor que a segunda
    return a->getPeso() < b->getPeso();
}

float Grafo::dijkstra(int v1, int v2)
{
    // Para Dijkstra, verificamos se os vértices solicitados são validos
    Vertice *s = busca(v1);
    Vertice *e = busca(v2);
    if(s == NULL || e == NULL)
    {
        cout << "Vértices inválidos" << endl;
        return -1;
    }
    // Atribuimos o custo inicial como máximo para todos os vértices, bem como indicamos que ele não foi visitado (aux1)
    for(list<Vertice>::iterator vertice = inicio(); vertice != final(); vertice++)
    {
        vertice->aux3 = FLT_MAX;
        vertice->aux1 = -1;
    }
    // Indicamos que o vértice de partida foi visitado
    s->aux3 = 0;
    // Começamos a caminhar no grafo saindo do vértice de partida
    dijkstra(s, 0);
    // Exibimos a distância mínima do vértice de partida até o vértice de chegada
    cout << "Distância mínima: " << e->aux3 << endl;
    return e->aux3;
}

void Grafo::dijkstra(Vertice *v, float l)
{
    Vertice *n = NULL;
    float d;
    // Para caminhar no grafo, verificamos todas as arestas que saem do vértice atual
    for(list<Aresta>::iterator aresta = v->inicio(); aresta != v->final(); aresta++)
    {
        // Se a extremidade desta aresta não foi visitada, analisamos
        if(aresta->getExtremidade()->aux1 < 0)
        {
            // O custo para ir até o vértice da extremidade será o mínimo entre o custo já conhecido até este vértice e custo da aresta mais o custo do caminho até o momento
            aresta->getExtremidade()->aux3 = min(aresta->getExtremidade()->aux3, aresta->getPeso() + l);
            if(n == NULL) // Se não há aresta selecionada para ser o próximo passo no grafo, selecionamos esta
            {
                n = aresta->getExtremidade();
                d = aresta->getPeso();
            }
            else if(aresta->getPeso() < d) // Se a aresta tem um custo menor do que a selecionada como próximo passo anteriormente, selecionamos esta
            {
                n = aresta->getExtremidade();
                d = aresta->getPeso();
            }
        }
    }
    if(n != NULL) // Se alguma aresta foi selecionada para ser o próximo passo, marcamos esta aresta e caminhamos sobre ela
    {
        n->aux1 = 1;
        dijkstra(n, n->aux3);
    }
}

float Grafo::floyd(int v1, int v2)
{
    // Verificamos se os vértices requisitados são válidos
    Vertice *s = busca(v1);
    Vertice *e = busca(v2);
    if(s == NULL || e == NULL)
    {
        cout << "Vértices inválidos" << endl;
        return -1;
    }
    float **A_1;
    float **A;

    // Criamos a matriz de custos atual e a da iteração passada
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
    // Analisamos os vértices do grafo para preencher a matriz de custos inicial
    for(list<Vertice>::iterator a = inicio(); a != final(); a++, i++)
    {
         // Se o vértice atual é o vértice de partida, o armazenamos o índice deste vértice (para não ter que procurar depois)
        if(a->getInfo() == s->getInfo())
            i1 = i;
         // Se o vértice atual é o vértice de chegada, o armazenamos o índice deste vértice (para não ter que procurar depois)
        if(a->getInfo() == e->getInfo())
            i2 = i;
        int j = 0;
        // Percorremos novamente o vetor de vértices
        for(list<Vertice>::iterator b = inicio(); b != final(); b++, j++)
        {
            // Se os dois vértices forem iguais, o custo entre eles é zero
            if(i == j)
                A_1[i][j] = 0;
            else
            {
                // Se os vértices forem diferentes, eles tem um custo inicialmente infinito
                A_1[i][j] = FLT_MAX;
                // Se existe uma aresta entre estes vértices, o custo é o peso desta aresta
                for(list<Aresta>::iterator aresta = b->inicio(); aresta != b->final(); aresta++)
                {
                    if(aresta->getExtremidade()->getInfo() == a->getInfo())
                        A_1[i][j] = aresta->getPeso();
                }
            }
        }
    }

    // Teremos uma iteração para cada vértice do grafo
    for(int k = 0; k < getTam(); k++)
    {
        // Percorremos a matriz de custos
        for(int i = 0; i < getTam(); i++)
        {
            for(int j = 0; j < getTam(); j++)
            {
                // O custo da célula atual será o custo mínimo entre o custo desta célula na iteração
                // passada e o custo para se chegar até esta célula desde a iteração passada
                A[i][j] = min(A_1[i][j], A_1[i][k] + A_1[k][j]);
            }
        }
        // Copiamos a tabela de custos encontrada para a tabela de custos da geração passada
        for(int i = 0; i < getTam(); i++)
        {
            for(int j = 0; j < getTam(); j++)
            {
                A_1[i][j] = A[i][j];
            }
        }
    }

    float ret = A[i1][i2]; // O custo do caminho entre os dois vértices será a célula da matriz que liga os dois vértices
    cout << "Distância mínima: " << ret << endl;
    // Liberamos os rescursos utilizados nos cálculos
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
    // Para encontrar um vértice pela sua identificação, percorremos a lista de vértices e retornamos quando encontramos algum vértice com a identificação correta
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        if(it->getInfo() == info) {
            return &(*it);
        }
    }
    // Retornamos nulo se não encontramos o vértice
    return NULL;
}

void Grafo::imprimir() {
    // Imprimimos o grafo, para isso, imprimimos cada vértice
    for (list<Vertice>::iterator it=this->vertices.begin() ; it != this->vertices.end(); ++it){
        it->imprimir();
    }
}

list<Vertice>::iterator Grafo::inicio(){
    // retornamos um iterador para o inicio da lista de vértices
    return this->vertices.begin();
}

list<Vertice>::iterator Grafo::final(){
    // retornamos um iterador para o final da lista de vértices
    return this->vertices.end();
}

void Grafo::draw(string fileName, SolucaoGuloso *solucao)
{
    // Para desenhar o grafo, inicialmente criamos um arquivo no formato dot (utilizado pelo graphviz)
    // Neste arquivo colocamos todas as informações do grafo atual, e se houver alguma solução a ser mostrada, marcamos os vértices e arestas que fazem parte da solução
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

    // Salvamos o arquivo no formato dot
    cout << "Renderizando" << endl;
    string dotFileName = fileName+".dot";
    ofstream dot(dotFileName.c_str());
    dot << ss.rdbuf();
    dot.close();
    // Invocamos o graphviz para desenhar o grafo
    string command1 = "dot -x -Goverlap=scale -Tpng -o"+fileName+" "+dotFileName;
    int rc = system(command1.c_str());
    if(rc != 0) // Se o graphviz não está instalado, exibimos ajuda para instalar
    {
        cout << "GraphViz não instalado no sistema." << endl;
        cout << "Exemplo de instalação: sudo apt-get install graphviz" << endl;
        return;
    }

    // Incocamos o software de visualização de imangens padrão para visualizar o grafo desenhado
    string command2 = "xdg-open "+fileName;
    rc = system(command2.c_str());
    if(rc != 0)
    {
        cout << "A imagem foi gerada mas não foi possível abrí-la" << endl;
        cout << "Abra-a manualmente pelo caminho: " << fileName << endl;
        return;
    }
}
