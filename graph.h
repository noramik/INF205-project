#ifndef GRAPH
#define GRAPH


#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>

/*
 * Directed Graph
 * Memory managment rules:
 * The graph object owns its nodes and edges.
 * The edge and node objects don't delete each other, they just have non-owning pointers to each other.
 */

namespace graph
{
	/*
	 * Edge class is defined here to avoid problems when we declare variables as Edge* in Node class.
	 * Implementation is further down in the file.
	 */
	class Edge;

	class Node
	{
	public:
	Node() {} // Default constructor
	Node(std::string in_label) {this->label = in_label;} // Constructor


	std::string get_label() const {return this->label;} // Returns edge label
	std::vector<Edge*> get_prev_edges() const {return this->prev_edges;} // returning edges leading to the node.
	std::vector<Edge*> get_next_edges() const {return this->next_edges;} // returning edges going out from the node.
	void append_prev_edges(Edge* new_edge) {this->prev_edges.push_back(new_edge);} // Appends new edge to the prev_edges vector
	void append_next_edges(Edge* new_edge) {this->next_edges.push_back(new_edge);} // Appends new edge to the next_edges vector




	private:
	std::string label = "";
	std::vector<Edge*> prev_edges = std::vector<Edge*>();
	std::vector<Edge*> next_edges = std::vector<Edge*>();
	};

	class Edge
	{
	public:
	Edge() {} //default constructor
	// Constructor
	Edge(std:: string in_label, Node* head_node, Node* tail_node) {
		this->label=in_label;
		this->set_head_node(head_node);
		this->set_tail_node(tail_node);
	};

	// Returns the label of the edge
	std::string get_label() const {return this->label;};
	/// Returns the node the edge is leading from.
	Node* get_head_node() const {return this->head_node;}
	// Returns the node the edge is leading to.
	Node* get_tail_node() const {return this->tail_node;}
	// Sets head node
	void set_head_node(Node* new_node) {this->head_node = new_node;}
	// Sets tail node
	void set_tail_node(Node* new_node) {this->tail_node = new_node;}


	private:
	std::string label = "";
	Node* head_node = nullptr; //probably, need default value
	Node* tail_node = nullptr;
	};

	class Graph
	{
	public:
	Graph() {}; // Constructor
	// Creates new node, if a node with label already exists, it returns that node.
	Node* create_node(std::string in_label);
	// Creates new edge.
	void create_edge(std::string in_label, std::string head_node_name, std::string tail_node_name);
	// Returns the node map.
	std::map<std::string, Node*> get_nodes() const {return this->nodes;} //Maybe delete, not used anywhere
	// Returns the edges map
	std::unordered_map<std::string, std::vector<Edge*> > get_edges() const {return this->edges;}

	// in method copied from directed-graph example from lecture.
    // read from *source
    void in(std::istream* source) {
       while(this->generate_graph_from(source)) {} };// read edge by edge, until generate_edge_from returns false
    // Writes to *target
    void out(std::ostream* target){print_graph(target);}
    // Prints the graph.
    void print_graph(std::ostream* target);
    int get_num_edges() const {return this->num_edges;} // Returns total number of edges in the graph
	/*

	Node* get_node_pointer_by_unique_label() (?)
    */

    std::vector<std::vector<Node*>> find_pattern(std::vector<std::string> p, std::vector<std::string> q, bool return_nodes=false); //False betyr True/False answer. True betyr finn alle noder og returner med label.

	~Graph();// Destructor
	Graph(const Graph& orig); // Copy constructor
	Graph& operator=(const Graph& rhs); // Copy assignment



	private:
	// Unordered map for edges, where label is key, and vector of edge pointers as value.
	std::unordered_map<std::string, std::vector<Edge*> > edges = std::unordered_map<std::string, std::vector<Edge*> >();
	// Map for nodes, label as key, Node pointer as value.
	std::map<std::string, Node*> nodes = std::map<std::string, Node*>();
	int num_edges = 0; // Total number of edges in the graph
	bool generate_graph_from(std::istream* source); // Implementation of this copied from directed-graph example.

    // Private functions only relevant for function "find_pattern"... INvestigate if this can be "hidden" elsewhere
    struct Parameters;
	std::vector<Edge*> analyse_graph(Parameters &params);
	std::vector<Edge*> analyse_path_edges(const bool start, Parameters &params, std::map<std::string,int> &counted_instances);
    void iterate_forward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params);
	void iterate_backward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params);
	void search_match(Node* node, std::vector<Node*> &stash, int current_index, Parameters &params);
	};

}
// I/O stream operator overloading
std::istream& operator>>(std::istream& is, graph::Graph& g);
std::ostream& operator<<(std::ostream& os, graph::Graph& g);

#endif
