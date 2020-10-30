#include <algorithm>
#include <functional>
#include <iterator>
#include <set>
#include <vector>
#include <unordered_set>
#include <map>

#include "MaxHeap.h"
#include "MinHeap.h"

#pragma once
class Instance 
{
private:
	std::set<int> universe;
	std::vector<std::vector<int>> families;

	// A min heap to find unique elements efficiently
	MinHeap min_heap;

	// A max heap to find trivial elements efficiently
	MaxHeap max_heap;

	// The addresses for the universe elements in the min heap
	std::vector<int>* minAddresses;

	// The addresses for the universe elements in the max heap
	std::vector<int>* maxAddresses;

	// Index of the last initialized family in families
	int last = -1;

	// Number of current empty sets
	int deleted = 0;

	// Adjacency list from elements to their sets
	std::vector<std::vector<int>> adj;

	// Synopsis that contains the number of sets an element is contained in
	std::vector<int> count;

	// ## Private Initialization ##
	void set_universe_size(int size)
	{

		minAddresses = new std::vector<int>;
		maxAddresses = new std::vector<int>;
		minAddresses->resize(size);
		maxAddresses->resize(size);

		adj.resize(size);

		for (int i = 0; i < size; i++)
		{
			universe.insert(i);
			minAddresses->at(i) = i;
			maxAddresses->at(i) = i;
		}

		count.resize(size);
	}

	// ## Callbacks ##
	// Methods used to keep track of the index of each element in the heaps 
	// for decrease_key / increase_key operations
	void update_index_min(int element, int index)
	{
		minAddresses->at(element) = index;
	}

	void update_index_max(int element, int index)
	{
		maxAddresses->at(element) = index;
	}

	// ## Evaluation ## 

	// Returns the number of sets that contain all elements in query
	std::vector<int> get_sets_by_list(std::vector<int> query)
	{
		auto compare_selectivity = [this](int element1, int element2) -> bool
		{
			if (count[element1] < count[element2])
			{
				return true;
			}
			else if (count[element1] == count[element2])
			{
				return element1 < element2;
			}
			return false;
		};

		sort(query.begin(), query.end(), compare_selectivity);
		std::vector<int> result = select(query[0]);

		int i = 1;
		while (result.size() > 1 && i < query.size())
		{
			result = join(result, select(query[i]));
			i++;
		}
		return result;
	}

	// Returns all the sets element is contained in
	std::vector<int> select(int element)
	{
		return adj[element];
	}

