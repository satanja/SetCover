#include "Reducer.h"

#include <algorithm>
#include <iterator>

std::pair<Instance, int> Reducer::reduce(Instance& instance)
{
    int k = 0;
    bool reduced = false;
    do 
    {
        reduced = false;

        int i = has_unique_element(instance);
        if (i >= 0)
        {
            k++;
            include_set(instance, i);
            reduced = true;
            continue;
        }
        
        i = has_common_set(instance);
        if (i >= 0)
        {
            delete_set(instance, i);
            reduced = true;
            continue;
        }

        int x = has_common_element(instance);
        if (x >= 0)
        {
            delete_element(instance, x);
            reduced = true;
            continue;
        }

    } while (reduced);
    return std::make_pair(instance, k);
}

std::pair<Instance, int> Reducer::copy_reduce(Instance instance)
{
    return reduce(instance);
}

int Reducer::has_common_element(Instance& instance)
{
    if (instance.families.size() == 0) return -1;
    std::set<int> intersect = instance.families[0];
    
    for (auto const& family : instance.families)
    {
        std::set<int> temp(std::move(intersect));
        std::set_intersection(family.begin(), family.end(), temp.begin(), temp.end(), std::inserter(intersect, intersect.begin()));
    }

    if (intersect.size() > 0) return *intersect.begin();
    return -1;
}

void Reducer::delete_element(Instance& instance, int element)
{
    for (auto& family : instance.families)
    {
        family.erase(element);
    }
    delete_empty(instance);
}

int Reducer::has_unique_element(Instance& instance)
{
    for (auto const& x : instance.universe)
    {
        int count = 0;
        int i = 0;
        for (auto const& family : instance.families)
        {
            count += family.contains(x);
        }
        
        // possible room for optimization
        if (count == 1)
        {
            for (int i = 0; i < instance.families.size(); i++)
            {
                if (instance.families[i].contains(x))
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

void Reducer::include_set(Instance& instance, int i)
{
    // delete occurence from each subfamily
    for (int j = 0; j < instance.families.size(); j++)
    {
        if (i == j) continue;
        for (auto const& x : instance.families[i])
        {
            instance.families[j].erase(x);
        }
    }

    // match universe
    for (auto const& x : instance.families[i])
    {
        instance.universe.erase(x);
    }

    // delete the included set
    delete_set(instance, i);
    delete_empty(instance);
}

int Reducer::has_common_set(Instance& instance)
{
    for (int i = 0; i < instance.families.size(); i++)
    {
        auto s1 = instance.families[i];
        for (int j = 0; j < instance.families.size(); j++)
        {
            if (j == i) continue;
            auto s2 = instance.families[j];

            bool contained = true;
            for (auto const &a : s1)
            {
                if (!s2.contains(a))
                {
                    contained = false;
                    break;
                }
            }
            if (contained) return i;
        }
    }
    return -1;
}

void Reducer::delete_set(Instance& instance, int i)
{
    instance.families.erase(instance.families.begin() + i);
    delete_empty(instance);
}

void Reducer::delete_empty(Instance& instance)
{
    bool reduced;
    do
    {
        reduced = false;
        for (auto it = instance.families.begin(); it != instance.families.end(); it++)
        {
            if (it->size() == 0)
            {
                instance.families.erase(it);
                reduced = true;
                break;
            }
        }
    } while (reduced);
}
