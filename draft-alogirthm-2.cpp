#include <math.h> // floor
#include <array>
#include <vector>
#include <algorithm>
#include <vector>
#include <iterator>
#include <set>
#include <cstring>
#include <string>

//#include <unordered_map>

#include "graph.h"
#include <mpi.h>
using namespace graph;

//Helper method, only used by this file?
// Alternatively. Have in a separate file potentially with namespace graph or smoething else like analyze



    //STEP 1: ANALYSE ----------------
    /*
    1.1 Count instances
        1.1.1 Find unique edges in p and q
        1.1.2 (g� inn i edge_mappingen of hent ut lengden av listene)
                Lagre antallet i en ny midlertidig map

    1.2 Sjekk requirements

        1.2.1 if any instances == 1: Velg dette som startpunkt
        1.2.2 elif (p_1, p_n, q_1, q_m) < Et eller annet: Sjekk felles noder, bruk dette som startpunkt (evt. stopp hele koden)
        1.2.3 else velg laveste antall av forekomst i den lengste pathen */


// save variables we will use over and over again instead of sending them back and forth between functions. Place in namespace
struct Graph::Parameters{
    std::set<std::vector<Node*>>* found_patterns; //start and end nodes connected by both p and q in pairs
    Edge* start_edge; //instead of node, where the traversing starts.
    int start_index; //index from sequence of our starting point. BETTER A CONSTANT. but this creates errors...
    bool return_nodes; // user input. constant?
    bool* exit; //change to true if p-q match found AND return_nodes=false. Then efficiently exit all recursion. "global"

    char path_letter; // p or q
    /*
    std::string *p;
    std::string *q;
    std::string path[]; //actual sequence corresponding to p or q (the one we are searching trough atm). Could point to this->p or this->q...
    */
    std::vector<std::string> p;
    std::vector<std::string> q;
    std::vector<std::string> path;

    void switch_parameters() {
        this->path_letter = (this->path_letter == 'p') ? 'q' : 'p'; //single quotes because char and not string
        this->path = (this->path_letter == 'p') ? this->p : this->q;

        /*Same as
        if (this->path_letter == 'p') {
            this->path_letter = 'q';
            this->path = this->q;
        }
        else {
            this->path_letter == 'p';
            this->path = this->p;
        }
        */
    }
    // would need a copy constructor to transfer the initial parameters!? no, do not think so
};