	// Returns the intersection of left and right
	std::vector<int> join(std::vector<int>& left, std::vector<int>& right)
	{
		size_t size = std::max(left.size(), right.size());
		std::vector<int> joined(size);

		auto it = std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), joined.begin());
		joined.resize(it - joined.begin());

		return joined;
	}

	// ## Data Structure Maintenance ## 

	// removes set from adj[element] using a binary search
	inline void remove_from_adj(int element, int set)
	{
		auto iter = std::lower_bound(adj[element].begin(), adj[element].end(), set);
		adj[element].erase(iter);
	}

	// removes an element from a set using a binary search
	inline void remove_from_set(int element, int set)
	{
		auto iter = std::lower_bound(families[set].begin(), families[set].end(), element);
		families[set].erase(iter);
	}

	// removes element from min_heap and max_heap 
	void remove_from_heaps(int element)
	{
		min_heap.decrease_key(std::make_pair(element, -1), minAddresses->at(element));
		min_heap.extract_min();
		max_heap.increase_key(std::make_pair(element, INT_MAX), minAddresses->at(element));
		max_heap.extract_max();
	}

	// decreases the occurence of element in the heaps to its actual count
	void decrease_heaps(int element)
	{
		int min_index = minAddresses->at(element);
		min_heap.decrease_key(std::make_pair(element, count[element]), min_index);

		int max_index = maxAddresses->at(element);
		max_heap.decrease_key(std::make_pair(element, INT_MAX), std::make_pair(element, count[element]), max_index);
	}


	// decreases the count of the element by one
	inline void update_count(int element)
	{
		count[element]--;
	}

	// maintenance for removing a subset set
	void subset_update(std::vector<int>& set, int index, std::unordered_set<int>& out_elements)
	{
		for (auto& element : set)
		{
			// element is present in 1 fewer set
			update_count(element);
			remove_from_adj(element, index);

			out_elements.insert(element);
		}
		families[index].resize(0);
		deleted++;
	}

	// ## Private reduction routines ##
	void delete_elements(std::vector<int>& total)
	{
		// complexity = O(n^2 * m)
		// but may perform well! 
		// for (int& i : total)
		// {
		// 	for (int & j : adj[i])
		// 	{
		// 		// remove_from_adj(i, j);
		// 		remove_from_set(i, j);
		// 		if (families[j].size() == 0)
		// 		{
		// 			deleted++;
		// 		}
		// 		update_count(i);
		// 	}
		// }

		// complexity = O(m * n)
		// but may be slower than above algorithm. 
		for (int i = 0; i < families.size(); i++)
		{
			std::vector<int> new_set(families[i].size());
			std::vector<int> family = families[i];
			auto it = std::set_difference(family.begin(),
										  family.end(),
										  total.begin(),
										  total.end(),
										  new_set.begin());
			new_set.resize(it - new_set.begin());
			
			// maintain data structures in case set is cleared		
			if (new_set.size() == 0)
			{
				// we do not need to maintain adj[x] for x in total, since we remove x 
				// from any set. Hence, only count needs to be maintained
				for (int element : families[i])
				{
					update_count(element);
				}

				// maintain deleted
				deleted++;
			}
			families[i] = new_set;
		}
	}

	// ## Finalization ##
	void delete_empty_families()
	{
		std::vector<std::vector<int>> new_families(families.size());
		int j = 0;
		for (auto& set : families)
		{
			if (set.size() > 0)
			{
				new_families[j++] = set;
			}
		}
		new_families.resize(j);
		families = new_families;
	}

