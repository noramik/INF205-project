#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "graph.h"

using namespace graph;

	Node* Graph::create_node(std::string node_label)
	{
		if (this->nodes.find(node_label) == this->nodes.end()) //Checks whether the node already exists in the nodes map, if it doesn't it creates it.
		{
			Node* new_node = new Node(node_label); // This need to be on the heap because we want the actual new_node to be returned, not a copy of it
			this->nodes.insert({node_label, new_node});
			return new_node;
		}
		else
		{
			 return nodes.find(node_label)->second; // Finds the node_label in the map, and returns the Node*
		}
	}

	void Graph::create_edge(std::string edge_label, std::string head_node_name, std::string tail_node_name) //We need head_node and tail_node here right? Or else we have an edge
	// that doesn't connect anything.
	{

		Node* head_node = this->create_node(head_node_name);

		Node* tail_node = this->create_node(tail_node_name);


		Edge* new_edge = new Edge(edge_label, head_node, tail_node); // is this pointer handled safely? When does it get deallocated? With the graph object?
		head_node->append_next_edges(new_edge); // Would this be correct?
		tail_node->append_prev_edges(new_edge);
		if (this->edges.find(edge_label) == this->edges.end()) // Checks whether the label is in the map
		{
			std::vector<Edge*> e; // intializes vector of Edge*
			e.push_back(new_edge); //Adds new_edge to vector
			this->edges.insert({edge_label, e});
		}
		else
		{
			this->edges.at(edge_label).push_back(new_edge);
		}



	}

	// Destructor
	Graph::~Graph()
	{
		// This I basically just took from undir-inclist-graph.cpp
		 // debug output
		   std::clog << "\tlog output: calling UndirInclistGraph destructor\n\t\t(this == " << this << ")\n";

		   // Access the vector of Edge*, then iterate through each Edge* element in that vector and deallocate.
		   for (auto iter = this->edges.begin(); iter != this->edges.end(); iter++)
		   {
			   std::vector<Edge*> edge_vector = iter->second;
			   for (auto x=edge_vector.begin(); x != edge_vector.end(); x++)
				   {
				   delete *x; // Not sure if this works or not, but at least no error messages.
				   }
		   }

		   this->edges.clear(); // Not sure if this is needed, clears the whole map, both key and val.

		   // Deleting Node* on the heap
		   // Do I need to delete the key as well? that shouldn't be on the heap though? It's just std::string
		   for (auto iter = this->nodes.begin(); iter != this->nodes.end(); iter++)
			   {
				   Node* node = iter->second;

				   delete node; // Not sure if this works or not, but at least no error messages. Does this actually destroy the node itself or just the pointer to it?
			   }
		   this->nodes.clear(); // Not sure if this is needed, clears the whole map, both key and val.


		//Need to remember to delete the pointers from Node and Edge class as well
	}

	//Copy constructor
	Graph::Graph(const Graph& orig)
	{
		std::clog << "\tlog output: calling Graph copy constructor\n\t\t(this == " << this << ")\n";
		for(auto it = orig.edges.begin(); it != orig.edges.end(); it++)
		{
			std::vector<Edge*> edge_vec = it->second;
			for(auto vec_it = edge_vec.begin(); vec_it != edge_vec.end(); vec_it++)
			{
				// Access edge label, head node label and tail node label and give them as parameters to create_edge function.
				this->create_edge(it->first, (*vec_it)->get_head_node()->get_label(), (*vec_it)->get_tail_node()->get_label());
			}
		}

		for (auto i = orig.nodes.begin(); i != orig.nodes.end(); i++)
		{
			this->create_node(i->first); //Do I need this
		}


	}

	// Copy assignment
	// Temp version, currently not working
	Graph& Graph::operator=(const Graph& rhs)
	{

		// This I basically just took from undir-inclist-graph.cpp
		 // debug output
		   std::clog << "\tlog output: calling Graph copy assignment operator\n\t\t(this == " << this << ")\n";


		   // Deleting old edges and node

		   // Deleting edges
		   for (auto it =this->edges.begin(); it != this->edges.end(); it++)
		   {
			   for (auto vec_it = it->second.begin(); vec_it != it->second.end(); vec_it++)
			   {
				   delete *vec_it;
			   }
		   }
		   this->edges.clear(); // Clears the whole unordered map, keys and vals

			// Deleting nodes
			for (auto it = this->nodes.begin(); it !=this->nodes.end(); it++)
			{
				delete it->second;
			}
			this->nodes.clear(); // Clears the whole nodes map, both keys and vals


		   //Copying from rhs
		   for (auto it = rhs.edges.begin(); it != rhs.edges.end(); it++)
		   {
			   for (auto vec_it = it->second.begin(); vec_it != it->second.end(); vec_it++)
			   {
				   this->create_edge((*vec_it)->get_label(), (*vec_it)->get_head_node()->get_label(), (*vec_it)->get_tail_node()->get_label());
			   }
		   }
		   for (auto it = rhs.nodes.begin(); it != rhs.nodes.end(); it++)
		   {
			   this->create_node(it->first);
		   }
		   return *this;


	}

	// Implementation from lecture code - directed-graph
	// create a single edge based on information from the stream
	// if it fails, return false, otherwise return true
	bool Graph::generate_edge_from(std::istream* source)
	{
	   char single_symbol = '\0';

	   // read label of the first node
	   while(single_symbol != '<')
	   {
	      *source >> single_symbol;
	      if(single_symbol == '\0') return false;  // format: \0 means that we are done
	   }
	   std::string node_a_label = "";
	   for(*source >> single_symbol; single_symbol != '>'; *source >> single_symbol)
	      node_a_label += single_symbol;
	   if(node_a_label == "") return false;  // format: empty label means that we are done


	   // read edge label
	   while(single_symbol != '<')
	   {
		  *source >> single_symbol;
		  if(single_symbol == '\0') return false;  // format: \0 means that we are done
	   }
	   std::string edge_label = "";
	   for(*source >> single_symbol; single_symbol != '>'; *source >> single_symbol)
		  edge_label += single_symbol;
	   if(edge_label == "") return false;


	   // read label of the second node
	   while(single_symbol != '<')
	   {
	      *source >> single_symbol;
	      if(single_symbol == '\0') return false;  // format: \0 means that we are done
	   }
	   std::string node_b_label = "";
	   for(*source >> single_symbol; single_symbol != '>'; *source >> single_symbol)
	      node_b_label += single_symbol;
	   if(node_b_label == "") return false;  // format: empty label means that we are done

	   this->create_edge(edge_label, node_a_label, node_b_label);
	   return true;
	}

	/*
	 * Prints out graph in same format at input file.
	 */
	void Graph::print_graph(std::ostream* target)
	{
		for (auto it = this->edges.begin(); it != this->edges.end(); it++)
		{
			std::vector<Edge*> edge_vec = it->second;

			for (auto edge_it = edge_vec.begin(); edge_it != edge_vec.end(); edge_it++)
			{
				Edge edge_obj = **edge_it; // The edge object
				*target << "<" << edge_obj.get_head_node()->get_label() << ">" << "\t" << "<" << it->first << ">" <<  "\t" << "<" << edge_obj.get_tail_node()->get_label()<< ">"  <<"\n";
			}

		}
	}
	 //I/O stream operator overloading for Graph
	// Copied from lecture code
	std::istream& operator>>(std::istream& is, graph::Graph& g) {
	   g.in(&is);
	   return is;
	}

	std::ostream& operator<<(std::ostream& os, graph::Graph& g)
	{
		g.out(&os);
		return os;
	}
