#include <graph.h>
#include <vector>
#include <unordered_map>


class Graph
{
	void create_node(in_label) // should label and edges be passed as an argument?
	// We have to do that directly in with passing arguments here and passing them to Node constructor
	// either that or we have to implement set functions for node class.
	{
	 Node new_node = Node(in_label);
	 this->nodes.push_back(new_node);

	}

	void create_edge(in_label)
	{
		Node new_edge = Edge(in_label);
		if (this->edges.find(in_label) == this->edges.end()) // Checks whether the label is in the map
		{
			std::vector<Edge*> e; // intializes vector of Edge*
			e.push_back(new_edge); //Adds new_edge to vector
			this->edges.insert(in_label, e);
		}
		else
		{

			this->edges.at(in_label).push_back(new_edge)
		}

	}
};
