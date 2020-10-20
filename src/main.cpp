#include "Reducer.h"
#include <list>
#include <iostream>
#include <sstream>
#include <string>

#include "MinHeap.h"
#include "MaxHeap.h"

#include "PairCompartor.h"

void read(Instance& instance)
{
    int u;
    std::cin >> u;
    int fs;
    std::cin >> fs;
    instance.resize(u, fs);

    std::string str;
    getline(std::cin, str);

    for (int i = 0; i < fs; i++)
    {
        getline(std::cin, str);
        std::istringstream ss(str);
        std::set<int> family;
        while (!ss.eof())
        {
            int x;
            ss >> x;
            family.insert(x);
        }
        instance.add_family(family);
    }
}

int main()
{
    Instance instance;
    read(instance);
    //auto kernel = Reducer::copy_reduce(instance);
    //std::cout << "|U| = " << kernel.first.universe.size() << std::endl;
    //std::cout << "|F| = " << kernel.first.families.size() << std::endl;
    
    //std::cout << "greedy = " << Reducer::solve_greedy(instance) << std::endl;
    //std::cout << "reduced greedy = " << Reducer::solve_greedy(kernel.first) + kernel.second << std::endl;
    return 0;
}