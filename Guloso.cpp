
#include "Guloso.h"
#include <iostream>
#include <cfloat>
#include <cstdlib>

Guloso::Guloso()
{
    // Não temos nada para fazer no construtor desta classe
}

Guloso::~Guloso()
{
    // Não temos nada para fazer no destrutor desta classe
}

void Guloso::calcular(Grafo &grafo, SolucaoGuloso &solucao)
{
    // Para calcular a solução gulosa de um grafo para o problema de Steiner com coleta, inicialmente garantimos que a solução está vazia
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

    // Se o grafo não tem vértices, retornamos a solução vazia
    if(grafo.getTam() == 0)
        return;

    // Cria conjunto de arestas vazio
    list<Aresta*> arestas;

    // Calcula o custo total (soma dos pesos dos vértices) e adiciona arestas ao conjunto de candidatos
    solucao.custoTotal = 0;
    for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
    {
        solucao.custoTotal += vertice->getPeso();
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            arestas.push_back(&*aresta);
        }
    }

    // Inicialmente o custo da solução é máximo
    solucao.custo = solucao.custoTotal;
    // No algoritmo guloso padrão não utilizamos o alfa, logo, é como se ele fosse igual a 0
    solucao.alpha = 0;
    // No algoritmo guloso padrão efetuamos uma única iteração pois o resultado será sempre igual, portanto a iteração do resultado é a 0
    solucao.iteracao = 0;

    // Se não existem arestas na lista de candidatos, o grafo não possúi arestas
    if(arestas.size() == 0)
    {
        // Encontramos o vértice de maior peso e adicionamos na solução
        Vertice *v = NULL;
        for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
        {
            if(v == NULL || v->getPeso() > vertice->getPeso())
                v = &*vertice;
        }
        if(v != NULL)
        {
            solucao.vertices.push_back(v);
            // Atualizamos a solução para o único vértice e retornamos
            atualizaSolucao(solucao);
            return;
        }
    }

    // Executa o algoritmo guloso
    algoritmoGuloso(arestas, solucao);
}

void Guloso::calcularRandomizado(Grafo &grafo, SolucaoGuloso &solucao, float alpha, int epocas)
{
    // Para calcular a solução gulosa randomizada de um grafo para o problema de Steiner com coleta, inicialmente garantimos que a solução está vazia
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

    // Se o grafo não tem vértices, retornamos a solução vazia
    if(grafo.getTam() == 0)
        return;

    // Cria conjunto de arestas vazio
    list<Aresta*> arestas;

    // Calcula o custo total (soma dos pesos dos vértices) e adiciona arestas ao conjunto de candidatos
    solucao.custoTotal = 0;
    for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
    {
        solucao.custoTotal += vertice->getPeso();
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            arestas.push_back(&*aresta);
        }
    }

    // Inicialmente o custo da solução é máximo
    solucao.custo = solucao.custoTotal;
    // O valor de alfa da solução será o mesmo informado na execução do algorítmo
    solucao.alpha = alpha;

    // Se não existem arestas na lista de candidatos, o grafo não possúi arestas
    if(arestas.size() == 0)
    {
        // Encontramos o vértice de maior peso e adicionamos na solução
        Vertice *v = NULL;
        for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
        {
            if(v == NULL || v->getPeso() > vertice->getPeso())
                v = &*vertice;
        }
        if(v != NULL)
        {
            solucao.vertices.push_back(v);
            // Atualizamos a solução para o único vértice e retornamos
            atualizaSolucao(solucao);
            return;
        }
    }

    // Garantimos que o valor de alfa está no intervalo de 0 à 1
    if(alpha < 0)
        alpha = 0;
    else if(alpha > 1)
        alpha = 1;

    // Criamos uma solução temporária
    SolucaoGuloso solucaoTemporaria;
    // Executamos o algorítmo guloso randomizado a quantidade de épocas solicitada
    for(int i = 0; i < epocas; i++)
    {
        cout << "Epoca: " << i << endl;
        // Limpamos a solução temporária
        solucaoTemporaria.custoTotal = solucao.custoTotal;
        solucaoTemporaria.arestas.clear();
        solucaoTemporaria.vertices.clear();
        solucaoTemporaria.indices.clear();
        solucaoTemporaria.custo = 0;
        solucaoTemporaria.alpha = alpha;
        // Definimos o número da iteração como o número da época
        solucaoTemporaria.iteracao = i+1;

        // Obtemos o resultado do algorítmo guloso randomizado e colocamos na solução temporária
        algoritmoGulosoRandomizado(arestas, solucaoTemporaria, alpha);
        // Se esta for a primeira solução encontrada ou o custo da solução temporária for menor do que o custo da solução final,
        // copiamos a solução temporária para a solução final
        if(solucaoTemporaria.custo < solucao.custo || i == 0)
        {
            copiaSolucao(solucaoTemporaria, solucao);
        }
    }
}

