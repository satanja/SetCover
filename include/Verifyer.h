#include "Instance.h"
#include <set>
#include <algorithm>

#pragma once
class Verifyer
{
public:
    static bool is_valid(Instance& instance, std::vector<int> solution)
    {
        std::set<int> X;
        for (auto set : solution)
        {
            std::vector<int> Y = instance.get_set(set);
            for (auto element : Y)
            {
                X.insert(element);
            }
        }
        std::vector<int> a(X.begin(), X.end());
        std::set<int> U = instance.get_universe();
        std::vector<int> b(U.begin(), U.end());

        std::vector<int> diff(std::max(a.size(), b.size()));
        auto it = std::set_difference(b.begin(), b.end(), a.begin(), a.end(), diff.begin());
        diff.resize(it - diff.begin());
    
        return diff.size() == 0 && a.size() == b.size();
    }
};
