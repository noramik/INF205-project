#include <iostream>
#include <string>

#include "graph.h"


int main()
{
	graph::Graph* g = new graph::Graph;
	std::string in_label = "is";
	g->create_node(in_label);
	std::map<std::string, graph::Node*> nodes = g->get_nodes();
	for (const auto& x: nodes)
	{
	std::cout << x.first << x.second << std::endl;
	}


	//delete g; // Seems like there is something wrong with the destructor. When I comment out this line everything works fine
	// but when it isn't commented, I get an error message when trying to compile.

}
