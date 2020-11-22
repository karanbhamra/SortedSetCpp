#pragma once
#include <stdexcept>
#include "RedBlackTree.h"

template<typename T>
class SortedSet
{
private:
	RedBlackTree<T> tree{};

	
public:

	void Clear()
	{
		tree.Clear();
	}

	size_t Size() const
	{
		auto count = tree.Size();

		return count;
	}

	bool Add(T value);

	template<typename Iterator>
	void AddRange(Iterator begin, Iterator end);

	bool Contains(T value) const;

	bool Remove(T value);

	T Max() const;

	T Min() const;

	T Ceiling(T value) const;

	T Floor(T value) const;

	SortedSet<T> Union(const SortedSet<T>& other) const;

	SortedSet<T> Intersection(const SortedSet<T>& other) const;

	std::vector<std::shared_ptr<RedBlackNode<T>>> InOrderTraversal() const
	{
		auto vals = tree.InOrderTraversal();

		return vals;
	}

};

template<typename T>
bool SortedSet<T>::Add(T value)
{
	auto prevSize = tree.Size();

	if (Contains(value)) return false;

	tree.Add(value);

	if (tree.Size() > prevSize) return true;

	return false;
}

template<typename T>
bool SortedSet<T>::Contains(T value) const
{
	return tree.Contains(value);
}

template<typename T>
bool SortedSet<T>::Remove(T value)
{
	return tree.Remove(value);
}

template<typename T>
T SortedSet<T>::Max() const
{
	if (Size() == 0) 
	{
		throw std::exception("Set is empty.");
	}

	auto current = tree.GetRoot();

	while (current->Right)
	{
		current = current->Right;
	}

	return current->Value;
}

template<typename T>
T SortedSet<T>::Min() const
{
	if (Size() == 0)
	{
		throw std::exception("Set is empty.");
	}

	auto current = tree.GetRoot();

	while (current->Left)
	{
		current = current->Left;
	}

	return current->Value;
}

template<typename T>
SortedSet<T> SortedSet<T>::Union(const SortedSet<T>& other) const
{
	SortedSet<T> newSet{};

	auto firstVals = InOrderTraversal();
	auto secondVals = other.InOrderTraversal();

	newSet.AddRange(firstVals.begin(), firstVals.end());
	newSet.AddRange(secondVals.begin(), secondVals.end());


	return std::move(newSet);	// unnecessary due to return optimizations for newer c++ standard
}

template<typename T>
SortedSet<T> SortedSet<T>::Intersection(const SortedSet<T>& other) const
{
	SortedSet<T> newSet{};

	auto firstVals = InOrderTraversal();
	auto secondVals = other.InOrderTraversal();


	if (firstVals.size() < secondVals.size())
	{
		for (auto node : firstVals) 
		{
			if (other.Contains(node->Value))
			{
				newSet.Add(node->Value);
			}
		}
	}
	else
	{
		for (auto node : secondVals)
		{
			if (Contains(node->Value))
			{
				newSet.Add(node->Value);
			}
		}
	}

	return std::move(newSet);
}

template<typename T>
template<typename Iterator>
void SortedSet<T>::AddRange(Iterator begin, Iterator end)
{
	while (begin != end)
	{
		auto val = *begin;
		Add(val->Value);
		begin++;
	}
}
