#include <iostream>
#include <string>

#include "graph.h"


int main()
{
	graph::Graph* g = new graph::Graph;
	std::string in_label = "is";
	g->create_node(in_label);
	//std::vector<Node*> nodes = g->get_nodes();
	std::cout << g->get_nodes().at(0);


	delete g;

}
