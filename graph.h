#ifndef GRAPH
#define GRAPH

<<<<<<< HEAD

/*
class Node
public:
Constructor (label)
Constructor() //default constructor

str get_label()
array get_next_edges()
array get_prev_edges()
void append_prev_edges()
void append_next_edges()


private:
str label = "";
Edge* prev_edges[] = nullptr;
Edge* next_edges[] = nullptr;


class Edge
public:
Constructor(label, head_node, tail_head)
Constructor() //default constructor

str get_label()
str get_nodes(head_or_tail)
void set_node(head_or_tail)


private:
str label = ";
Node* head_node = nullptr; //probably, need default value
Node* tail_node = nullptr;


class Graph

public:
Constructor()
void create_node()
void create_edge()
void generate_graph(source) // reading from file and generating graph.
print_graph()?

multimap get_edges() //get the multimap
Node* get_node_pointer_by_unique_label() (?)

type T find_pattern(path p, path q, return_nodes=False);  #False betyr True/False answer. True betyr finn alle noder og returner med label.
# FInn ut av hvordan returnere to ulike ting

Destructor() // default destructor or additional functionality for handling pointers?

Copy constructor and assignement or forbid copying.
Move_constructor()?
move_assignment()?


private:
multimap? edges; //Labels as keys, pointers to edge object as value. Maybe list of pointers as values, and then use map instead of multimap.
nodes? // Do we need this?
=======
#include <string>
#include <vector>
#include <iostream>
#include <map>

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
	void append_prev_edges(new_edge) {this->prev_edges.push_back(new_edge);}
	void append_next_edges(new_edge) {this->next_edges.push_back(new_edge);}



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
	}


	std::string get_label() const {return this->label;};
	Node* get_head_node() const {return this->head_node;}
	Node* get_tail_node() const {return this->tail_node;}
	void set_head_node(new_node) {this->head_node = new_node;}
	void set_tail_node(new_node) {this->tail_node = new_node;}


	private:
	std::string label = "";
	Node* head_node = nullptr; //probably, need default value
	Node* tail_node = nullptr;
	};
	/*
	class Graph

	public:
	Constructor()
	void create_node()
	void create_edge()
	void generate_graph(source) // reading from file and generating graph.
	print_graph()?

	multimap get_edges() //get the multimap
	Node* get_node_pointer_by_unique_label() (?)

	find_pattern(path p, path q, return_nodes=False);  #False betyr True/False answer. True betyr finn alle noder og returner med label.

	Destructor() // default destructor or additional functionality for handling pointers?

	Copy constructor and assignement or forbid copying.
	Move_constructor()?
	move_assignment()?


	private:
	multimap? edges; //Labels as keys, pointers to edge object as value. Maybe list of pointers as values, and then use map instead of multimap.
	nodes? // Do we need this
*/
}
>>>>>>> 1c1dab84204acbcc4c9bad5f29926a503e32a7c2
