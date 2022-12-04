#include <math.h>
#include <vector>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <string>

#include "graph.h"
using namespace graph;

/** Function called by _analyse_graph:
        Search for nodes containing either both start or ending edges of p and q as its' "next_edges".
        If such a node does not exist, then neither can the requested pattern.
        The search is only done if the number of instances of the edges are lower than a set requirement
    @param[out] vector of starting edges, if any.
                If *params.exit is false, an empty return vector means the requirements
                was not met, and "_analyse_graph" will continue analysis.
                If *params.exit is true, an empty vector means the pattern does not exist.
    @param[out] *params.exit is set to true if no pattern exists, to swiftly exit analysis and returns an empty vector
    @param[out] params, structure filled with general information found in the analysis (if starting points are found)
    @param[in] bool start is true if we are investigating the starting edges of p and q. If false, we investigate the ending edges.
    @param[in] params, structure holding general information
    @param[in] counted_instances, a map of edge labels and their corresponding number of instances in the graph
*/
std::vector<const Edge*> Graph::_analyse_path_edges(const bool start, Parameters &params, const std::map<const std::string,int> &counted_instances) {

    std::vector<const Edge*> start_points; // vector to be returned

    int requirement = floor (this->get_num_edges()/15); // This can be changed as wanted

    // choose the labels corresponding to whether we investigate the start or ending nodes
    const std::string p_label = (start) ? params.p[0] : params.p.back();
    const std::string q_label = (start) ? params.q[0] : params.q.back();

    Node* node; // holds temporary nodes investigated for its' "next_labels"
    const std::vector<const Edge*> vec_p = this->get_edges().at(p_label); // all edges with label p_label
    const std::vector<const Edge*> vec_q = this->get_edges().at(q_label); // all edges with label q_label

    // investigate the label with the fewest instances only if requirements are fulfilled
    if (counted_instances.at(p_label) <= counted_instances.at(q_label) && counted_instances.at(p_label) < requirement) {
        // search for q_label in "next_edges" to node connected to edges with p_label

        params.path_letter = 'p';
        params.path = params.p;
        int start_index;

        for (int idx = 0; idx < vec_p.size(); idx++) {
            std::vector<std::string> potential_edges; //edge_labels to search for instance of both p- and q_label among \
                                                        for each iteration
            if (start) { // investigating starting edges
                node = vec_p[idx]->get_tail_node();
                start_index = 0;
                for (const auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }
            else { // investigating ending edges
                node = vec_p[idx]->get_head_node();
                start_index = params.path.size()-1;
                for (const auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }

            //search for instance of q_label among potential_edges
            auto it = std::find(potential_edges.begin(), potential_edges.end(), q_label);
            if (it != potential_edges.end()) {// if found, add edge pointer from vec_p to starting_points
                start_points.push_back(vec_p[idx]);
            }
        }

        // update parameters
        params.start_index = start_index;
        if (start_points.empty()) *params.exit = true;
        return start_points;
    }

    else if (counted_instances.at(q_label) < requirement) {
        // search for p_label in "next_edges" to node connected to edges with q_label

        params.path_letter = 'q';
        params.path = params.q;
        int start_index;

        for (int idx = 0; idx < vec_q.size(); idx++) {
            std::vector<std::string> potential_edges; //edge_labels to search for instance of both p- and q_label among \
                                                        for each iteration
            if (start) {// investigating starting edges
                node = vec_q[idx]->get_tail_node();
                start_index = 0;
                for (const auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }
            else {// investigating ending edges
                node = vec_q[idx]->get_head_node();
                start_index = params.path.size()-1;
                for (const auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }

            // search for instance of q_label among potential_edges
            auto it = std::find(potential_edges.begin(), potential_edges.end(), p_label);
            if (it != potential_edges.end()) {// if found, add edge pointer from vec_q to starting_points
                start_points.push_back(vec_q[idx]);
            }
        }

        // update parameters
        params.start_index = start_index;

        if (start_points.empty()) *params.exit = true;

        return start_points;
    }

    else {// requirements not fulfilled. Return empty vector without updating parameters.

        return start_points;
    }
};



/**
 * Perform analysis of graph to find optimal starting points before traversing the graph for patterns.
 * (read: optimal - reducing starting points significantly compared to a random search)
 *
 * @param[out] vector of pointers which will operate as starting points for the pattern search traversals
 * @param[out] params, structure filled with important information for traversals
 * @param[in]  params, a structure holding some general information
*/
/*
 * General idea for analysis:
    * Fewer instances of edge labels in the graph are prioritized as starting point
    * The longer sequence of p and q are prioritized (likely to have fewer occurences in the graph)
    The fewer starting points the less traversing is neccessary.

 * The analysis consists of the following main steps:
    * 1. Count number of instances of all the unique edge labels given in sequences p and q from the user.
       If a label is not found in the graph, the code terminates with an error message.
    * 1.1 Special occurence observed: An edge label only appears once in the graph.
        This is immediately chosen as starting point and analysis returns the necessary information to start traversing.
    * 2. If the number of instances of either both start or end labels is lower than a set requirement, we search for nodes that contains both the start/ending \
          edges (in p and q) as "next_edges". If no such nodes are found, the pattern does not exist.
         Lower number of instances is prioritized for initial search.
         This procedure is not allowed for equal starting edges or equal ending edges (in p and q)
    * 3. The last step handles the general scenario.
         Edgelabels are prioritized after number of instances (fewer are valued more)
         The longest sequence with an instance of the edge label are chosen for the initial traversals.
*/
std::vector<const Edge*> Graph::_analyse_graph(Parameters &params) {
    std::vector<const Edge*> start_points;  // vector to be returned

    // Step 1
    // -------
    // Map with edge labels as keys and their number of instances in the graph as value
    std::map<const std::string, int> counted_instances;

    // Extract all unique labels from input sequences p and q
    std::set<std::string> unique_labels;
    std::merge(params.p.begin(), params.p.end(),
                params.q.begin(), params.q.end(),
                std::inserter(unique_labels, unique_labels.begin()));

    // Count number of instances and add to map "counted_instances"
    for (const auto &edge_label: unique_labels) {

        int num_instances; // temporary holds number of instances of different edge labels

        // catch error if given label does not exist in the graph
        try {num_instances = this->get_edges().at(edge_label).size();} // length of extracted list == number of instances
        catch (const std::out_of_range& oor){ //.at() will throw an out_of_range error if edge label does not occur as a key
            std::cerr << "InputError: The provided label '" << edge_label << "' does not exist in the graph." << std::endl;
            return start_points;
        }

        // Step 1.1: Choose unique occurence as starting point
        // If label occurs in both p and q, the longest sequence will be chosen
        if (num_instances == 1) {

            char path_letter; // the sequence containing the label
            int start_index; // indexposition in the path "path_letter" for the label

            // Find occurence of the label in p or q and choose starting sequence
            auto it_p = std::find(params.p.begin(), params.p.end(), edge_label);
            auto it_q = std::find(params.q.begin(), params.q.end(), edge_label);

            if (params.p.size() >= params.q.size() && it_p != params.p.end()) { // if label in the longest sequence
                path_letter = 'p';
                start_index = it_p - params.p.begin(); // Find index
            }
            else if (params.q.size() >= params.p.size() && it_q != params.q.end()) { // if label in the longest sequence
                path_letter = 'q';
                start_index = it_q - params.q.begin(); // Find index
            }
            else { // if label in the shortest sequence
                path_letter = (it_p != params.p.end()) ? 'p': 'q';
                start_index = (it_p != params.p.end()) ? (it_p - params.p.begin()) : (it_q - params.q.begin());
            }

            start_points = this->get_edges().at(edge_label);

            params.start_index = start_index;
            params.path_letter = path_letter;
            params.path = (params.path_letter == 'p') ? params.p : params.q;
            return start_points;
        }

        // Add number of instances to the mapping
        counted_instances[edge_label] = num_instances;
    }


    // Step 2
    // -------
    // If occurences of start or end labels is lower than set requirement, we search for nodes that contains both the start/ending \
       edges as "next_edges". If no such nodes are found, the pattern does not exist.
    // Lower number of instances is prioritized for initial search.
    // Procedure not allowed for equal starting edges or equal ending edges (in p and q)

    // The following part chooses the lower instances and sends this to function "_analyse_path_edges" which \
        investigates whether the requirements are met, and returns starting points if they're found.
    const int p_0 = counted_instances[params.p[0]];
    const int p_n = counted_instances[params.p.back()];
    const int q_0 = counted_instances[params.q[0]];
    const int q_m = counted_instances[params.q.back()];

    if (params.p[0] != params.q[0] && params.p.back() != params.q.back()) { // if neither start nor ending edges are equal
        const int minima = std::min({p_0, p_n, q_0, q_m}); // prioritize fewer instances for the coming search

        if (minima == p_0 || minima == q_0) start_points = _analyse_path_edges(true, params, counted_instances);
        else start_points = _analyse_path_edges(false, params, counted_instances);
        if (!start_points.empty()) return start_points; // Make sure pointers is handled correctly
        if (*params.exit) return start_points; // if analyse_path_edges left us with no possible starting points, the pattern does not exist
    }
    else if (params.p[0] != params.q[0]) {
        start_points = _analyse_path_edges(true, params, counted_instances);

        if (!start_points.empty()) return start_points;
        if (*params.exit) return start_points;
    }
    else if (params.p.back() != params.q.back()) {
        start_points = _analyse_path_edges(false, params, counted_instances);
        if (!start_points.empty()) return start_points;
        if (*params.exit) return start_points;
    }
    // if no edges was qualified for the "_analyse_path_edges", and the code did not terminate, continue with step 3.

    // Step 3
    // -------
    // Find the lowest count of the edge labels and choose the longest sequence which holds this label

    // lambda function, return pair of (label, num_instances)
    auto comp = [&](std::pair<std::string, int> a, std::pair<std::string, int> b) { //captured by reference
        return a.second < b.second;
    };
    // use lambda function "comp" to find the label with the fewest instances in the graph in p or q
    std::pair<std::string, int> minima = *min_element(counted_instances.begin(), counted_instances.end(), comp);

    start_points = this->get_edges().at(minima.first); // add all starting points with the found label

    // Find the longest sequence that holds the chosen label, and update parameters
    char path_letter; // will hold the chosen path letter ('p' or 'q')
    int start_index;      // will hold the index of the label in the chosen sequence

    // Find occurence of the label in p or q and choose starting sequence
    auto it_p = std::find(params.p.begin(), params.p.end(), minima.first);
    auto it_q = std::find(params.q.begin(), params.q.end(), minima.first);

    if (params.p.size() >= params.q.size() && it_p != params.p.end()) {// If label in the longest sequence
        path_letter = 'p';
        start_index = it_p - params.p.begin(); // Find index
    }
    else if (params.q.size() >= params.p.size() && it_q != params.q.end()) {// If label in the longest sequence
        path_letter = 'q';
        start_index = it_q - params.q.begin(); // Find index
    }
    else {// If label in the shortest sequence
        path_letter = (it_p != params.p.end()) ? 'p': 'q';
        start_index = (it_p != params.p.end()) ? (it_p - params.p.begin()) : (it_q - params.q.begin());
    }

    // update parameters
    params.start_index = start_index;
    params.path_letter = path_letter;
    params.path = (params.path_letter == 'p') ? params.p : params.q;
    return start_points;
};