std::vector<Edge*> Graph::analyse_path_edges(const bool start, Parameters &params, std::map<std::string,int> &counted_instances) {
    /// We find nodes connected to both p_label and q_label if the edge with the fewest appearances is lower than a mathematical requirement
    /// p_label and q_label are both either the first or last edges in sequence p and q.
    /// place == True if at the beginning of the sequence, False if at the end

    int requirement = floor (this->get_num_edges()/3); ///Must choose this math carefully. Dummy value now. REPLACE this->tot_num_edges
    std::string p_label = (start) ? params.p[0] : params.p.back(); //first element if at the start, otherwise the last element
    std::string q_label = (start) ? params.q[0] : params.q.back(); //first element if at the start, otherwise the last element

    std::vector<Edge*> start_points; //return vector
    //std::vector<std::string> potential_edges; //edges to search among. MOVED TO for loops
    Node* node;
    std::vector<Edge*> vec_p = this->get_edges().at(p_label); //copy?
    std::vector<Edge*> vec_q = this->get_edges().at(q_label);



    //DELETE. Did not work
    /*
    auto pointer_to_label = [&] (std::vector<Edge*>&& edge_pointers) {
        //lambda not working 100% correctly
        std::cout << "in lambda"<<std::endl;
        std::vector<std::string> potential_edges_lambda; //edges to search among
        for (auto &edge: edge_pointers) {
            potential_edges_lambda.push_back(edge->get_label());
        }
        return potential_edges_lambda;
    }; //local lambda function*/


    if (counted_instances[p_label] <= counted_instances[q_label] && counted_instances[p_label] < requirement) {
        //.... check for matching nodes

        params.path_letter = (params.p.size()>= params.q.size()) ? 'p' : 'q'; /*p or q: find which sequence is the largest*/
        params.path = (params.path_letter == 'p') ? params.p : params.q;
        int start_index; //prefer as a const, but this is difficult

        //for (Edge* edge_pointer : this->get_edges().at(p_label)) {std::cout << edge_pointer << "\t";}
        //for (auto edge_pointer = std::begin(this->get_edges().at(p_label)); edge_pointer != std::end(this->get_edges().at(p_label)); ++edge_pointer) {std::cout << *edge_pointer << "\t";}
        //for (int i = 0; i < vec.size(); i++) {std::cout << vec[i] << "\t";}


        //for (Edge* const &edge_pointer : this->get_edges().at(p_label)) { //could it be a mistake here...?
        //for (auto edge_pointer = this->get_edges().at(p_label).begin(); edge_pointer != this->get_edges().at(p_label).end(); edge_pointer++) {
        for (int i = 0; i < vec_p.size(); i++) { //for--ranged loop created bug. not sure why. also iterator...
            //for (Edge* edge_pointer : this->get_edges().at(p_label)) {std::cout << edge_pointer << "\t";}
            std::vector<std::string> potential_edges; //edges to search among
            if (start) {//if True
                std::cout << vec_p[i] << std::endl; //this is sometimes a nullpointer. Think this is fixed now
                node = vec_p[i]->get_tail_node();
                start_index = 0;
                for (auto &i: node->get_next_edges()) {potential_edges.push_back(i->get_label());}
            }
            else { //if at the end
                node = vec_p[i]->get_head_node();
                start_index = params.path.size()-1;
                for (auto &i: node->get_next_edges()) {potential_edges.push_back(i->get_label());}
            }

            auto it = std::find(potential_edges.begin(), potential_edges.end(), q_label);
            if (it != potential_edges.end()) {// if found
                start_points.push_back(vec_p[i]);
            }
        }

        params.start_index = start_index;
        if (start_points.empty()) *params.exit = true;
        return start_points;
    }

    else if (counted_instances[q_label] < requirement) {
        //...check for matching nodes DUPLICATE (create another function?)

        params.path_letter = (params.p.size()>= params.q.size()) ? 'p' : 'q'; /*p or q: find which sequence is the largest*/
        params.path = (params.path_letter == 'p') ? params.p : params.q;
        int start_index; //prefer as constant.. how to?

        //for (Edge* edge_pointer : this->get_edges().at(q_label)) {
        for (int i = 0; i < vec_q.size(); i++) {
            std::vector<std::string> potential_edges; //edges to search among
            if (start) {//if True
                Node* node = vec_q[i]->get_tail_node();
                for (auto &i: node->get_next_edges()) {potential_edges.push_back(i->get_label());}
                start_index = 0;
            }
            else { //if at the end
                Node* node = vec_q[i]->get_head_node();
                for (auto &i: node->get_next_edges()) {potential_edges.push_back(i->get_label());}
                start_index = params.path.size()-1;
            }

            auto it = std::find(potential_edges.begin(), potential_edges.end(), p_label);
            if (it != potential_edges.end()) {// if found
                start_points.push_back(vec_q[i]);
            }
        }

        params.start_index = start_index;
        if (start_points.empty()) *params.exit = true;
        return start_points;
        // DUPLICATE END
    }

    else { // we don't want to do anything, but return for consistency
        return start_points;
    }
};

