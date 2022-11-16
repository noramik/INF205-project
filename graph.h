#ifndef GRAPH
#define GRAPH


#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>

#include "query.h"

namespace graph
{
	class Edge; // Edge class is defined here to avoid problems when we declare variables as Edge* in Node class.
	// Implementation is further down in the file.

	class Node
	{
	public:
	Node() {} //default constructor
	Node(std::string in_label) {this->label = in_label;} // Constructor


	std::string get_label() const {return this->label;} // spørsmål: må det være const her? hvorfor?
	std::vector<Edge*> get_prev_edges() const {return this->prev_edges;} // returning edges leading to the node. Can this even be const as prev_edges might change
	std::vector<Edge*> get_next_edges() const {return this->next_edges;} // returning edges going from the node. Can this even be const as next_edges might change
	void append_prev_edges(Edge* new_edge) {this->prev_edges.push_back(new_edge);}
	void append_next_edges(Edge* new_edge) {this->next_edges.push_back(new_edge);}



	private:
	std::string label = "";
	std::vector<Edge*> prev_edges = std::vector<Edge*>(); // this is how it's done in undir-inclist-graph.h instead of nullptr
	// Is this the way it needs to be done? What does it do? Creates an empty vector? uses edge default constructor?
	std::vector<Edge*> next_edges = std::vector<Edge*>();
	};

	class Edge
	{
	public:
	Edge() {} //default constructor
	Edge(std:: string in_label, Node* head_node, Node* tail_node) {
		this->label=in_label;
		this->set_head_node(head_node);
		this->set_tail_node(tail_node);
	};


	std::string get_label() const {return this->label;};
	Node* get_head_node() const {return this->head_node;}
	Node* get_tail_node() const {return this->tail_node;}
	void set_head_node(Node* new_node) {this->head_node = new_node;}
	void set_tail_node(Node* new_node) {this->tail_node = new_node;}


	private:
	std::string label = "";
	Node* head_node = nullptr; //probably, need default value
	Node* tail_node = nullptr;
	};

	class Graph
	{
	public:
	Graph() {}; // Constructor, to be implemented
	void create_node(std::string in_label){
		Node* new_node = new Node(in_label);
		this->nodes.insert({in_label, new_node});
	};
	void create_edge(std::string in_label, std::string head_node_name, std::string tail_node_name);
	std::map<std::string, Node*> get_nodes() const {return this->nodes;}
	std::unordered_map<std::string, std::vector<Edge*> > get_edges() const {return this->edges;}
	// in method copied from directed-graph example from lecture.
    // read from *source
    void in(std::istream* source) {
       while(this->generate_edge_from(source)) {}
    };// read edge by edge, until generate_edge_from returns false
	/*
	void generate_graph(source) // reading from file and generating graph.
	print_graph()?

	multimap get_edges() //get the multimap
	Node* get_node_pointer_by_unique_label() (?)

	find_pattern(path p, path q, return_nodes=False);  #False betyr True/False answer. True betyr finn alle noder og returner med label.
	*/
	~Graph();// default destructor or additional functionality for handling pointers?
	/*
	Copy constructor and assignement or forbid copying.
	Move_constructor()?
	move_assignment()?
	*/


	private:
	// Should we use Edge* and Node* in the maps? I notice in the undir-inclist-graph.h, pointers aren't used, instead the Node object itself is used.
	std::unordered_map<std::string, std::vector<Edge*> > edges; //Labels as keys, pointers to edge object as value. Maybe list of pointers as values, and then use map instead of multimap.
	// Used unordered_map instead of map. Usually faster, i think for our use-case, an ordered map wouldn't be neccessary either way.
	std::map<std::string, Node*> nodes; // What data stucture here? Map? Set? vector?
	bool generate_edge_from(std::istream* source); // Implementation of this copied from directed-graph example.
	};
}
#endif
