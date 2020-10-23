#include <set>
#include <vector>

#include "MinHeap.h"
#include "MaxHeap.h"


#include <functional>

#include <iostream>
#include <algorithm>
#include <iterator>

class Instance
{
public:
    std::set<int> universe;
    std::vector<std::vector<int>> families;

    // A min heap to find unique elements efficiently
    MinHeap min_heap;

    // A max heap to find trivial elements efficiently
    // NB: trivial = present in every set, and at least 2 sets
    MaxHeap max_heap;

    // The addresses for the universe elements in the min heap
    std::vector<int>* minAddresses;

    // The addresses for the universe elements in the max heap
    std::vector<int>* maxAddresses;

    // index of the last initialized family in families
    int last = -1;

    // adjacency list from elements to their sets
    std::vector<std::vector<int>> adj;

    void set_universe_size(int size) {
        std::vector<std::pair<int, int>> min_vec(size);
        minAddresses = new std::vector<int>;
        maxAddresses = new std::vector<int>;
        minAddresses->resize(size);
        std::vector<std::pair<int, int>> max_vec(size);
        maxAddresses->resize(size);

        adj.resize(size);

        for (int i = 0; i < size; i++) {
            universe.insert(i);
            min_vec[i] = std::make_pair(i, 0);
            minAddresses->at(i) = i;
            max_vec[i] = std::make_pair(i, 0);
            maxAddresses->at(i) = i;
        }

        using namespace std::placeholders;
        min_heap = MinHeap(min_vec, std::bind(&Instance::update_index_min, this, _1, _2));
        max_heap = MaxHeap(max_vec, std::bind(&Instance::update_index_max, this, _1, _2));
    }

    void delete_set(int set) {
        families.erase(families.begin() + set);
    }

    void delete_adj(int set) {

    }

    void delete_heap(int set) {
        for (auto const& x : families[set]) {
            int min_addr = minAddresses->at(x);
            int max_addr = maxAddresses->at(x);

            min_heap.decrease_key(std::make_pair(x, -1), min_addr);
            min_heap.extract_min();

            max_heap.increase_key(std::make_pair(x, universe.size() + 1), max_addr);
            max_heap.extract_max();

            minAddresses->at(x) = -1;
            maxAddresses->at(x) = -1;
        }
    }

    void delete_empty_adj() {

    }

    void delete_empty_families() {

        std::vector<std::vector<int>> new_families;
        for (auto& set : families) {
            if (set.size() > 0)
            {
                new_families.push_back(set);
            }
        }
        families = new_families;
    }

    void include_set(int set) {
        for (int i = 0; i < families.size(); i++) {
            if (i == set) continue;
            for (auto const& x : families[set]) {
                auto ptr = std::find(families[i].begin(), families[i].end(), x);
                families[i].erase(ptr);
            }
        }


        // clean up all references to the set
        delete_adj(set);
        delete_empty_adj();
        delete_heap(set);

        // possible to clean up the set itself
        for (auto const& x : families[set]) {
            universe.erase(x);
        }

        delete_set(set);
        delete_empty_families();
    }

    void include_sets(std::vector<int>& sets) {
        std::set<int> combined;
        for (auto const& set : sets) {
            for (auto const& element : families[set]) {
                combined.insert(element);
            }
        }

        std::vector<int> total(combined.begin(), combined.end());

        int j = 0;
        for (int i = 0; i < families.size(); i++) {
            std::vector<int> new_set(families[i].size());
            std::vector<int> family = families[i];
            auto it = std::set_difference(family.begin(), family.end(), total.begin(), total.end(), new_set.begin());
            new_set.resize(it - new_set.begin());
            families[i] = new_set;
        }

        auto max = universe.size() + 1;
        for (auto const& element : combined) {
            universe.erase(element);
            int index = maxAddresses->at(element);
            max_heap.increase_key(std::make_pair(element, max), index);
            max_heap.extract_max();
        }
    }

public:

    void update_index_min(int element, int index) {
        minAddresses->at(element) = index;
    }

    void update_index_max(int element, int index) {
        maxAddresses->at(element) = index;
    }

    Instance() {

    }

    void resize(int universe_size, int families_size) {
        set_universe_size(universe_size);
        families.resize(families_size);
    }

    void add_family(std::vector<int> family) {
        last++;
        families[last] = family;
        for (int element : family) {

            int min_addr = minAddresses->at(element);
            const std::pair<int, int> state = min_heap[min_addr];
            min_heap.increase_key(std::make_pair(element, -1), std::make_pair(element, state.second + 1), min_addr);

            int max_addr = maxAddresses->at(element);
            max_heap.increase_key(std::make_pair(element, state.second + 1), max_addr);
            
            adj[element].push_back(last);
        }
    }

    bool has_unique_element() {
        return min_heap.peek_min().second == 1;
    }

    bool has_common_element() {
        return max_heap.peek_max().second == families.size();
    }

    int include_unique_element() {
        std::vector<int> uniques;
        auto pair = min_heap.peek_min();
        while (pair.second == 1) {
            uniques.push_back(pair.first);
            min_heap.extract_min();
            pair = min_heap.peek_min();
        }
        std::set<int> unique_sets;
        for (auto const& x : uniques) {
            unique_sets.insert(adj[x][0]);
        }

        int count = unique_sets.size();
        std::vector<int> result(unique_sets.begin(), unique_sets.end());
        include_sets(result);
        return count;
    }

    void clean_up() {
        delete_empty_families();
        delete minAddresses;
        delete maxAddresses;
    }
};