void Guloso::calcularRandomizadoReativo(Grafo &grafo, SolucaoGuloso &solucao, float *alpha, int nAlphas, int bloco, int epocas)
{
    // Para calcular a solução gulosa randomizada reativa de um grafo para o problema de Steiner com coleta, inicialmente garantimos que a solução está vazia
    solucao.arestas.clear();
    solucao.vertices.clear();
    solucao.indices.clear();
    solucao.custo = 0;
    solucao.iteracao = 0;

    // Se o grafo não tem vértices, retornamos a solução vazia
    if(grafo.getTam() == 0)
        return;

    // Cria conjunto de arestas vazio
    list<Aresta*> arestas;

    // Calcula o custo total (soma dos pesos dos vértices) e adiciona arestas ao conjunto de candidatos
    solucao.custoTotal = 0;
    for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
    {
        solucao.custoTotal += vertice->getPeso();
        for(list<Aresta>::iterator aresta = vertice->inicio(); aresta != vertice->final(); aresta++)
        {
            arestas.push_back(&*aresta);
        }
    }

    // Inicialmente o custo da solução é máximo
    solucao.custo = solucao.custoTotal;

    // Se não existem arestas na lista de candidatos, o grafo não possúi arestas
    if(arestas.size() == 0)
    {
        // Encontramos o vértice de maior peso e adicionamos na solução
        Vertice *v = NULL;
        for(list<Vertice>::iterator vertice = grafo.inicio(); vertice != grafo.final(); vertice++)
        {
            if(v == NULL || v->getPeso() > vertice->getPeso())
                v = &*vertice;
        }
        if(v != NULL)
        {
            solucao.vertices.push_back(v);
            // Definimos o alfa para esta solução como 0
            solucao.alpha = 0;
            // Atualizamos a solução para o único vértice e retornamos
            atualizaSolucao(solucao);
            return;
        }
    }

    // Garantimos que todos os valores de alfa desejados estejam entre 0 e 1
    for(int i = 0; i < nAlphas; i++)
    {
        if(alpha[i] < 0)
            alpha[i] = 0;
        else if(alpha[i] > 1)
            alpha[i] = 1;
    }

    // Criamos uma solução temporária
    SolucaoGuloso solucaoTemporaria;
    // Definimos variáveis auxiliares
    int n = 0, roleta = 0;
    // Criamos o vetor de probabilidades
    float *p = new float[nAlphas];
    // Criamos o vetor de chances
    float *q = new float[nAlphas];
    // Criamos o vetor de médias de soluções
    float *A = new float[nAlphas];
    // Criamos o vetor de soma de soluções
    float *a = new float[nAlphas];
    float sum;
    float sum_q;
    // Inicializamos o vetor de probabilidades com uma distribuição uniforme e o vetor de somas e médias como vazio
    p[0] = 1.0f / nAlphas;
    A[0] = 0;
    a[0] = 0;
    for(int i = 1; i < nAlphas; i++)
    {
        p[i] = p[0];
        A[i] = 0;
        a[i] = 0;
    }

    // Para cada época solicitada
    for(int i = 1,  k = 1; i <= epocas; i++)
    {
        cout << "Epoca: " << i << endl;
        // Sorteamos um número aleatório entre 0 e 100
        roleta = rand() % 100;
        sum = 0;
        n = 0;
        // Procure o índice do alfa cuja probabilidade faz com que o somatório de probabilidades alcance o número sorteado
        while(n < nAlphas)
        {
            sum += p[n] * 100.0f;
            if(sum > roleta)
                break;
            n++;
        }

        // Limpamos a solução temporária
        solucaoTemporaria.custoTotal = solucao.custoTotal;
        solucaoTemporaria.arestas.clear();
        solucaoTemporaria.vertices.clear();
        solucaoTemporaria.indices.clear();
        solucaoTemporaria.custo = 0;
        // Definimos o alfa da solução temporária como o alfa encontrado no sorteio
        solucaoTemporaria.alpha = alpha[n];
        // Definimos o número da iteração como a época atual
        solucaoTemporaria.iteracao = i;

        // Executamos o algorítmo guloso randomizado
        algoritmoGulosoRandomizado(arestas, solucaoTemporaria, alpha[n]);
        // Se esta for a primeira solução encontrada ou o custo da solução temporária for menor do que o custo da solução final,
        // copiamos a solução temporária para a solução final
        if(solucaoTemporaria.custo < solucao.custo || i == 1)
        {
            copiaSolucao(solucaoTemporaria, solucao);
        }
        cout << "Solucao: " << solucaoTemporaria.custo << " ( " << solucaoTemporaria.alpha << " )" << endl;
        cout << "Melhor solucao: " << solucao.custo << " ( " << solucao.alpha << " )"  << endl;
        // Adicionamos o custo encontrado ao somatório de custos
        a[n] += solucaoTemporaria.custo;

        // Se a iteração atual define um bloco
        if(i % bloco == 0)
        {
            // Inicializamos o somatório de chances como 0
            sum_q = 0;
            for(int j = 0; j < nAlphas; j++)
            {
                // Calculamos a média de soluções para cada alfa
                A[j] = a[j] / i;
                // Se a média de soluções para cada alfa for maior que 0, a chance deste alfa ser bom é o custo da melhor solução sobre a média
                // Caso contrário, a chance é mínima
                q[j] = A[j] > 0 ? solucao.custo / A[j] : FLT_MIN;
                // Adicionamos a chance atual no somotório de chances
                sum_q += q[j];
            }
            cout << "P: ";
            // Calculamos a nova probabilidade dividindo cada chance pelo somatório de chances
            // Ou seja, as probabilidades de cada alfa são suas chances normalizadas
            for(int j = 0; j < nAlphas; j++)
            {
                p[j] = q[j] / sum_q;
                cout << p[j] << ", ";
            }
            cout << endl;
            k++;
        }
    }
    // Liberamos a memória alocada para armazenas as probabilides, médias, chances e somatórios
    delete[] p;
    delete[] q;
    delete[] A;
    delete[] a;
}

