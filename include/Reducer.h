#include "Instance.h"

#include <vector>
#include <utility>
#include <array>
#pragma once

class Reducer
{
public:
	static std::pair<Instance, std::vector<int>> reduce(Instance& instance);
	static std::pair<Instance, std::vector<int>> copy_reduce(Instance instance);
};
