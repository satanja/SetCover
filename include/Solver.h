#include "Reducer.h"
#include <iostream>
#pragma once
class Solver
{
public:
	//static int greedy_solve(Instance instance)
	//{
	//	int size = 0;
	//	while (instance.universe.size() != 0 && instance.families.size() != 0)
	//	{
	//		std::vector<int> set = instance.families[0];
	//		int index = 0;
	//		int max = set.size();
	//		for (int i = 0; i < instance.families.size(); i++)
	//		{
	//			auto X = instance.families[i];
	//			if (X.size() > max)
	//			{
	//				set = X;
	//				max = X.size();
	//				index = i;
	//			}
	//		}
	//		size++;
	//		instance.include_set(set);
	//	}
	//	return size;
	//}

	//static int greedy_reduce_solve(Instance instance)
	//{
	//	int size = 0;
	//	while (!instance.is_empty())
	//	{
	//		size += Reducer::reduce(instance).second;
	//		if (!instance.is_empty())
	//		{
	//			std::vector<int> set = instance.families[0];
	//			int index = 0;
	//			int max = set.size();
	//			for (int i = 0; i < instance.families.size(); i++)
	//			{
	//				auto X = instance.families[i];
	//				if (X.size() > max)
	//				{
	//					set = X;
	//					max = X.size();
	//					index = i;
	//				}
	//			}
	//			size++;
	//			instance.include_set(set);
	//			std::cout << instance.universe.size() << std::endl;
	//			
	//		}
	//	}
	//	return size;
	//}
};