void Guloso::algoritmoGuloso(list<Aresta*> &arestas, SolucaoGuloso &solucao)
{
    Aresta *x; // Aresta de seleção
    list<Aresta*> C;
    // Geramos a lista de candidatos viáveis
    geraCandidatos(arestas, C, solucao);
    //imprimirCandidatos(C);

    // Enquanto houver candidatos viáveis
    while(C.size() > 0)
    {
        // No algorítmo guloso o melhor candidato é o primeiro da lista
        x = C.front();
        // Se a solução está vazia, adicionamos na solução o vértice de maior peso do candidato selecionado
        if(solucao.vertices.size() == 0)
        {
            if(x->getOrigem()->getPeso() > x->getExtremidade()->getPeso())
                solucao.vertices.push_back(x->getOrigem());
            else
                solucao.vertices.push_back(x->getExtremidade());
        }
        else // Se a solução não está vazia
        {
            // Se a diferença entre o peso do vértice da extremidade menos o peso da aresta candidata for negativo, testamos o vértice de origem
            if( (x->getExtremidade()->getPeso() - x->getPeso()) < 0)
            {
                // Se a diferença entre o peso do vértice da origem menos o peso da aresta candidata for negativo, está aresta não é viável para a solução
                // E como ela era a mais viável, para o algorítmo não existem outras melhores, portanto, finalizamos a solução
                if( (x->getOrigem()->getPeso() - x->getPeso()) < 0)
                    break;
                else
                {
                    // Se a diferença é positiva, adicionamos o vértice de extremidade à solução, bem como a aresta selecionada
                    solucao.vertices.push_back(x->getExtremidade());
                    if(solucao.vertices.size() > 1)
                        solucao.arestas.push_back(x);
                }
            }
            else
            {
                // Se a diferença é positiva, adicionamos o vértice de extremidade à solução, bem como a aresta selecionada
                solucao.vertices.push_back(x->getExtremidade());
                if(solucao.vertices.size() > 1)
                    solucao.arestas.push_back(x);
            }
        }
        //imprimirSolucao(solucao);

        // Geramos a nova lista de candidatos com base na solução atual
        geraCandidatos(arestas, C, solucao);
        //imprimirCandidatos(C);
    }

    // Atualizamos a solução
    atualizaSolucao(solucao);
}

