#include "Instance.h"

#include <vector>
#include <utility>

#pragma once

class Reducer
{
private:
    // finds the element that is present in all sets (more than 1)
    static int has_common_element(Instance& instance);
    
    // delete the element from the entire instance
    static void delete_element(Instance& instance, int element);

    // finds the set's index with an unique element
    static int has_unique_element(Instance& instance);
    
    // include a set to the overall "solution"; delete all included elements
    static void include_set(Instance& instance, int i);

    // find a set's index that's a subset of at least one other set
    static int has_common_set(Instance& instance);

    // delete a set from the instance
    static void delete_set(Instance& instance, int i);

    // delete all empty sets
    static void delete_empty(Instance& instance);

public:
    static std::pair<Instance, int> reduce(Instance& instance);
    static std::pair<Instance, int> copy_reduce(Instance instance);
};
