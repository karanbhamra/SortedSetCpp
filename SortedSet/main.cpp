// SortedSet.cpp : Defines the entry point for the application.
//

#include "main.h"

int main()
{
	std::cout << std::boolalpha;

	SortedSet<int> set1{};
	SortedSet<int> set2{};
	

	set1.Add(5);
	set1.Add(15);
	set1.Add(35);
	set1.Add(55);

	set2.Add(-6);
	set2.Add(4);
	set2.Add(5);
	
	auto newset = set1.Union(set2);
	auto temp = newset.InOrderTraversal();
	auto newset2 = set1.Intersection(set2);
	return 0;
}
