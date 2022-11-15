#include <vector>
#include <unordered_map>
#include <string>

#include "graph.h"
namespace graph
{
	/*
	void create_node(std::string in_label) // should label and edges be passed as an argument?
	// We have to do that directly in with passing arguments here and passing them to Node constructor
	// either that or we have to implement set functions for node class.
	{
	 Node new_node = Node(in_label);
	 this->nodes.push_back(&new_node);

	}

	 */
	void Graph::create_edge(std::string in_label, std::string head_node_name, std::string tail_node_name) //We need head_node and tail_node here right? Or else we have an edge
	// that doesn't connect anything.
	{
		Node* head_node = new Node(head_node_name);
		Node* tail_node = new Node(tail_node_name);
		Edge* new_edge = new Edge(in_label, head_node, tail_node); // is this pointer handled safely? When does it get deallocated? With the graph object?
		if (this->edges.find(in_label) == this->edges.end()) // Checks whether the label is in the map
		{
			std::vector<Edge*> e; // intializes vector of Edge*
			e.push_back(new_edge); //Adds new_edge to vector
			this->edges.insert({in_label, e});
		}
		else
		{
			this->edges.at(in_label).push_back(new_edge);
		}

	}

	// Destructor
	Graph::~Graph()
	{
		// This I basically just took from undir-inclist-graph.cpp
		 // debug output
		   std::clog << "\tlog output: calling UndirInclistGraph destructor\n\t\t(this == " << this << ")\n";

		   // deallocate all the edges
		   //for(auto iter = this->edges.begin(); iter != this->edges.end(); iter++) delete *iter;
	}


}
