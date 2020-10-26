#include "Reducer.h"
#include "Solver.h"

#include <list>
#include <iostream>
#include <sstream>
#include <string>

#include "MinHeap.h"
#include "MaxHeap.h"

#include "PairCompartor.h"

#include <chrono>

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
		std::vector<int> family;
		while (!ss.eof())
		{
			int x;
			ss >> x;
			family.push_back(x);
		}
		instance.add_family(family);
	}
	instance.initialize_heaps();
}

int main()
{
	Instance instance;

	auto start = std::chrono::high_resolution_clock::now();
	read(instance);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(stop - start);
	std::cout << "initializing = " << duration.count() << "s" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	auto kernel = Reducer::reduce(instance);
	stop = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration<double>(stop - start);

	instance.clean_up();

	std::cout << "reduce time = " << duration.count() << "s" << std::endl;
	std::cout << "|U| = " << instance.universe_size() << std::endl;
	std::cout << "|F| = " << instance.families_size() << std::endl;
	std::cout << "required = " << kernel.second << std::endl;
	std::cout << std::endl;

	return 0;
}