std::vector<Edge*> Graph::analyse_graph(Parameters &params) {
    ///Returns : starting points. structure params will hold the rest of the information (which sequence, index ...)
    std::vector<Edge*> start_points; //return vector

    // create map with keys as edge labels and number of instances as value
    std::map<std::string,int> counted_instances;

    // Extract all unique labels. THIS PART IS NOT NECCESSARY TO REPEAT. Add as a part of graph pulic???
    std::set<std::string> unique_labels;
    std::merge(params.p.begin(), params.p.end(),
                params.q.begin(), params.q.end(),
                std::inserter(unique_labels, unique_labels.begin()));


    for (auto edge_label: unique_labels) {

        int num_instances;
        // catch error if given label does not exist in the graph
        try {
            num_instances = this->get_edges().at(edge_label).size();/*length of list belonging to map edges in graph*/; //at because we KNOW this will be here
        }
        catch (const std::out_of_range& oor){//{ //.at() will throw an out_of_range error
            std::cerr << "InputError: The provided label '"<<edge_label<< "' does not exist in the graph." << std::endl;
            //std::cerr << "Out of Range error: " << oor.what() << '\n';
            return start_points;
        }

        // Special case; If we have any unique edges, we choose the first appearance and stop the analysis
        if (num_instances == 1) {/* find which sequence this label is in (p or q), prioritize the longest one*/

            //duplicated later
            char sequence_letter; /* find which sequence this label is in (p or q), start searching in the longest one*/
            int start_index; /*the index in the path for the starting point*/ //WOULD PREFER THIS TO BE A CONSTANT

            auto it_p = std::find(params.p.begin(), params.p.end(), edge_label);
            auto it_q = std::find(params.q.begin(), params.q.end(), edge_label);

            if (params.p.size() >= params.q.size() && it_p != params.p.end()) {
                sequence_letter = 'p';
                start_index = it_p - params.p.begin(); // Code for finding index!
            }
            else if (params.q.size() >= params.p.size() && it_q != params.q.end()) {
                sequence_letter = 'q';
                start_index = it_q - params.q.begin(); // Code for finding index!
            }
            else {
                sequence_letter = (it_p != params.p.end()) ? 'p': 'q';
                start_index = (it_p != params.p.end()) ? (it_p - params.p.begin()) : (it_q - params.q.begin());
            }

            start_points = this->get_edges().at(edge_label);

            params.start_index = start_index;
            params.path_letter = sequence_letter;
            params.path = (params.path_letter == 'p') ? params.p : params.q;
            return start_points;
        }

        // Add number of instances to the mapping
        counted_instances[edge_label] = num_instances;
    }



    // Special case for starting and ending points. We do not allow this procedure if the starting/ending edge labels of p and q are equal
    //
    const int p_0 = counted_instances[params.p[0]];
    const int p_n = counted_instances[params.p.back()];
    const int q_0 = counted_instances[params.q[0]];
    const int q_m = counted_instances[params.q.back()];

    if (params.p[0] != params.q[0] && params.p.back() != params.q.back()) {
        //choose start or end based on fewest appearances. Priorotize beginning (lower or equal to)
        const int minima = std::min({p_0, p_n, q_0, q_m});

        if (minima == p_0 || minima == q_0) start_points = analyse_path_edges(true, params, counted_instances);
        else start_points = analyse_path_edges(false, params, counted_instances);
        if (!start_points.empty()) return start_points; // Make sure pointers is handled correctly

        // if analyse_path_edges left us with no possible starting points
        if (*params.exit) return start_points;
    }

    else if (params.p[0] != params.q[0]) {
        start_points = analyse_path_edges(true, params, counted_instances);
        if (!start_points.empty()) return start_points;
        if (*params.exit) return start_points;
    }
    else if (params.p.back() != params.q.back()) {
        start_points = analyse_path_edges(false, params, counted_instances);
        if (!start_points.empty()) return start_points;
        if (*params.exit) return start_points;
    }


    // -----General case: lowest count and longest sequence
    auto comp = [&](std::pair<std::string, int> a, std::pair<std::string, int> b) { //captured by reference
        return a.second < b.second;
    }; //lambda function //something wrong i think
    std::pair<std::string, int> minima = *min_element(counted_instances.begin(), counted_instances.end(), comp);
                                                      //[] (std::pair<std::string, int> a, std::pair<std::string, int> b) {return a.second < b.second;}
                                                      //); //should return pair of (label, num_instances)
    start_points = this->get_edges().at(minima.first); //can we add all at once, or do we have to iterate and add one at a time?

    // Find the longest sequence that holds the min_label + the index //DUPLICATE (expect for start above...)
    char sequence_letter; /* find which sequence this label is in (p or q), start searching in the longest one*/
    int start_index; /*the index in the path for the starting point*/ //again...better constant

    auto it_p = std::find(params.p.begin(), params.p.end(), minima.first);
    auto it_q = std::find(params.q.begin(), params.q.end(), minima.first);

    if (params.p.size() >= params.q.size() && it_p != params.p.end()) {
        sequence_letter = 'p';
        start_index = it_p - params.p.begin(); // Code for finding index!
    }
    else if (params.q.size() >= params.p.size() && it_q != params.q.end()) {
        sequence_letter = 'q';
        start_index = it_q - params.q.begin(); // Code for finding index!
    }
    else {
        sequence_letter = (it_p != params.p.end()) ? 'p': 'q';
        start_index = (it_p != params.p.end()) ? (it_p - params.p.begin()) : (it_q - params.q.begin());
    }

    params.start_index = start_index;
    params.path_letter = sequence_letter;
    params.path = (params.path_letter == 'p') ? params.p : params.q;
    return start_points;
};