void Guloso::copiaSolucao(SolucaoGuloso &src, SolucaoGuloso &dst)
{
    // Copiamos cada propriedade da solução de origem para a solução de destino
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
    // Geramos a lista de candidatos viáveis
    geraCandidatos(arestas, C, solucao);
    //imprimirCandidatos(C);
    int index = 0;
    int limite;

    // Enquanto houver candidatos viáveis
    while(C.size() > 0)
    {
        // Calculamos o índice limite para o candidato selecionado com base no valor de alfa
        limite = C.size() * alpha;
        // Selecionamos randimicamente um índice no intervalo de 0 até o limite calculado
        index = rand() % (limite + 1);

        // Selecionamos o candidato no índice sorteado
        std::list<Aresta*>::iterator it = C.begin();
        std::advance(it, index);
        x = *it;

        // Se a solução está vazia, adicionamos na solução o vértice de maior peso do candidato selecionado
        if(solucao.vertices.size() == 0)
        {
            if(x->getOrigem()->getPeso() > x->getExtremidade()->getPeso())
                solucao.vertices.push_back(x->getOrigem());
            else
                solucao.vertices.push_back(x->getExtremidade());
        }
        else // Se a solução não está vazia
        {
            // Se a diferença entre o peso do vértice da extremidade menos o peso da aresta candidata for negativo, testamos o vértice de origem
            if( (x->getExtremidade()->getPeso() - x->getPeso()) < 0)
            {
                // Se a diferença entre o peso do vértice da origem menos o peso da aresta candidata for negativo, está aresta não é viável para a solução
                // E como ela era a mais viável, para o algorítmo não existem outras melhores, portanto, finalizamos a solução
                if( (x->getOrigem()->getPeso() - x->getPeso()) < 0)
                    break;
                else
                {
                    // Se a diferença é positiva, adicionamos o vértice de extremidade à solução, bem como a aresta selecionada
                    solucao.vertices.push_back(x->getExtremidade());
                    if(solucao.vertices.size() > 1)
                        solucao.arestas.push_back(x);
                }
            }
            else
            {
                // Se a diferença é positiva, adicionamos o vértice de extremidade à solução, bem como a aresta selecionada
                solucao.vertices.push_back(x->getExtremidade());
                if(solucao.vertices.size() > 1)
                    solucao.arestas.push_back(x);
            }
        }
        //imprimirSolucao(solucao);

        // Geramos a nova lista de candidatos com base na solução atual
        geraCandidatos(arestas, C, solucao);
        //imprimirCandidatos(C);
    }

    // Atualizamos a solução
    atualizaSolucao(solucao);
    //imprimir(solucao);
}

void Guloso::geraCandidatos(list<Aresta*> &arestas, list<Aresta*> &C, SolucaoGuloso &solucao)
{
    // Para gerar a lista de candidatos, inicialmente definimos esta lista como vazia
    C.clear();
    // Se a solução é vazia, então todos os candidatos são viáveis
    if(solucao.vertices.size() == 0)
    {
        C = arestas;
        // Ordenamos os candidatos com base no peso das arestas e dos vértices
        C.sort(ordenaPrimeirosCandidatos);
    }
    else // Se a solução não é vazia
    {
        bool valido = true;
        // Percorremos cada aresta A disponível
        for(list<Aresta*>::iterator aresta = arestas.begin(); aresta != arestas.end(); aresta++)
        {
            // Percorremos cada vértice B da solução
            for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
            {
                // Se a origem da aresta A for igual ao vértice B, esta aresta candidata é inicialmente válida, é preciso conferir
                if((*aresta)->getOrigem() == *vertice)
                {
                    valido = true;
                    // Para cada vértice D na solução
                    for(list<Vertice*>::iterator extremidade = solucao.vertices.begin(); extremidade != solucao.vertices.end(); extremidade++)
                    {
                        // Se a extremidade da aresta A for igual ao vértice D, então esta aresta fecha um ciclo na solução e não é válida
                        if((*aresta)->getExtremidade() == *extremidade)
                        {
                            valido = false;
                            break;
                        }
                    }
                    if(valido) // Se a resta passou pela validação, adicionamos-a na lista de candidatos viáveis
                        C.push_back(*aresta);
                }
            }
        }
        // Ordenamos os candidatos com base no peso das arestas e dos vértices
        C.sort(ordenaCandidatos);
    }

}

