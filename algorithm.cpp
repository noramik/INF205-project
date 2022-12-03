#include <vector>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <cstring>
#include <string>

#include "graph.h"
#include <omp.h>


using namespace graph;


/**
 * Common variables all iterations/recursions need access to.
 * Rather than sending many variables as inputarguments for each recursion, they are stored together in a structure.
*/
/*
struct Graph::Parameters{

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
};*/



/**
 * Below is three functions: _iterate_forward, _iterate_backwards and _search_match.
 * These functions represent the actual traversal troughout the graph, and are connected by recursions.
*/

/**
 * Iterate forward in the graph from current edge to search for matching edge labels at the next position in sequence p or q,
    compared to where the search is at "now".
 * The iteration is done by recursions calling itself when traversals move forward, calling _iterate_backward when end of
    sequence is reached, but entire sequence is not yet found, and calling _search_match if entire sequence of p or q is found,
    and the next step is to search for match by q or p respectively.
 *
 * @param[out] update stash with node at end of path if this point is reached.
 * @param[in] edge pointer. The current edge found as a match from the previous recursion, pointing to the node where the
                             search for new matched will continue.
 * @param[in] stash, vector of pointers to nodes.
                Works as a temporary storage of important results from the traversals, where each stack will have its' own copy.
                Stash will maximum contain two nodes, end and start node for our sequences.
                If a path is followed trough to the end, the ending node will be stashed (saved in stash).
 * @param[in] current_index. The current index in the sequence which are searched for.
                Each stack has its' own copy and current_index is only increased after moving to the next stack (level of recursion) for consistency.
 * @param[in] params, structure storing common information relevant for each recursion
 * @param[in] start_edge, the initial edge this main-iteration started from. Is used to navigate to the right edge when moving
                from _iterate_forward to _iterate_backward.
                This is unique for each rank, and cannot be saved in the shared structure "params".
 * @param[in] found_patterns, local vector (for each rank) for storing all found patterns.
*/
void Graph::_iterate_forward(const Edge* &edge, std::vector<const Node*> stash, int current_index, const Parameters &params, const Edge* &start_edge,
                             std::set<std::vector<const Node*>> &found_patterns) {
    if (current_index == params.path.size()-1) { //the end of the sequence is reached
        stash.push_back(edge->get_head_node());  //save the end node

        if (stash.size() == 2) {             // The entire path is found. Move on to search for a matching path
            current_index = -1;              // Will be increased by _search_match
            Parameters copy_params = params; // Shallow copy (keeps pointers to the same memory for all instances)
            copy_params.switch_parameters(); // switch from p to q or opposite

            //Search for p or q matching start and end position of our current found sequence.
            _search_match(stash[0], stash, current_index, copy_params, found_patterns);
        }

        else { // at end of path, but the entire sequence is not yet found
               // start iterating backwards from our initial starting point, to search for the rest of the sequence
            int current_index = params.start_index;
            _iterate_backward(start_edge, stash, current_index, params, found_patterns);
        }
        if (*params.exit) return;            // handle exit strategy. See structure Parameters for more
    }

    else if (!edge->get_head_node()->get_next_edges().empty()) {           // not a dangling node
        current_index++;
        for (const Edge* next_edge: edge->get_head_node()->get_next_edges()) {

            if (next_edge->get_label() == params.path[current_index]) {    // match found
                _iterate_forward(next_edge, stash, current_index, params, start_edge, found_patterns); // keep recursing forward
                if (*params.exit) return;    // handle exit strategy. See structure Parameters for more
            }
        }
        return;
    }
    else return;
};


