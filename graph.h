#ifndef GRAPH
#define GRAPH


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

void get_edges() //get the multimap

Destructor() // default destructor or additional functionality for handling pointers?

Copy constructor and assignement or forbid copying.
Move_constructor()?
move_assignment()?


private:
multimap? edges; //Labels as keys, pointers to edge object as value
nodes? // Do we need this?
