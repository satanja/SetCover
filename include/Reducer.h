#include "Instance.h"

#include <algorithm>
#include <iterator>
#include <iostream>

#include <vector>
#include <utility>

#pragma once
class Reducer
{
public:
	static std::pair<Instance, std::vector<int>> reduce(Instance& instance)
	{
		std::vector<int> k;
		bool reduced = false;
		do
		{
			reduced = false;

			bool unique = instance.has_unique_element();
			if (unique)
			{
				std::vector<int> d = instance.include_unique_elements();
				k.insert(k.end(), d.begin(), d.end());
				reduced = true;
			}

			if (instance.remove_equals())
			{
				reduced = true;
			}

			if (instance.remove_subsets())
			{
				reduced = true;
			}

		} while (reduced);
		return std::make_pair(instance, k);
	}

	static std::pair<Instance, std::vector<int>> copy_reduce(Instance instance)
	{
		return reduce(instance);
	}
};
