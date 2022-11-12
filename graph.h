#ifndef GRAPH
#define GRAPH


class Edge; // Edge class is defined here to avoid problems when we declare variables as Edge* in Node class.
// Implementation is further down in the file.

class Node
public:
Constructor() {} //default constructor
Constructor(std::string in_label) {this->label = in_label;) // Constructor


std::string get_label() const {return this->label;} // spørsmål: må det være const her? hvorfor?
std::vector<Edge*> get_prev_edges() const {return this->prev_edges;} // returning edges leading to the node. vector here or array?
std::vecotr<Edge*> get_next_edges() const {return this->next_edges;} // returning edges going from the node. vector here or array?
void append_prev_edges(new_edge) {this->prev_edges.push_back(new_edge);}
void append_next_edges(new_edge) {this->next_edges.push_back(new_edge);}



private:
std::string label = "";
std::vector<Edge*> prev_edges = nullptr;
std::vector<Edge*> next_edges = nullptr;
/*
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

find_pattern(path p, path q, return_nodes=False);  #False betyr True/False answer. True betyr finn alle noder og returner med label.

Destructor() // default destructor or additional functionality for handling pointers?

Copy constructor and assignement or forbid copying.
Move_constructor()?
move_assignment()?


private:
multimap? edges; //Labels as keys, pointers to edge object as value. Maybe list of pointers as values, and then use map instead of multimap.
nodes? // Do we need this
*/
