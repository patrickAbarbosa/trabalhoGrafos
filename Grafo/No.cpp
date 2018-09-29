#include <iostream>
#include <cstddef>
#include "No.h"

using namespace std;

No::No()
{
    prox = NULL;
    info = -1;
}
No::~No()
{

}

void No::setInfo(int info)
{
    this->info = info;
}

int No::getInfo()
{
    return info;
}

No* No::getProx()
{
    return prox;
}

