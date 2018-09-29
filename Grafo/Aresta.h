#ifndef ARESTA_H_INCLUDED
#define ARESTA_H_INCLUDED

class Aresta
{
public:
    Aresta();
    ~Aresta();
    void setInfo(Vertice* p){info = p;}
    Vertice* getInfo(){return info;}
    void setPeso(float peso){this->peso = peso;}
    float getPeso(){return peso;}

private:
    Vertice* info;
    float peso;
};


#endif // ARESTA_H_INCLUDED