/**
 * Iterate backward in the graph from current edge to search for matching edge labels at the previous unmacthed position in sequence p or q,
    compared to where the search is at "now".
 * The iteration is done by recursions calling itself when traversals move backward,
    and calling _search_match if entire sequence of p or q is found, and the next step is to search for a match by q or p respectively.
 * Function will not be called if a search started at the beginning of the sequence.
 *
 * @param[out] update stash with node at beginning of path if this point is reached.
 * @param[in] edge pointer. The current edge found as a match from the previous recursion, pointing to the node where the
                             search for new matched will continue.
 * @param[in] stash, vector of pointers to nodes.
                Works as a temporary storage of important results from the traversals, where each stack will have its' own copy.
                Stash will maximum contain two nodes, end and start node for our sequences.
                If a path is followed trough (backwards) to the beginning of the sequence, the start node will be stashed (saved in stash).
 * @param[in] current_index. The current index in the sequence which are searched for.
                Each stack has its' own copy and current_index is only decreased after moving to the next stack (level of recursion) for consistency.
 * @param[in] params, structure storing common information relevant for each recursion
 * @param[in] found_patterns, local vector (for each rank) for storing all found patterns.
*/
void Graph::_iterate_backward(const Edge* &edge, std::vector<const Node*> stash, int current_index, const Parameters &params,
                              std::set<std::vector<const Node*>> &found_patterns) {
    if (current_index == 0) {                               // reached the beginning
        stash.insert(stash.begin(), edge->get_tail_node()); //store the beginning node as the first element in stash

         //Start searching for matching path!
         Parameters copy_params = params; // Shallow copy (keeps pointers to the same memory for all instances)
         copy_params.switch_parameters(); // switch from p to q or opposite
         current_index = -1;              // Will be increased by _search_match
         //Search for p or q matching start and end position of our current found sequence.
         _search_match(stash[0], stash, current_index, copy_params, found_patterns);
         if (*params.exit) return;        // handle exit strategy. See structure Parameters for more
    }

    else if (!edge->get_tail_node()->get_prev_edges().empty()) {       // not a dangling node
        current_index--;

        for (const Edge* edge: edge->get_tail_node()->get_prev_edges()) {
            if (edge->get_label() == params.path[current_index]) {     // match found
                _iterate_backward(edge, stash, current_index, params, found_patterns); // keep recursing backward
                if (*params.exit) return; // handle exit strategy. See structure Parameters for more
            }
        }
        return;
    }
    else return;
};

/**
 * Search for sequence p or q matching start and end node of already found match q or p respectively.
 * Iterate forward in the graph starting from the first node in the already found path recursively, looking for matches on edge labels.
 * If entire sequence is found and the end node is a match, the pair of nodes (stored in "stash") is stored in the vector
    "found_patterns" common for all instances of structure Parameters.
 *
 * @param[out] update "found_patterns" with pair of nodes if matches are found.
 * @param[in] node pointer. The current node from which its' next edges are going to be investigated for matches.
 * @param[in] stash, vector of one pair of node pointers, representing what the currently investigated sequence has to match.
 * @param[in] current_index. The current index in the sequence which are searched for.
                Each stack has its' own copy and current_index is only increased after moving to the next stack (level of recursion) for consistency.
 * @param[in] params, structure storing common information relevant for each recursion.
 * @param[in] found_patterns, local vector (for each rank) for storing all found patterns.
*/
void Graph::_search_match(const Node* node, const std::vector<const Node*> &stash, int current_index, Parameters &params,
                          std::set<std::vector<const Node*>> &found_patterns) {

    if (current_index == params.path.size()-1) {// entire sequence found
        if (node == stash.back()) { // the sequence's ending location matches that of the other sequence. A match is found!

            found_patterns.insert(stash);
            if (!params.return_nodes) *params.exit = true; // handle exit strategy. See structure Parameters for more
        }
        return;
    }

    else if (!node->get_next_edges().empty()) {// not a dangling node
        current_index++;

        for (const Edge* edge: node->get_next_edges()) {
            if (edge->get_label() == params.path[current_index]) {// match found
                _search_match(edge->get_head_node(), stash, current_index, params, found_patterns); // keep recursing forward
                if (*params.exit) return; // handle exit strategy. See structure Parameters for more
            }
        }
        return;
    }
    else return;
};



