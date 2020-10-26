#include <utility>
#include <vector>
#include <functional>

#pragma once
class MaxHeap
{
private:
	std::vector<std::pair<int, int>> heap;

	int left_child(int index)
	{
		return 2 * index + 1;
	}
	int right_child(int index)
	{
		return 2 * index + 2;
	}
	int parent(int index)
	{
		return (index - 1) / 2;
	}

	void build_min()
	{
		for (int i = size() / 2; i >= 0; i--)
		{
			max_heapify(i);
		}
	}

	void max_heapify(int index)
	{
		int left = left_child(index);
		int right = right_child(index);
		int largest = 0;
		if (left < size() && heap[left].second > heap[index].second)
		{
			largest = left;
		}
		else
		{
			largest = index;
		}

		if (right < size() && heap[right].second > heap[largest].second)
		{
			largest = right;
		}

		if (largest != index)
		{
			std::pair<int, int> temp = heap[index];
			// fix bookkeeping
			update_index(heap[largest].first, index);
			update_index(temp.first, largest);

			// swap
			heap[index] = heap[largest];
			heap[largest] = temp;
			max_heapify(largest);
		}
	}

	std::function<void(int, int)> update_index;

public:

	MaxHeap()
	{

	}

	MaxHeap(std::vector<std::pair<int, int>> ar, std::function<void(int, int)> func)
	{
		heap = ar;
		update_index = func;
		build_min();
	}

	void decrease_key(std::pair<int, int> max, std::pair<int, int> element, int index)
	{
		increase_key(max, index);
		extract_max();
		insert(element);
	}

	void increase_key(std::pair<int, int> element, int index)
	{
		std::pair<int, int> original = heap[index];
		if (element < original)
		{
			return;
		}

		heap[index] = element;
		while (index > 0 && heap[parent(index)].second < heap[index].second)
		{
			std::pair<int, int> temp = heap[parent(index)];
			// fix bookkeeping
			update_index(temp.first, index);
			update_index(heap[index].first, parent(index));

			// swap
			heap[parent(index)] = heap[index];
			heap[index] = temp;
			index = parent(index);
		};
	}

	const std::pair<int, int>& peek_max()
	{
		return heap[0];
	}

	const std::pair<int, int>& operator[](int index)
	{
		return heap[index];
	}

	std::pair<int, int> extract_max()
	{
		std::pair<int, int> element = heap[0];
		// fix bookkeeping
		update_index(heap[size() - 1].first, 0);
		// swap
		heap[0] = heap[size() - 1];
		heap.pop_back();
		max_heapify(0);
		return element;
	}

	void insert(std::pair<int, int> element)
	{
		heap.push_back(element);
		int index = size() - 1;
		update_index(element.first, index);
		while (index > 0 && heap[parent(index)].second < heap[index].second)
		{
			std::pair<int, int> temp = heap[parent(index)];

			// fix bookkeeping
			update_index(temp.first, index);
			update_index(heap[index].first, parent(index));

			// swap
			heap[parent(index)] = heap[index];
			heap[index] = temp;
			index = parent(index);
		}
	}

	size_t size()
	{
		return heap.size();
	}
};