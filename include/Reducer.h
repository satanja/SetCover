#include "Instance.h"

#include <vector>
#include <utility>
#include <array>
#pragma once

class Reducer
{
public:
	static std::pair<Instance, int> reduce(Instance& instance);
	static std::pair<Instance, int> copy_reduce(Instance instance);
};
