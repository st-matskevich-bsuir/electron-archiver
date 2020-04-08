#include "tree_node.h"

tree_node::tree_node()
{
	character = 0;
	cost = 0;
	left = 0;
	right = 0;
}

void tree_node::free_tree()
{
	if (left != 0)
	{
		left->free_tree();
		delete left;
	}

	if (right != 0)
	{
		right->free_tree();
		delete right;
	}
}

struct greaters {
	bool operator()(const tree_node& a, const tree_node& b) const {
		return a.cost > b.cost;
	}
};

tree_node build_tree(std::vector<tree_node>& arr)
{
	std::make_heap(arr.begin(), arr.end(), greaters());

	while (arr.size() > 1)
	{
		std::pop_heap(arr.begin(), arr.end(), greaters());
		tree_node a = arr.back();
		arr.pop_back();

		std::pop_heap(arr.begin(), arr.end(), greaters());
		tree_node b = arr.back();
		arr.pop_back();

		tree_node c;
		c.left = new tree_node();
		*c.left = a;

		c.right = new tree_node();
		*c.right = b;
		c.cost = a.cost + b.cost;

		arr.push_back(c);
		std::push_heap(arr.begin(), arr.end(), greaters());
	}

	return arr[0];
}