void Guloso::imprimirCandidatos(list<Aresta*> &C)
{
    // Imprimimos os candidatos na tela (Apenas para debugar)
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
    // Na atualização da solução, calculamos seu custo atual
    float custo = solucao.custoTotal;
    // Do custo total, subtraimos o peso de cada vértice adicionado à solução
    for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
    {
        solucao.indices.push_back((*vertice)->getInfo());
        custo -= (*vertice)->getPeso();
    }
    // Do custo calculado acima, somamos o custo de cada aresta adicionada à solução
    for(list<Aresta*>::iterator aresta = solucao.arestas.begin(); aresta != solucao.arestas.end(); aresta++)
    {
        custo += (*aresta)->getPeso();
    }
    // Obtemos o novo custo da solução
    solucao.custo = custo;
}

void Guloso::imprimirSolucao(SolucaoGuloso &solucao)
{
    // Imprimimos a solução (Apenas para debugar)
    cout << "S = " << solucao.vertices.size() << endl;
    for(list<Vertice*>::iterator vertice = solucao.vertices.begin(); vertice != solucao.vertices.end(); vertice++)
    {
        cout << (*vertice)->getInfo() << ", ";
    }
    cout << endl;
}

void Guloso::imprimir(SolucaoGuloso &solucao, Grafo &grafo, float best)
{
    // Imprimimos a solução
    cout << "Custo: " << solucao.custo << endl; // Exibimos o custo
    cout << "Custo Máximo: " << solucao.custoTotal << endl; // Exibimos o custo máximo (soma dos vértices)
    cout << "Relação Custo Máximo " << ((solucao.custo/solucao.custoTotal)*100.0f) << "%" << endl; // Calculamos a relação do custo com o custo máximo
    if(best > 0)
    {
        // Se o custo ótimo foi informado, calculamos sua relação com o custo encontrado
        float percent = ((solucao.custo - best)/best)*100.0f;
        cout << "Relação Melhor Custo: " << percent << "%" << endl;
    }
    if(solucao.alpha > 0) // Se foi utilizado um valor de alfa, exibimos-o
        cout << "Alpha: " << solucao.alpha << endl;
    cout << "Iteração: " << solucao.iteracao << endl; // Exibimos a iteração em que a solução foi encontrada
    cout << "Vertices (" << grafo.getTam() << "): " << solucao.indices.size() << endl; // Exibimos a quantidade de vértices na solução
    /*
    for (list<int>::iterator it = solucao.indices.begin() ; it != solucao.indices.end(); ++it)
    {
        cout << (*it) << ", ";
    }
    */
    //cout << endl;
    cout << "Arestas (" << grafo.getArestas() << "): " << solucao.arestas.size() << endl; // Exibimos a quantidade de arestas na solução
    /*for (list<Aresta*>::iterator it = solucao.arestas.begin() ; it != solucao.arestas.end(); ++it)
    {
        cout << (*it)->getOrigem()->getInfo() << " -> " << (*it)->getExtremidade()->getInfo() << " | " << (*it)->getPeso() << endl;
    }*/
}

bool Guloso::ordenaCandidatos(Aresta *primeiro, Aresta *segundo)
{
    // Para ordenar os candidatos quando existe uma solução, consideramos a diferença de peso do vértice de
    // maior peso da aresta e o peso da aresta
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
    // O candidato que tiver um peso mais positivo deve ficar na frente da lista
    return pesoPrimeiro > pesoSegundo;
}

bool Guloso::ordenaPrimeirosCandidatos(Aresta *primeiro, Aresta *segundo)
{
    // Quando a solução atual é vazia, calculamos o peso de cada candidato como a diferença entre a soma dos pesos dos dois
    // vértices e o peso da aresta
    float pesoPrimeiro = (primeiro->getExtremidade()->getPeso() + primeiro->getOrigem()->getPeso()) - primeiro->getPeso();
    float pesoSegundo = (segundo->getExtremidade()->getPeso() + segundo->getOrigem()->getPeso()) - segundo->getPeso();
    // O candidato que tiver um peso mais positivo deve ficar na frente da lista
    return pesoPrimeiro > pesoSegundo;
}
