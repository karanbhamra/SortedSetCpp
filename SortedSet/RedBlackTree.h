#pragma once

#include <memory>
#include <iostream>
#include <stack>
#include <vector>

template<typename T>
struct RedBlackNode
{
public:
	bool IsRed{};
	std::shared_ptr<RedBlackNode<T>> Left{};
	std::shared_ptr<RedBlackNode<T>> Right{};
	T Value{};

	RedBlackNode(T val) : Value(val)
	{
		IsRed = true;
	}
};

template<typename T>
class RedBlackTree
{
private:
	std::shared_ptr<RedBlackNode<T>> Add(T Value, std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> Remove(T Value, std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> MoveRedLeft(std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> MoveRedRight(std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> Fixup(std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> Minimum(std::shared_ptr<RedBlackNode<T>> node);
	
	void inOrderTraversal(std::vector<std::shared_ptr<RedBlackNode<T>>>& vec, std::shared_ptr<RedBlackNode<T>> node) const;

	bool Contains(T Value, std::shared_ptr<RedBlackNode<T>> Node) const;

	size_t count{};
	std::shared_ptr<RedBlackNode<T>> root{};
public:
	RedBlackTree() = default;

	~RedBlackTree();

	void Add(T Value);

	void FlipColor(std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> GetRoot() const
	{
		return root;
	}

	std::shared_ptr<RedBlackNode<T>> RotateLeft(std::shared_ptr<RedBlackNode<T>> Node);

	std::shared_ptr<RedBlackNode<T>> RotateRight(std::shared_ptr<RedBlackNode<T>> Node);

	bool Remove(T Value);

	bool Contains(T Value) const;

	size_t Size() const
	{
		return count;
	}

	void Clear();

	std::vector<std::shared_ptr<RedBlackNode<T>>> InOrderTraversal() const;
};

template<typename T>
RedBlackTree<T>::~RedBlackTree()
{
	Clear();
}

template<typename T>
void RedBlackTree<T>::Clear()
{
	count = 0;
	std::stack<std::shared_ptr<RedBlackNode<T>>> stack{};
	if (root)
	{
		stack.push(root);
		while (!stack.empty())
		{
			root = std::move(stack.top());
			stack.pop();
			if (root->Right)
			{
				stack.push(root->Right);
			}
			if (root->Left)
			{
				stack.push(root->Left);

			}
			root.reset();
		}
	}
}

template<typename T>
std::vector<std::shared_ptr<RedBlackNode<T>>> RedBlackTree<T>::InOrderTraversal() const
{
	std::vector<std::shared_ptr<RedBlackNode<T>>> values{};

	inOrderTraversal(values, root);

	return std::move(values);
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::RotateRight(std::shared_ptr<RedBlackNode<T>> Node)
{
	auto left = Node->Left;
	left->IsRed = Node->IsRed;
	Node->IsRed = true;
	Node->Left = std::move(left->Right);
	auto tempNode = Node;
	left->Right = std::move(tempNode);

	return left;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::RotateLeft(std::shared_ptr<RedBlackNode<T>> Node)
{
	auto right = Node->Right;
	right->IsRed = Node->IsRed;
	Node->IsRed = true;
	Node->Right = std::move(right->Left);
	auto tempNode = Node;
	right->Left = std::move(tempNode);


	return right;
}

template<typename T>
void RedBlackTree<T>::Add(T Value)
{
	root = Add(Value, root);
	root->IsRed = false;
	count++;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::Add(T Value, std::shared_ptr<RedBlackNode<T>> Node)
{
	if (!Node)
	{
		return std::make_shared<RedBlackNode<T>>(Value);
	}

	if (Node->Right && Node->Left && Node->Right->IsRed && Node->Left->IsRed)
	{
		FlipColor(Node);
	}

	if (Value >= Node->Value)
	{
		Node->Right = Add(Value, Node->Right);
	}
	else
	{
		Node->Left = Add(Value, Node->Left);
	}

	if (Node->Right && Node->Right->IsRed)
	{
		Node = std::move(RotateLeft(Node));
	}
	if (Node->Left && Node->Left->Left && Node->Left->IsRed && Node->Left->Left->IsRed)
	{
		Node = std::move(RotateRight(Node));
	}

	return Node;
}

template<typename T>
void RedBlackTree<T>::FlipColor(std::shared_ptr<RedBlackNode<T>> Node)
{
	Node->IsRed = !Node->IsRed;
	if (Node->Right)
	{
		Node->Right->IsRed = !Node->Right->IsRed;
	}
	if (Node->Left)
	{
		Node->Left->IsRed = !Node->Left->IsRed;
	}
}

template<typename T>
bool RedBlackTree<T>::Contains(T Value) const
{
	return Contains(Value, root);
}


template<typename T>
bool RedBlackTree<T>::Contains(T Value, std::shared_ptr<RedBlackNode<T>> Node) const
{
	if (!Node)
	{
		return false;
	}


	if (Value > Node->Value)
	{
		return Contains(Value, Node->Right);
	}
	else if (Value < Node->Value)
	{
		return Contains(Value, Node->Left);
	}

	return true;
}

template<typename T>
bool RedBlackTree<T>::Remove(T Value)
{
	if (!Contains(Value))
	{
		return false;
	}
	root = Remove(Value, root);
	count--;
	return true;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::Remove(T Value, std::shared_ptr<RedBlackNode<T>> Node)
{
	if (Value < Node->Value)
	{
		if (!Node->Left->IsRed && Node->Left->Left && !Node->Left->Left->IsRed)
		{
			Node = std::move(MoveRedLeft(Node));
		}

		Node->Left = std::move(Remove(Value, Node->Left));
	}
	else
	{
		if (Node->Left && Node->Left->IsRed)
		{
			Node = std::move(RotateRight(Node));
		}

		if (Node->Value == Value && !Node->Right && !Node->Left)
		{
			Node = std::move(nullptr);
			return Node;
		}

		if (Node->Right)
		{
			if (!Node->Right->IsRed && Node->Right->Left && !Node->Right->Left->IsRed)
			{
				Node = std::move(MoveRedRight(Node));
			}

			if (Node->Value == Value)
			{
				std::shared_ptr<RedBlackNode<T>> min = Minimum(Node->Right);
				Node->Value = min->Value;
				Node->Right = std::move(Remove(min->Value, Node->Right));
			}
			else
			{
				Node->Right = std::move(Remove(Value, Node->Right));
			}

		}
	}

	return Fixup(Node);
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::MoveRedLeft(std::shared_ptr<RedBlackNode<T>> Node)
{
	FlipColor(Node);

	if (Node->Right && Node->Right->Left && Node->Right->Left->IsRed)
	{
		Node->Right = std::move(RotateRight(Node->Right));
		Node = std::move(RotateLeft(Node));
		FlipColor(Node);

		if (Node->Right && Node->Right->Right && Node->Right->Right->IsRed)
		{
			Node->Right = std::move(RotateLeft(Node->Right));
		}
	}
	return Node;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::MoveRedRight(std::shared_ptr<RedBlackNode<T>> Node)
{
	FlipColor(Node);
	if (Node->Left && Node->Left->Left && Node->Left->Left->IsRed)
	{
		Node = std::move(RotateRight(Node));
		FlipColor(Node);
	}
	return Node;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::Fixup(std::shared_ptr<RedBlackNode<T>> Node)
{
	if (Node->Right && Node->Right->IsRed)
	{
		Node = std::move(RotateLeft(Node));
	}

	if (Node->Left && Node->Left->Left && Node->Left->IsRed && Node->Left->Left->IsRed)
	{
		Node = std::move(RotateRight(Node));
	}

	if (Node->Right && Node->Left && Node->Right->IsRed && Node->Left->IsRed)
	{
		FlipColor(Node);
	}

	if (Node->Left && Node->Left->Right && Node->Left->Left && Node->Left->Right->IsRed && !Node->Left->Left->IsRed)
	{
		Node->Left = std::move(RotateLeft(Node->Left));
		if (Node->Left->IsRed)
		{
			Node = std::move(RotateRight(Node));
		}
	}

	return Node;
}

template<typename T>
std::shared_ptr<RedBlackNode<T>> RedBlackTree<T>::Minimum(std::shared_ptr<RedBlackNode<T>> Node)
{
	std::shared_ptr<RedBlackNode<T>> current = Node;
	while (Node->Left)
	{
		Node = Node->Left;
	}
	return current;
}

template<typename T>
void RedBlackTree<T>::inOrderTraversal(std::vector<std::shared_ptr<RedBlackNode<T>>>& vec, std::shared_ptr<RedBlackNode<T>> node) const
{
	if (!node) return;

	inOrderTraversal(vec, node->Left);
	vec.push_back(node);
	inOrderTraversal(vec, node->Right);
}
