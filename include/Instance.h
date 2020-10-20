#include <set>
#include <vector>

#include "MinHeap.h"
#include "MaxHeap.h"


#include <functional>

class Instance
{
public:
    std::set<int> universe;
    std::vector<std::set<int>> families;

    // A min heap to find unique elements efficiently
    MinHeap min_heap;

    // A max heap to find trivial elements efficiently
    // NB: trivial = present in every set, and at least 2 sets
    MaxHeap max_heap;

    // The addresses for the universe elements in the min heap
    std::vector<int> minAddresses;

    // The addresses for the universe elements in the max heap
    std::vector<int> maxAddresses;

    // index of the last initialized family in families
    int last = -1;

    void set_universe_size(int size) {
        std::vector<std::pair<int, int>> min_vec(size);
        minAddresses.resize(size);
        std::vector<std::pair<int, int>> max_vec(size);
        maxAddresses.resize(size);

        for (int i = 0; i < size; i++) {
            universe.insert(i);
            min_vec[i] = std::make_pair(i, 0);
            minAddresses[i] = i;
            max_vec[i] = std::make_pair(i, 0);
            maxAddresses[i] = i;
        }

        using namespace std::placeholders;
        min_heap = MinHeap(min_vec, std::bind(&Instance::update_index_min, this, _1, _2));
        max_heap = MaxHeap(max_vec, std::bind(&Instance::update_index_max, this, _1, _2));
    }

    
public:

    void update_index_min(int element, int index) {
        minAddresses[element] = index;
    }

    void update_index_max(int element, int index) {
        maxAddresses[element] = index;
    }

    Instance() {

    }

    void resize(int universe_size, int families_size) {
        set_universe_size(universe_size);
        families.resize(families_size);
    }

    void add_family(std::set<int> family) {
        last++;
        families[last] = family;
        for (int element : family) {

            int min_addr = minAddresses[element];
            const std::pair<int, int> state = min_heap[min_addr];
            min_heap.increase_key(std::make_pair(element, -1), std::make_pair(element, state.second + 1), min_addr);

            int max_addr = maxAddresses[element];
            max_heap.increase_key(std::make_pair(element, state.second + 1), max_addr);
        }
    }

    bool has_unique_element() {
        return min_heap.peek_min().second == 1;
    }

    bool has_common_element() {
        return max_heap.peek_max().second == families.size();
    }
};

