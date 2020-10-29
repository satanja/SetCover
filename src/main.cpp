#include "Reducer.h"
#include "Solver.h"
#include "Verifier.h"

#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "MinHeap.h"
#include "MaxHeap.h"


#include <chrono>

void read(Instance& instance, const std::string& file)
{
	std::ifstream input { file };
	int u;
	input >> u;
	int fs;
	input >> fs;
	instance.resize(u, fs);

	std::string str;
	getline(input, str);

	for (int i = 0; i < fs; i++)
	{
		getline(input, str);
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

int main(int argc, char* argv[])
{
	std::string file{ argv[2] };

	Instance instance;
	auto start = std::chrono::high_resolution_clock::now();
	read(instance, file);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(stop - start);

	if (*argv[1] == '0')
	{
		std::cout << "initialize time = " << duration.count() << "s" << "\n";

		std::cout << "\n";
		std::cout << "original:" << "\n";
		std::cout << "|U| = " << instance.universe_size() << "\n";
		std::cout << "|F| = " << instance.families_size() << "\n";

		start = std::chrono::high_resolution_clock::now();
		auto kernel = Reducer::reduce(instance);
		stop = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration<double>(stop - start);

		instance.clean_up();

		std::cout << "\n";
		std::cout << "reduced:" << "\n";
		std::cout << "reduce time = " << duration.count() << "s" << "\n";
		std::cout << "|U| = " << instance.universe_size() << "\n";
		std::cout << "|F| = " << instance.families_size() << "\n";
		std::cout << "solution size lower bound = " << kernel.second.size() << "\n";
	}
	else if (*argv[1] == '1')
	{
		Instance copy(instance);
		std::vector<int> solution = Solver::greedy_reduce_solve(instance);
		std::cout << "solution size = " << solution.size() << "\n";
		std::cout << "is valid = " << Verifier::is_valid(copy, solution) << "\n";

		std::ofstream output;
		output.open(file + "-solution.txt");
		output << solution.size() << "\n";
		for (auto set : solution) 
		{
			output << set << "\n";
		}
		output.close();
	}
	return 0;
}