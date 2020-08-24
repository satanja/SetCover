#include "Reducer.h"

#include <iostream>

int main()
{
    Instance I;
    I.universe = {1, 2, 3};
    I.families.resize(3);
    I.families[0] = {1, 2};
    I.families[1] = {2, 3};
    I.families[2] = {3, 1};
    Reducer::reduce(I);
    return 0;
}