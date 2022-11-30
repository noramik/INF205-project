#include <math.h>
#include <array>
#include <vector>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <cstring>
#include <string>

#include "graph.h"

#include <mpi.h> //MPI SOLUTION ///////////////////
//#include <omp.h> // OpenMP SOLUTION ----------------


using namespace graph;





/**
 * Common variables all iterations/recursions need access to.
 * Rather than sending many variables as inputarguments for each recursion, they are stored together in a structure.
*/
struct Graph::Parameters{

    Edge* start_edge;    //traversal starts here. Updated for each starting point
    int start_index;     //starting index in sequence. Used to orient traversals
    bool return_nodes;   // user input

    // "found_patterns" and "exit" are global pointers for all instances of the structure for each rank
    // The variables are common for all instances.
    std::set<std::vector<Node*>>* found_patterns; //start and end nodes connected by both p and q in pairs
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
std::vector<Edge*> Graph::_analyse_path_edges(const bool start, Parameters &params, std::map<std::string,int> &counted_instances) {

    std::vector<Edge*> start_points; // vector to be returned

    int requirement = floor (this->get_num_edges()/3); // This can be changed as wanted

    // choose the labels corresponding to whether we investigate the start or ending nodes
    std::string p_label = (start) ? params.p[0] : params.p.back();
    std::string q_label = (start) ? params.q[0] : params.q.back();

    Node* node; // holds temporary nodes investigated for its' "next_labels"
    std::vector<Edge*> vec_p = this->get_edges().at(p_label); // all edges with label p_label
    std::vector<Edge*> vec_q = this->get_edges().at(q_label); // all edges with label q_label

    // investigate the label with the fewest instances only if requirements are fulfilled
    if (counted_instances[p_label] <= counted_instances[q_label] && counted_instances[p_label] < requirement) {
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
                for (auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }
            else { // investigating ending edges
                node = vec_p[idx]->get_head_node();
                start_index = params.path.size()-1;
                for (auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
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

    else if (counted_instances[q_label] < requirement) {
        // REMOVE: DUPLICATE
        // search for p_label in "next_edges" to node connected to edges with q_label

        params.path_letter = 'q';
        params.path = params.q;
        int start_index;

        for (int idx = 0; idx < vec_q.size(); idx++) {
            std::vector<std::string> potential_edges; //edge_labels to search for instance of both p- and q_label among \
                                                        for each iteration
            if (start) {// investigating starting edges
                Node* node = vec_q[idx]->get_tail_node();
                start_index = 0;
                for (auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }
            else {// investigating ending edges
                Node* node = vec_q[idx]->get_head_node();
                start_index = params.path.size()-1;
                for (auto &edge: node->get_next_edges()) {potential_edges.push_back(edge->get_label());}
            }

            // search for instance of q_label among potential_edges
            auto it = std::find(potential_edges.begin(), potential_edges.end(), p_label);
            if (it != potential_edges.end()) {// if found, add edge pointer from vec_q to starting_points
                start_points.push_back(vec_q[idx]);
            }
        }

        // update parameters
        params.start_index = start_index;
        if (start_points.empty()) {*params.exit = true;}
        return start_points;
        // REMOVE: DUPLICATE END
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
std::vector<Edge*> Graph::_analyse_graph(Parameters &params) {
    std::vector<Edge*> start_points;  // vector to be returned

    // Step 1
    // -------
    // Map with edge labels as keys and their number of instances in the graph as value
    std::map<std::string,int> counted_instances;

    // Extract all unique labels from input sequences p and q
    std::set<std::string> unique_labels;
    std::merge(params.p.begin(), params.p.end(),
                params.q.begin(), params.q.end(),
                std::inserter(unique_labels, unique_labels.begin()));

    // Count number of instances and add to map "counted_instances"
    for (auto edge_label: unique_labels) {

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

            //REMOVE duplicated later
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

    //REMOVE duplicate from step 1.1
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



void Graph::_iterate_forward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params) { //COPY stash and index
    /// iterate forward in the graph
    if (current_index == params.path.size()-1) {/*at the end of the path -> start iterating backwards unless the whole pattern is found ->end iterations*/
        stash.push_back(edge->get_head_node()); //at the end

        if (stash.size() == 2) {//Start searching for matching path!
            current_index = -1; //UPDATW TO THIS EVERYWHERE?
            Parameters copy_params = params; //SHALLOW copy, does this work as wanted? (we want the same poitners)
            copy_params.switch_parameters();

            _search_match(stash[0], stash, current_index, copy_params); //When this continues/returns we are done with this iteration
        }

        else {/*at the end of the path -> start iterating backwards*/
            int current_index = params.start_index; //update current_index as we are about to move backwards
            _iterate_backward(params.start_edge, stash, current_index, params);
        }
        if (*params.exit) return; //if poitner params->exit return;

    }

    else if (!edge->get_head_node()->get_next_edges().empty()) {//make sure it is not empty
        current_index++; //must handle the special case of the first iteration?
        for (Edge* next_edge: edge->get_head_node()->get_next_edges()) {

            if (next_edge->get_label() == params.path[current_index]) { //a match is found. Double check. what is index now?
                _iterate_forward(next_edge, stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};



void Graph::_iterate_backward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params) {
    //we do not go here if we started at index 0
    if (current_index == 0) {
        stash.insert(stash.begin(), edge->get_tail_node()); //order matters. stash must be (start, end)
        //Start searching for matching path!

         Parameters copy_params = params; //SHALLOW copy, does this work as wanted?
         copy_params.switch_parameters();
         current_index = -1;
         _search_match(stash[0], stash, current_index, copy_params);
         if (*params.exit) return;
    }

    else if (!edge->get_tail_node()->get_prev_edges().empty()) { // make sure it is not empty
        current_index--;

        for (Edge* edge: edge->get_tail_node()->get_prev_edges()) {
            if (edge->get_label() == params.path[current_index]) { //if match!
                _iterate_backward(edge, stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};

//STASH must contain nodes in the order: (start, end)
void Graph::_search_match(Node* node, std::vector<Node*> &stash, int current_index, Parameters &params) { //?node is the last node in the previously found pattern

    if (current_index == params.path.size()-1) {//found an entire path, but does the ending point match?
        if (node == stash.back()) { //.back() has O(1) https://www.geeksforgeeks.org/vectorfront-vectorback-c-stl/

            //#pragma omp critical // OpenMP Solution ------------------
            //{                    //OpenMP Solution  ------------------
            params.found_patterns->insert(stash);
            //}                    //OpenMP Solution  ------------------

            if (*params.exit) return;

            if (!params.return_nodes) *params.exit = true; //obs! make work with the current copy. pointer solution, does this worl?
                //..........send to rank 0? but what if only one rank...

            int rank = 0;

        }
        return;
    }

    else if (!node->get_next_edges().empty()) { //not empty
        current_index++; //somethign wrong with this one now (24.11)

        for (Edge* edge: node->get_next_edges()) {

            if (edge->get_label() == params.path[current_index]) {
                _search_match(edge->get_head_node(), stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};





//MPI solution
std::set<std::vector<Node*>> Graph::find_pattern(int rank, int num_ranks, std::vector<std::string> p, std::vector<std::string> q, bool return_nodes) { //return_nodes=false as default

//std::set<std::vector<Node*>> Graph::find_pattern(int num_ranks, std::vector<std::string> p, std::vector<std::string> q, bool return_nodes) { //return_nodes=false as default

    // initialize parameters
    Parameters params; //each rank will have its' own.. important! not shared memory

    params.return_nodes = return_nodes;
    params.p = p;
    params.q = q;

    auto starting_points = _analyse_graph(params);

    if (starting_points.empty()) {//Somehow check if starting points is empty:
        std::cout << "No such pattern exists in the graph." << std::endl;
        return *params.found_patterns;
    }


    //Make global variables for each rank
    std::set<std::vector<Node*>> found_patterns;
    bool exit = false;
    params.found_patterns = &found_patterns;
    params.exit = &exit;



    //STEP 2. Deleger startpunkter til ranks (parallellisering) --------------

    //#pragma omp parallel //OpenMP Solution -----------------------
    //{                    //OpenMP Solution -----------------------
    //int rank = omp_get_thread_num(); // OpenMP SOLUTION -------------------

    int current_index = params.start_index; //each recursion need it's own in it's scope (private label)


    //find starting points: //use formula to extract starting points
    int num_points = starting_points.size();
    int a = floor(num_points/num_ranks);
    int b = num_points%num_ranks;


    std::vector<Edge*> rank_start_points;
    if (rank < b) {
        int start_pos = rank+rank*a;
        for(auto it=starting_points.begin()+start_pos; it != starting_points.begin()+(start_pos + a+1); it++) {
            rank_start_points.push_back(*it);
        }
    }
    else {
        int start_pos = a*(rank-b)+b*(a+1);
        for (auto it=starting_points.begin()+start_pos; it != starting_points.begin()+(start_pos+a); it++) {
            rank_start_points.push_back(*it);
        }
    }


    //STEP 3. Start søk ------------------

    // Find all matching patterns of path_letter (p or q)
    for (Edge* edge : rank_start_points) {


        std::vector<Node*> stash; //will contain start and end node! Will continously be made several copies.
                              //Might CHANGE TO array of set size 2!!!
        params.start_edge = edge; //COPY
        if (params.start_index == 0) stash.push_back(edge->get_tail_node()); //if we start at the beginning //recursion will handle all else cases

        //using recursive function to iterate trough graph until patterns are found or not found
        _iterate_forward(edge, stash, current_index, params);

        if (*params.exit) break;
    }
  //} //pragma end //Open MP Solution --------------


/*// Open MP Solution ---------------------------
    //----SHOW RESULTS: found_patterns now contain either one set of nodes or several or none at all
    if (params.found_patterns->empty()) {// do not have any containment:
         std::cout << "No such pattern exists in the graph." << std::endl;;
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

        for (auto node_pairs: *params.found_patterns) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
         }
        return *params.found_patterns;
    }


} //OpenMP Solution -------------- end */




 // MPI Solution /////////////////
    // COLLECT ALL NODES ON RANK 0 (root)

    // step 1: Figure out how many nodes each rank has found. This is the number of elements each rank will send to rank 0
    //MPI_Barrier(MPI_COMM_WORLD);
    if (rank) {

        // Send number of nodes
        int num_nodes = params.found_patterns->size()*2;
        MPI_Send(&num_nodes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // for each pair in set
        for (auto it : *params.found_patterns) {
            //for each node in pair
            for (auto node : it) {

                //send number of characters
                const char* str = node->get_label().c_str();
                int str_len = strlen(str);
                MPI_Send(&str_len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

                //send label
                MPI_Send(str, str_len+1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);

            }
        }
    }

    std::set<std::vector<Node*>> all_pairs;




    // GATHERING
    if (rank == 0) {
        std::map<int, int> recv_num_nodes; //number of nodes to recieve

        std::map<int, std::vector<int>> recv_len; //length of each node-label to recieve
        int temp; //temporary storage

        std::set<std::vector<Node*>> all_pairs;
        //insert the pairs from rank 0:
        all_pairs = *params.found_patterns; //copy in


        //add num_nodes to map from each rank
        for (int _rank=1; _rank<num_ranks; _rank++) {

            // number of nodes
            MPI_Recv(&recv_num_nodes[_rank], 1, MPI_INT, _rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // length of node labels
            for (int pos = 0; pos < recv_num_nodes[_rank]; pos++) {
                    //does not send or recieve 0 if no nodes..
                MPI_Recv(&temp, 1, MPI_INT, _rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                recv_len[_rank].push_back(temp);
            }
        }


        //recieve labels
        std::vector<Node*> temp_pair;
        for (auto map_it : recv_len) {

            for (int len : map_it.second) {

                char str[len+1]; //char adds '\0' which is wy we add 1
                MPI_Recv(str, len+1, MPI_CHAR, map_it.first, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                //find node
                temp_pair.push_back(this->get_nodes()[str]); //not pair

                if (temp_pair.size() == 2) {
                    all_pairs.insert(temp_pair);
                    temp_pair.clear();
                }
            }
        }

//MPI Solution ----------------------

    //----SHOW RESULTS: found_patterns now contain either one set of nodes or several or none at all
    if (all_pairs.empty()) {
         std::cout << "No such pattern exists in the graph." << std::endl;
    }
    else if (!return_nodes) {
        std::cout << "The requested pattern is found!" << std::endl;
        std::cout << "WARNING: The returned nodes might not be the only nodes connected by these paths" <<"\n"<<
                    "Set parameter return_nodes=true to see all connections."<<std::endl;
    }
    else {
        std::cout << "The requested pattern is found! All connections found is as follows:" <<  std::endl;

        for (auto node_pairs: all_pairs) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
         }
    }

    return all_pairs;
    }

    return all_pairs; //empty for all ranks but 0

} // MPI Solution ///////////////// end