public:

	// ## INITIALIZATION ##
	Instance() {}

	// Resize the instance
	void resize(int universe_size, int families_size)
	{
		set_universe_size(universe_size);
		families.resize(families_size);
	}

	// Add a new set to the instance
	void add_family(std::vector<int>&& family)
	{
		last++;
		families[last] = std::move(family);
		for (int element : families[last])
		{
			count[element]++;
			adj[element].push_back(last);
		}
	}

	// After processing all the sets, construct internal heaps
	void initialize_heaps()
	{
		std::vector<std::pair<int, int>> min_vec(universe.size());
		std::vector<std::pair<int, int>> max_vec(universe.size());
		for (int i = 0; i < universe.size(); i++)
		{
			min_vec[i].first = i;
			min_vec[i].second = count[i];
			max_vec[i].first = i;
			max_vec[i].second = count[i];
		}

		using namespace std::placeholders;
		min_heap = MinHeap(min_vec, std::bind(&Instance::update_index_min, this, _1, _2));
		max_heap = MaxHeap(max_vec, std::bind(&Instance::update_index_max, this, _1, _2));
	}

	// ## QUERIES ##

	size_t universe_size()
	{
		return universe.size();
	}

	size_t families_size()
	{
		return families.size();
	}

	// returns a set
	std::vector<int> get_set(int i)
	{
		return families[i];
	}

	// returns the universe
	std::set<int> get_universe()
	{
		return universe;
	}

	// Returns whether there is an element in exactly one set
	bool has_unique_element()
	{
		if (min_heap.size() == 0)
			return false;
		return min_heap.peek_min().second == 1;
	}

	// Returns whether there is an element in every set (and at least two)
	bool has_common_element()
	{
		if (max_heap.size() == 0)
			return false;
		return max_heap.peek_max().second >= families.size() - deleted;
	}

	// Returns whether the instance is empty
	bool is_empty()
	{
		return universe.size() == 0;
	}

	// queries and removes subsets directly
	bool remove_equals()
	{
		bool result = false;
		std::map<std::vector<int>, std::vector<int>> subsets;
		for (int i = 0; i < families.size(); i++)
		{
			std::vector<int>& set = families[i];
			if (set.size() == 0) continue;
			if (subsets.count(set) > 0)
			{
				result = true;
				subsets.at(set).push_back(i);
			}
			else
			{
				std::vector<int> subset = { i };
				subsets.insert(std::make_pair(set, subset));
			}
		}

		// remove all but one set
		std::unordered_set<int> updated_elements;
		for (auto [key, list] : subsets)
		{
			for (int i = 0; i < list.size() - 1; i++)
			{
				int index = list[i];
				subset_update(families[index], index, updated_elements);
			}
		}
		for (auto element : updated_elements)
		{
			decrease_heaps(element);
		}

		return result;
	}

	bool remove_subsets()
	{
		std::vector<int> subsets(families.size());
		int j = 0;
		bool result = false;
		for (int i = 0; i < families.size(); i++)
		{
			std::vector<int>& set = families[i];

			if (set.size() == 0) continue;
			auto candidates = get_sets_by_list(set);
			if (candidates.size() > 1)
			{
				result = true;
				subsets[j++] = i;
			}
		}
		subsets.resize(j);

		// remove all (proper) subsets
		std::unordered_set<int> updated_elements;
		for (auto i : subsets)
		{
			subset_update(families[i], i, updated_elements);
		}
		for (auto element : updated_elements)
		{
			decrease_heaps(element);
		}

		return result;
	}

	int get_largest_set()
	{
		int largest = 0;
		int index = 0;
		for (uint32_t i = 0; i < families.size(); i++)
		{
			if (families[i].size() > largest)
			{
				largest = (int) families[i].size();
				index = i;
			}
		}
		return index;
	}

	// ## REDUCTION OPERATIONS ## 

	// TODO fix data structure maintenance
	void include_common_elements()
	{
		std::vector<int> commons;
		auto pair = max_heap.peek_max();

		while (pair.second >= families.size() - deleted)
		{
			commons.push_back(pair.first);
			max_heap.extract_max();

			int element = pair.first;
			min_heap.decrease_key(std::make_pair(element, -1),
								  minAddresses->at(element));
			min_heap.extract_min();

			if (max_heap.size() > 0)
			{
				pair = max_heap.peek_max();
			}
			else
			{
				break;
			}
		}

		sort(commons.begin(), commons.end());
		delete_elements(commons);
	}

	std::vector<int> include_unique_elements()
	{
		// find all the unique elements
		std::vector<int> uniques(universe.size());
		auto pair = min_heap.peek_min();
		int j = 0;
		while (pair.second == 1)
		{
			uniques[j++] = pair.first;
			min_heap.extract_min();

			if (min_heap.size() > 0)
			{
				pair = min_heap.peek_min();
			}
			else
			{
				break;
			}
		}
		uniques.resize(j);

		// find all distinct sets that cover all the unique elements
		std::set<int> unique_sets;
		for (auto const& x : uniques)
		{
			unique_sets.insert(adj[x][0]);
		}

		std::vector<int> result(unique_sets.begin(), unique_sets.end());
		deleted += (int) result.size();

		// must be a balanced BST s.t. total is eventually sorted.
		std::set<int> combined;
		for (auto const& set : result)
		{
			for (auto const& element : families[set])
			{
				combined.insert(element);
			}
		}

		std::vector<int> total(combined.begin(), combined.end());
		delete_elements(total); // also maintains count & deleted
		
		// maintaining heaps
		// we can remove all elements from total in max_heap
		for (auto const& element : total)
		{
			universe.erase(element);
			int max_index = maxAddresses->at(element);
			max_heap.increase_key(std::make_pair(element, INT_MAX), max_index);
			max_heap.extract_max();
		}

		// only a subset of total can be removed from min_heap
		std::vector<int> undeleted(total.size() - uniques.size());
		sort(uniques.begin(), uniques.end());
		auto it = std::set_difference(total.begin(), total.end(), uniques.begin(), uniques.end(), undeleted.begin());

		for (auto const& element : undeleted)
		{
			int min_index = minAddresses->at(element);
			min_heap.decrease_key(std::make_pair(element, -1), min_index);
			min_heap.extract_min();
		}

		return result;
	}

	// ## Solver stuff ##
	void include_set(int i)
	{
		std::vector<int> total = families[i];
		for (int element : total)
		{
			universe.erase(element);
			remove_from_heaps(element);
		}
		delete_elements(total);
	}

	// ## Finalization ##

	void clean_up()
	{
		delete_empty_families();
	}
};
