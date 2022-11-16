#include <iostream>
#include <string>

#include "graph.h"

using namespace graph;
int main()
{
	graph::Graph* g = new graph::Graph;
	std::string in_label = "Cat";
	g->create_node(in_label);
	std::string new_label = "Dog";
	g->create_node(new_label);
	std::map<std::string, graph::Node*> nodes = g->get_nodes();
	for (const auto& x: nodes)
	{
	//std::cout << x.first << "\t" << x.second << std::endl;
	}
	std::string edge_label = "Hates";
	g->create_edge(edge_label, in_label, new_label);
	g->create_edge(edge_label, new_label, in_label);
	std::unordered_map<std::string, std::vector<Edge*> > edges = g->get_edges();


	for (const auto& x: edges)
	{
	std::cout << x.first << "\t" << x.second.size() << "\n";
	}



	delete g; // Seems like there is something wrong with the destructor. When I comment out this line everything works fine
	// but when it isn't commented, I get an error message when trying to compile.

}
