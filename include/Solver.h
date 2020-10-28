#include "Reducer.h"

#pragma once
class Solver
{
public:
	static std::vector<int> greedy_reduce_solve(Instance instance)
	{
		std::vector<int> solution;
		while (!instance.is_empty())
		{
			std::vector<int> X = Reducer::reduce(instance).second;
			solution.insert(solution.end(), X.begin(), X.end());
			if (!instance.is_empty())
			{
				int largest = instance.get_largest_set();
				instance.include_set(largest);
				solution.push_back(largest);
			}
		}
		return solution;
	}
};