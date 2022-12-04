#ifndef GRAPH
#define GRAPH


#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>

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


	std::string get_label() const {return this->label;} // Returns the label of the edge
    Node* get_head_node() const {return this->head_node;} // Returns the node the edge is leading from.
	Node* get_tail_node() const {return this->tail_node;}// Returns the node the edge is leading to.
	void set_head_node(Node* new_node) {this->head_node = new_node;} // Sets head node
	void set_tail_node(Node* new_node) {this->tail_node = new_node;} // Sets tail node


	private:
	std::string label = "";
	Node* head_node = nullptr;
	Node* tail_node = nullptr;
	};

	class Graph
	{
	public:
	Graph() {}; // Constructor
	// Creates new node, if a node with label already exists, it returns that node.
    Node* create_node(std::string in_label);

	void create_edge(std::string in_label, std::string tail_node_name, std::string head_node_name); // Creates new edge.
	std::map<std::string, Node*> get_nodes() const {return this->nodes;} // Returns the node map.
	std::unordered_map<std::string, std::vector<const Edge*>> get_edges() const {return this->edges;} // Returns the edges map

	// in method copied from directed-graph example from lecture.
    // read from *source
    void in(std::istream* source) {
       while(this->generate_graph_from(source)) {} };// read edge by edge, until generate_edge_from returns false
    void out(std::ostream* target){print_graph(target);} // Writes to *target
    void print_graph(std::ostream* target); // Prints the graph.
    int get_num_edges() const {return this->num_edges;} // Returns total number of edges in the graph

    std::set<std::vector<const Node*>> find_pattern (const int num_ranks, const std::vector<std::string> p, const std::vector<std::string> q, const bool return_nodes=false);

	~Graph();// Destructor
	Graph(const Graph& orig); // Copy constructor
	Graph& operator=(const Graph& rhs); // Copy assignment



	private:
	// Unordered map for edges, where label is key, and vector of edge pointers as value.
	std::unordered_map<std::string, std::vector<const Edge*>> edges = std::unordered_map<std::string, std::vector<const Edge*>>();
	// Map for nodes, label as key, Node pointer as value.
	std::map<std::string, Node*> nodes = std::map<std::string, Node*>();
	int num_edges = 0; // Total number of edges in the graph
	bool generate_graph_from(std::istream* source); // Implementation of this comes in graph.cpp comes from directed-graph example (Martin Horsch)


    // Private functions only relevant for function "find_pattern"
    // -----------------------------------------------------------
    struct Parameters{ // storage for important common values used by search algorithm in "find_pattern";

        int start_index;     //starting index in sequence. Used to orient traversals
        bool return_nodes;   // user input

        // "found_patterns" and "exit" are global pointers for all instances of the structure for each rank
        // The variables are common for all instances.
        std::set<std::vector<const Node*>>* found_patterns; //start and end nodes connected by both p and q in pairs
        bool* exit; //set to true if pattern match is found and "return_nodes" is false.\
                      Used to exit recursions efficiently.

        char path_letter; // p or q
        std::vector<std::string> p;     // first sequence from user input
        std::vector<std::string> q;     // second sequence from user input
        std::vector<std::string> path;  // current path we are traversing. Is either p or q above

        // Change path and path_letter to the opposite of what we currently have.
        // Used before the search for the second path (if the first is found) as it is unknown wether this is p or q.
        void switch_parameters() {
            this->path_letter = (this->path_letter == 'p') ? 'q' : 'p';
            this->path = (this->path_letter == 'p') ? this->p : this->q;
        }
    };


	std::vector<const Edge*> _analyse_graph(Parameters &params); // perform an analysis of the graph to provide "find_pattern" with wise starting points
    // special scenario of analysis
	std::vector<const Edge*> _analyse_path_edges(const bool start, Parameters &params, const std::map<const std::string,int> &counted_instances);
    void _iterate_forward(const Edge* &edge, std::vector<const Node*> stash, int current_index, const Parameters &params, const Edge* &start_edge,
                          std::set<std::vector<const Node*>> &found_patterns);  // recursive pattern-searching function
	void _iterate_backward(const Edge* &edge, std::vector<const Node*> stash, int current_index, const Parameters &params,
                            std::set<std::vector<const Node*>> &found_patterns); // recursive pattern-searching function
	void _search_match(const Node* node, const std::vector<const Node*> &stash, int current_index, Parameters &params,
                            std::set<std::vector<const Node*>> &found_patterns);     // recursive pattern-searching function
	};

}
// I/O stream operator overloading
std::istream& operator>>(std::istream& is, graph::Graph& g);
std::ostream& operator<<(std::ostream& os, graph::Graph& g);

#endif