void Graph::iterate_forward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params) { //COPY stash and index
    /// iterate forward in the graph
    std::cout << "Iterate forward" << std::endl;
    if (current_index == params.path.size()-1) {/*at the end of the path -> start iterating backwards unless the whole pattern is found ->end iterations*/
        stash.push_back(edge->get_head_node()); //at the end

        if (stash.size() == 2) {//Start searching for matching path!
            current_index = -1; //UPDATW TO THIS EVERYWHERE?
            Parameters copy_params = params; //SHALLOW copy, does this work as wanted? (we want the same poitners)
            copy_params.switch_parameters();

            search_match(stash[0], stash, current_index, copy_params); //When this continues/returns we are done with this iteration
        }

        else {/*at the end of the path -> start iterating backwards*/
            int current_index = params.start_index; //update current_index as we are about to move backwards
            iterate_backward(params.start_edge, stash, current_index, params);
        }
        if (*params.exit) return; //if poitner params->exit return;

    }

    else if (!edge->get_head_node()->get_next_edges().empty()) {//make sure it is not empty
        current_index++; //must handle the special case of the first iteration?
        for (Edge* next_edge: edge->get_head_node()->get_next_edges()) {

            if (next_edge->get_label() == params.path[current_index]) { //a match is found. Double check. what is index now?
                iterate_forward(next_edge, stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};



void Graph::iterate_backward(Edge* edge, std::vector<Node*> stash, int current_index, Parameters &params) {
    //we do not go here if we started at index 0
    std::cout << "Iterate backwards" << std::endl;
    if (current_index == 0) {
        stash.insert(stash.begin(), edge->get_tail_node()); //order matters. stash must be (start, end)
        //Start searching for matching path!

         Parameters copy_params = params; //SHALLOW copy, does this work as wanted?
         copy_params.switch_parameters();
         current_index = -1;
         search_match(stash[0], stash, current_index, copy_params);
         if (*params.exit) return;
    }

    else if (!edge->get_tail_node()->get_prev_edges().empty()) { // make sure it is not empty
        current_index--;

        for (Edge* edge: edge->get_tail_node()->get_prev_edges()) {
            if (edge->get_label() == params.path[current_index]) { //if match!
                iterate_backward(edge, stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};

//STASH must contain nodes in the order: (start, end)
void Graph::search_match(Node* node, std::vector<Node*> &stash, int current_index, Parameters &params) { //?node is the last node in the previously found pattern
    std::cout << "log: arrived at search_match. " << "\n";

    if (current_index == params.path.size()-1) {//found an entire path, but does the ending point match?
        if (node == stash.back()) { //.back() has O(1) https://www.geeksforgeeks.org/vectorfront-vectorback-c-stl/
            params.found_patterns->insert(stash);

            if (!params.return_nodes) *params.exit = true; //obs! make work with the current copy. pointer solution, does this worl?
                //..........send to rank 0? but what if only one rank...
        }
        return;
    }

    else if (!node->get_next_edges().empty()) { //not empty
        current_index++; //somethign wrong with this one now (24.11)

        for (Edge* edge: node->get_next_edges()) {

            if (edge->get_label() == params.path[current_index]) {
                search_match(edge->get_head_node(), stash, current_index, params);
                if (*params.exit) return;
            }
        }
        return;
    }
    else return;
};





std::set<std::vector<Node*>> Graph::find_pattern(int rank, int num_ranks, std::vector<std::string> p, std::vector<std::string> q, bool return_nodes) { //return_nodes=false as default
    std::cout << rank << std::endl;
    std::cout << "Rank above" << std::endl;

    // initialize parameters
    Parameters params; //each rank will have its' own.. important! not shared memory

    params.return_nodes = return_nodes;
    params.p = p;
    params.q = q;

    auto starting_points = analyse_graph(params);

    if (starting_points.empty()) {//Somehow check if starting points is empty:
        std::cout << "No such pattern exists in the graph." << std::endl;
        return *params.found_patterns;
    }


    //Make global variables for each rank
    std::set<std::vector<Node*>> found_patterns;
    bool exit = false;
    params.found_patterns = &found_patterns;
    params.exit = &exit;


    int current_index = params.start_index; //each recursion need it's own in it's scope


    //STEP 2. Deleger startpunkter til ranks (parallellisering) --------------
    int num_points = starting_points.size();
    int a = floor(num_points/num_ranks);
    int b = num_points%num_ranks;


    std::vector<Edge*> rank_start_points;
    if (rank < b) {
        int start_pos = rank+rank*a;
        for(auto it=starting_points.begin()+start_pos; it != starting_points.begin()+(start_pos + a+1); it++) {
            std::cout <<" EXTRA ELEMENTS" << std::endl;
            std::cout <<*it << std::endl;
            rank_start_points.push_back(*it);
        }
    }

    else {
        int start_pos = a*(rank-b)+b*(a+1);
        for (auto it=starting_points.begin()+start_pos; it != starting_points.begin()+(start_pos+a); it++) {
            std::cout <<" OG ELEMENTS" << std::endl;
            std::cout <<*it << std::endl;
            rank_start_points.push_back(*it);
        }
    }


    //STEP 3. Start s�k ------------------

    // Create instance of struct, fill in values and send as reference
    //Parameters params; //each rank has it's own


    // Find all matching patterns of path_letter (p or q)
    for (Edge* edge : rank_start_points) {


        std::vector<Node*> stash; //will contain start and end node! Will continously be made several copies.
                              //Might CHANGE TO array of set size 2!!!

        params.start_edge = edge; //COPY
        if (params.start_index == 0) stash.push_back(edge->get_tail_node()); //if we start at the beginning //recursion will handle all else cases

        //using recursive function to iterate trough graph until patterns are found or not found
        iterate_forward(edge, stash, current_index, params);
        if (*params.exit) break;
    }



    // COLLECT ALL NODES ON RANK 0 (root)

    // step 1: Figure out how many nodes each rank has found. This is the number of elements each rank will send to rank 0

    if (rank) {
        int num_nodes = params.found_patterns->size()*2;
        MPI_Send(&num_nodes, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // for each pair in set
        //for (auto it=*params.found_patterns->begin(); it != *params.found_patterns->end(); ++it) {
        for (auto it : *params.found_patterns) {
            //for each node in pair
            for (auto node : it) {

                const char* str = node->get_label().c_str();
                int str_len = strlen(str);
                MPI_Send(&str_len, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            }
        }
    }




    //TEST on one rank
    /*
    if (rank==1) {
        //std::string ONE_LABEL = (*params.found_patterns->begin())[0]->get_label();
        std::string ONE_LABEL = "ertetre her";
        std::cout << "TESTER: " << ONE_LABEL << std::endl;
        const char* str = ONE_LABEL.c_str();
        std::cout << "TESTER: " << str << std::endl;
        std::cout << "TESTER: " << strlen(str) << std::endl;
        MPI_Send(str, strlen(str)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }*/



    // GATHERING
    if (rank == 0) {
        std::map<int, int> recv_num_nodes; //number of nodes to recieve

        std::map<int, std::vector<int>> recv_len; //length of each node-label to recieve
        int temp; //temporary storage

        //add num_nodes to map from each rank
        for (int _rank=1; _rank<num_ranks; _rank++) {

            // number of nodes
            MPI_Recv(&recv_num_nodes[_rank], 1, MPI_INT, _rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // length of node labels
            for (int pos = 0; pos < recv_num_nodes[_rank]; pos++) {
                MPI_Recv(&temp, 1, MPI_INT, _rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                std::cout << "TEMP: " << temp << std::endl;
                recv_len[_rank].push_back(temp);
            }
        }

        std::cout << "----------------------------------" << std::endl;
        for (auto pair: recv_num_nodes) {std::cout<< pair.first << ", " <<pair.second << std::endl;}


        for (auto pair: recv_len) {std::cout<< pair.first << ", " <<pair.second[1] << std::endl;}




        // gathering node labels
        /*char str[4];

        MPI_Recv(str, 12, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "FINISH: "<< str << std::endl;

        for (int rank=1; rank<num_ranks; rank++) {
            for (int i=0; i<recv_num_nodes[rank]; i++) {
                std::cout << "..." << std::endl;
                //MPI_Recv()
            }
        }*/

             // REMEMBER TO ADD NODES FOUND ON RANK 0 to vector as well
    }









    //COLLECT RESULTS
    /*MPI_Gather(
        void* send_data, //start address of send buffer
        int send_count, //sizeof(array)
        MPI_CHAR, //MPI_Datatype send_datatype,
        void* recv_data, //recieve array
        int recv_count,
        MPI_CHAR, //MPI_Datatype recv_datatype,
        0, //int root
        MPI_COMM_WORLD) //MPI_Comm */
    //

    /*int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, const int *recvcounts, const int *displs,
                MPI_Datatype recvtype, int root, MPI_Comm comm)*/

    /*MPI_Gatherv(//start address of send buffer,
                1, //int sendcount
                MPI_CHAR,
                //address of recieve buffer,
                //recieve count . differs.
                //int displacement,
                MPI_CHAR,
                0,
                MPI_COMM_WORLD)*/


    //collect recieve array in pairs, and translate to nodes

    //----SHOW RESULTS: found_patterns now contain either one set of nodes or several or none at all
    if (params.found_patterns->empty()) {// do not have any containment:
         std::cout << rank<<"No such pattern exists in the graph." << std::endl;
    }
    else if (!return_nodes) {
        std::cout << rank << "The requested pattern is found!" << std::endl;
        std::cout << "WARNING: The returned nodes might not be the only nodes connected by these paths" <<"\n"<<
                    "Set parameter return_nodes=true to see all connections."<<std::endl;
    }
    else {
        std::cout << rank <<"The requested pattern is found! All connections found is as follows:" <<  std::endl;

        for (auto node_pairs: *params.found_patterns) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
         }
    }
    //MPI collect or something.....
    return *params.found_patterns;


}



