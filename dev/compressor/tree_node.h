#ifndef TREE_NODE
#define TREE_NODE

#include <vector>
#include <algorithm>

class tree_node
{
public:
	uint32_t cost;
	uint8_t character;
    tree_node *left, *right;

	tree_node();

	void free_tree();
};

tree_node build_tree(std::vector<tree_node>& arr);

#endif // tree_node
