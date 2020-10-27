#include "Reducer.h"

#include <algorithm>
#include <iterator>
#include <iostream>

std::pair<Instance, std::vector<int>> Reducer::reduce(Instance& instance)
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
			continue;
		}

		if (instance.remove_equals())
		{
			reduced = true;
			continue;
		}

		if (instance.remove_subsets())
		{
			reduced = true;
			continue;
		}

		//bool common = instance.has_common_element();
		//if (common)
		//{
		//	instance.include_common_elements();
		//	reduced = true;
		//	last = 0;
		//	continue;
		//}

		//auto pair = has_strong_pair(instance);
		//if (pair[0] >= 0)
		//{
		//    last_common_set = 0;
		//    std::cout << "strong pair: (" << pair[0] << ", " << pair[1] << ")" << std::endl;
		//    include_pair(instance, pair);

		//    k += 2;
		//    reduced = true;
		//    continue;
		//}

	} while (reduced);
	//instance.clean_up();
	return std::make_pair(instance, k);
}

std::pair<Instance, std::vector<int>> Reducer::copy_reduce(Instance instance)
{
	return reduce(instance);
}