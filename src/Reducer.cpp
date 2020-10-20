#include "Reducer.h"

#include <algorithm>
#include <iterator>
#include <iostream>

std::pair<Instance, int> Reducer::reduce(Instance& instance)
{
    int k = 0;
    bool reduced = false;
    int last_common_set = 0;
    do 
    {
        reduced = false;

        bool unique = instance.has_unique_element();
        if (unique)
        {
            // include unique
        }
        
        last_common_set = has_common_set(instance, last_common_set);
        if (last_common_set >= 0)
        {
            std::cout << "common set: " << last_common_set << std::endl;
            delete_set(instance, last_common_set);
            reduced = true;
            continue;
        }

        bool common = instance.has_common_element();
        if (common)
        {
            // include common
        }

        auto pair = has_strong_pair(instance);
        if (pair[0] >= 0)
        {
            last_common_set = 0;
            std::cout << "strong pair: (" << pair[0] << ", " << pair[1] << ")" << std::endl;
            include_pair(instance, pair);

            k += 2;
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
    instance.universe.erase(element);
    delete_empty(instance);
}

std::vector<int> Reducer::has_unique_element(Instance& instance)
{
    std::vector<int> uniques;
    for (auto const& x : instance.universe)
    {
        int count = 0;
        int index = 0;
        for (int i = 0; i < instance.families.size(); i++)
        {
            auto family = instance.families[i];
            if (family.contains(x))
            {
                count++;
                index = i;
            }
            if (count > 1)
            {
                break;
            }
        }
        
        // possible room for optimization
        if (count == 1)
        {
            uniques.push_back(index);
        }
    }
    std::sort(uniques.begin(), uniques.end());
    return uniques;
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

int Reducer::has_common_set(Instance& instance, int last)
{
    for (int i = last; i < instance.families.size(); i++)
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

std::array<int, 2> Reducer::has_strong_pair(Instance& instance)
{
    for (int i = 0; i < instance.families.size(); i++)
    {
        for (int j = i + 1; j < instance.families.size(); j++)
        {
            std::set<int> joined;
            auto a = instance.families[i];
            auto b = instance.families[j];
            std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(joined, joined.begin()));

            bool isStrong = true;
            for (int x = 0; x < instance.families.size() && isStrong; x++)
            {
                for (int y = x + 1; y < instance.families.size() && isStrong; y++)
                {
                    // check all other pairs
                    if (x == i && y == j) continue;

                    std::set<int> candidate;
                    auto a = instance.families[x];
                    auto b = instance.families[y];
                    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(candidate, candidate.begin()));

                    std::set<int> temp;
                    std::set_union(joined.begin(), joined.end(), candidate.begin(), candidate.end(), std::inserter(temp, temp.begin()));
                    
                    if (temp.size() > joined.size())
                    {
                        isStrong = false;
                        break;
                    }
                }
            }
            
            if (isStrong)
            {
                // joined was a superset of all other pairs
                return std::array<int, 2> {i, j};
            }
        }
    }
    
    // no strong pair found
    return std::array<int, 2> {-1, -1};
}

void Reducer::include_pair(Instance& instance, std::array<int, 2> indices)
{
    auto a = instance.families[indices[0]];
    auto b = instance.families[indices[1]];
    std::set<int> joined;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(joined, joined.begin()));

    for (int j = 0; j < instance.families.size(); j++)
    {
        if (indices[0] == j || indices[1] == j) continue;
        for (auto const& x : joined)
        {
            instance.families[j].erase(x);
        }
    }

    // match universe
    for (auto const& x : joined)
    {
        instance.universe.erase(x);
    }

    // invariant: indices[0] < indices[1]
    instance.families.erase(instance.families.begin() + indices[0]);
    instance.families.erase(instance.families.begin() + indices[1] - 1);

    delete_empty(instance);
}

int Reducer::solve_greedy(Instance instance) {
    int size = 0;
    while (instance.universe.size() != 0 && instance.families.size() != 0) {
        std::set<int> set = instance.families[0];
        int index = 0;
        int max = set.size();
        for (int i = 0; i < instance.families.size(); i++) {
            auto X = instance.families[i];
            if (X.size() > max) {
                set = X;
                max = X.size();
                index = i;
            }
        }
        size++;
        include_set(instance, index);
    }
    return size;
}