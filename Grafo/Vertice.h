#ifndef VERTICE_H_INCLUDED
#define VERTICE_H_INCLUDED

class Vertice
{
public:
    Vertice();
    ~Vertice();
    void setInfo(int info);
    int getInfo();
    void setPeso(float Peso);
    float getPeso();

private:
    int info; // informacao do vertice
    float peso;
    //vetor de aresta
};

#endif // VERTICE_H_INCLUDED
