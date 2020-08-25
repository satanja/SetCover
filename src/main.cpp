#include "Reducer.h"

#include <iostream>

void read(Instance& instance)
{
    int u;
    std::cin >> u;

    int fs;
    std::cin >> fs;
    instance.families.resize(fs);

    for (int i = 0; i < u; i++)
    {
        instance.universe.insert(i);
    }

    for (int i = 0; i < fs; i++)
    {
        int m;
        std::cin >> m;
        for (int j = 0; j < m; j++)
        {
            int x;
            std::cin >> x;
            instance.families[i].insert(x);
        }
    }
}

int main()
{
    Instance instance;
    read(instance);
    auto kernel = Reducer::reduce(instance);
    std::cout << kernel.second << std::endl;
    return 0;
}