#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "graph.h"
//#include "query.h" // I don't think this was included in directed-graph example but I don't get code to run without it

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

		   // Access the vector of Edge*, then iterate through each Edge* element in that vector and deallocate.
		   for (auto iter : this->edges)
		   {
			   std::vector<Edge*> edge_vector = iter.second;
			   for (auto x=edge_vector.begin(); x != edge_vector.end(); x++) delete *x; // Not sure if this works or not, but at least no error messages.
		   }

		   // Deleting Node* on the heap
		   // Do I need to delete the key as well? that shouldn't be on the heap though? It's just std::string
		   for (auto iter : this->nodes)
		   		   {
		   			   Node* node = iter.second;
		   			   delete node; // Not sure if this works or not, but at least no error messages.
		   		   }
	}


	// Implementation from lecture code - directed-graph
	// create a single edge based on information from the stream
	// if it fails, return false, otherwise return true
	bool Graph::generate_edge_from(std::istream* source)
	{
	   // read label of the first node
	   std::string head_label = "";
	   if(!IRI_input(&head_label, source)) return false;

	   // read edge label
	   std::string edge_label = "";
	   if(!IRI_input(&edge_label, source)) return false;

	   // read label of the second node
	   std::string tail_label = "";
	   if(!IRI_input(&tail_label, source)) return false;

	   this->create_edge(edge_label, head_label, tail_label);
	   return true;
	}


	 //I/O stream operator overloading for Graph
	// Copied from lecture code
	std::istream& operator>>(std::istream& is, Graph& g) {
	   g.in(&is);
	   return is;
	}
}