/**
 * Find pair of nodes connecting two sequences p and q both by start and end, present in the graph.
 *
 * @param[out] vector of pairs of pointers to nodes, that connect p and q. Empty vector if no such matches are found.
 * @param[in] num_ranks, the number of ranks in the parallelization
 * @param[in] p, vector of edge labels. One of the two sequences we are searching for in the graph.
 * @param[in] q, vector of edge labels. One of the two sequences we are searching for in the graph.
 * @param[in] return_nodes, a boolean. False by default.
               If false, the function returns whether such a pattern exists or not in the graph, with the first matching pair of nodes.
               If true, the function keep traversing the graph until all possible matching pairs are found, and returns all pairs.

               The recursions will exit immediately if return_nodes is set to false, and a match has been found,
               using the parameter "exit" in structure Parameters, which is common for all instances of this object.
*/
std::set<std::vector<const Node*>> Graph::find_pattern (const int num_ranks, const std::vector<std::string> p, const std::vector<std::string> q, const bool return_nodes) { //return_nodes=false as default

    // STEP 1: Initialize parameter, and analyse graph to find starting edges
    // ------
    // initialize parameters and provide it with initial information
    Parameters params;
    params.return_nodes = return_nodes;
    params.p = p;
    params.q = q;

    const auto starting_points = _analyse_graph(params); // fetch the edges the traversals will start at by performing an analysis of the graph

    // The analysis might return an empty vector, indicating that no such pattern as requested can be found in the graph.
    // Terminate the code immediately.
    if (starting_points.empty()) {
        std::cout << "No such pattern exists in the graph." << std::endl;
        return *params.found_patterns;
    }

    // Create "global" variables common for all instances of the structure Parameters
    std::set<std::vector<const Node*>> found_patterns; // will collect all found patterns
    bool exit = false; // by default
    params.found_patterns = &found_patterns;
    params.exit = &exit;

    // STEP 2: Start traversing the graph in search for matches using parallelization
    // -------

    // start parallel section
    #pragma omp parallel
    {
    int current_index = params.start_index;
    std::set<std::vector<const Node*>> found_patterns;

    // Parallelize the starting_points
    // nowait removes the default barrier at the end of the for loop
    # pragma omp for nowait
    for (int i = 0; i < starting_points.size(); i++) {
        if (!*params.exit) { // handle exit strategy. See structure Parameters for more

            std::vector<const Node*> stash; // temporarily storage of start end end nodes of found paths. \
                                          each stack (level of recursion) will have its' own "stash".
            const Edge* start_edge = starting_points[i]; // temporary storage of start edge for each iteration for each rank
            if (params.start_index == 0) stash.push_back(start_edge->get_tail_node());// if we start at the beginning

            // recursive function to iterate trough graph until patterns are found or not found
            _iterate_forward(start_edge, stash, current_index, params, start_edge, found_patterns);
        }
    }

    // collect all locally found patterns in each thread in a global variable.
    // omp critical used to avoid race condition
    #pragma omp critical
    {
        for (const auto pairs : found_patterns) {
            params.found_patterns->insert(pairs);
        }
    }
    }


    // STEP 3: Print and return results
    // -------
    if (params.found_patterns->empty()) {// do not have any containment:
         std::cout << "No such pattern exists in the graph." << "MORE" << std::endl;
         return *params.found_patterns;
    }
    else if (!return_nodes) {
        std::cout << "The requested pattern is found!" << std::endl;
        std::cout << "WARNING: The returned nodes might not be the only nodes connected by these paths" <<"\n"<<
                    "Set parameter return_nodes=true to see all connections."<<std::endl;
        return *params.found_patterns;
    }
    else {
        std::cout << "The requested pattern is found! All connections found is as follows:" <<  std::endl;
        int counter = 0; //REMOVE
        for (const auto &node_pairs: *params.found_patterns) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
            counter++; //REMOVE
         }
        std::cout << "Number of found patterns is " << counter << std::endl;
        return *params.found_patterns;
    }
}





