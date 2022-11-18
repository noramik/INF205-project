#include <math.h> // floor
#include <array>
#include <vector>

//Helper method, only used by this file?
// Alternatively. Have in a separate file potentially with namespace graph or smoething else like analyze
namespace {


    //STEP 1: ANALYSE ----------------
    /*
    1.1 Count instances
        1.1.1 Find unique edges in p and q
        1.1.2 (gå inn i edge_mappingen of hent ut lengden av listene)
                Lagre antallet i en ny midlertidig map

    1.2 Sjekk requirements

        1.2.1 if any instances == 1: Velg dette som startpunkt
        1.2.2 elif (p_1, p_n, q_1, q_m) < Et eller annet: Sjekk felles noder, bruk dette som startpunkt (evt. stopp hele koden)
        1.2.3 else velg laveste antall av forekomst i den lengste pathen */
std::tuple<const char, const int, *std::vector<Edge*>> analyse_graph(const path &p, const path &q) {
    ///Returns : which sequence pattern to initially search for with the corresponding starting points as edge-pointers

    // create map with keys as edge labels and number of instances as value
    std::map<string,int> counted_instances;
    for (/*edge_label in p+q*/) { // iterate over both loops (or create two loops)
        if (!counted_instance.count(edge_label)) { //if the key does not already exist
            int num_instances = /*length of list belonging to map edges in graph*/;

            // Special case; If we have any unique edges, we choose the first appearance and stop analysing
            if (num_instances == 1) {
                const char sequence_letter = /* find which sequence this label is in (p or q), start searching in the longest one*/;
                const int start_index = /*the index in the path for the starting point*/;
                std::vector<Edge*, 1> *starting_point = new std::vector<Edge*>();  //HEAP
                starting_point->push_back(/*this->edges[edge_label]*/);

                return std::tuple<sequence_letter, start_index, starting_point>;
            }

            // Add number of instances to the mapping
            counted_instances[edge_label] = num_instances;
        }
    }



    // Special case for starting and ending points. We do not allow this procedure if the starting/ending edge labels of p and q are equal
    //
    const int p_0 = counted_instances[p[0]];
    const int p_n = counted_instances[p.back()];
    const int q_0 = counted_instances[q[0]];
    const int q_m = counted_instances[q.back()];

    if (p[0] != q[0] && p.back() != q.back()) {
        //choose start or end based on fewest appearances. Priorotize beginning (lower or equal to)
        const int minima = std::min({p_0, p_n, q_0, q_m});

        if (minima == p_0 || minima == q_0) {
            std::tuple<const char, const int, *std::vector<Edge*>> start = analyse_path_edges(p[0], q[0]);
        }
        else std::tuple<const char, const int, *std::vector<Edge*>> start = analyse_path_edges(p.back(), q.back());

        // if start[1] != nullptr: return it. Make sure pointers is handled correctly
    }

    else if (p[0] != q[0]) {
            std::tuple<const char, const int, *std::vector<Edge*>> start = analyse_path_edges(p[0], q[0]);
            // if start[1] != nullptr: return it. Make sure pointers is handled correctly
    }
    else if (p.back() != q.back()) {
        std::tuple<const char, const int, *std::vector<Edge*>> start = analyse_path_edges(p.back(), q.back());
        // if start[1] != nullptr: return it. Make sure pointers is handled correctly
    }


    // -----General case: lowest count and longest sequence

    /// Something called VALUE_COMP() in stl.library. Use that instead
    string min_label = counted_instances[0].first
    int minima = counted_instances[0].second

    for (int idx=1; idx< counted_instances.size(); idx++) {
        if (counted_instances[idx].second < minima) {
            min_label = counted_instances[idx].first;
            minima = counted_instances[idx].second;
        }
    }
    std::vector<Edge*, 1> *starting_point = new std::vector<Edge*>();  //HEAP
    starting_points = this->edges[min_label] //can we add all at once, or do we have to iterate and add one at a time?

    // Find the longest sequence that holds the min_label
    std::vector<int>::iterator it;
    if (p.size() > q.size()) {
        it = std::find(p.begin(), p.end(), min_label);
        if (it != p.end()) char sequence_letter = "p"; /* if found */
        else char sequence_letter = "q";
    }
    else {
        it = std::find(q.begin(), q.end(), min_label);
        if (it != p.end()) char sequence_letter = "q"; /* if found */
        else char sequence_letter = "p";
    }
    return std::tuple<sequence_letter, starting_points>;

}




std::tuple<char, *std::vector<Edge*>> analyse_path_edges(string p_label, string q_label, const bool start) {
    /// We find nodes connected to both p_label and q_label if the edge with the fewest appearances is lower than a mathematical requirement
    /// p_label and q_label are both either the first or last edges in sequence p and q.
    /// place == True if at the beginning of the sequence, False if at the end

    int requirement = floor (this->tot_num_edges/3) ///Must choose this math carefully. Dummy value now

    if (counted_instances[p_label] <= counted_instances[q_label] && counted_instances[p_label] < requirement) {
        //.... check for matching nodes
        std::vector<Edge*> *starting_points = new std::vector<Edge*>(); //HEAP

        for (/* edge_pointer in this->edges[p_label]*/) {

            if start {//if True
                Node* node = edge_pointer.get_tail_node();
                std::vector<Edge*> potential_edges = node.get_next_edges();
                const int start_index = 0;
            }
            else { //if at the end
                Node* node = edge_pointer.get_head_node();
                std::vector<Edge*> potential_edges = node.get_prev_edges();
                const int start_index = /*path p.size()-1*/ //Input?
            }

            std::vector<int>::iterator it;
            it = std::find(potential_edges.begin(), potential_edges.end(), q_label);
            if (it != potential_edges.end()) {// if found
                    starting_points->push_back(edge_pointer);
            }

        }
        char sequence_letter = /*p or q: find which sequence is the largest*/;
        return std::tuple<sequence_letter, start_index, starting_points>;
    }
    else if (counted_instances[q_label]) < requirement) {
        //...check for matching nodes DUPLICATE (create another function?)
        std::vector<Edge*> *starting_points = new std::vector<Edge*>();

        for (/* edge_pointer in this->edges[q_label]*/) {
            if start {//if True
                Node* node = edge_pointer.get_tail_node();
                std::vector<Edge*> potential_edges = node.get_next_edges();
                const int start_index = 0;
            }
            else { //if at the end
                Node* node = edge_pointer.get_head_node();
                std::vector<Edge*> potential_edges = node.get_prev_edges();
                const int start_index = /*path q.size()-1*/ //Input?
            }

            std::vector<int>::iterator it;
            it = std::find(potential_edges.begin(), potential_edges.end(), p[0]);
            if (it != potential_edges.end()) {// if found
                    starting_points->push_back(edge_pointer);
            }

        }
        char sequence_letter = /* find which sequence is the largest*/;
        return std::tuple<sequence_letter, start_index, starting_points>;
        // DUPLICATE END
    }
    else { // we don't want to do anything, but need to return for consistency?
        char sequence_letter = ""; //empty
        std::vector<Edge*> *starting_points = nullptr;

        return std::tuple<"", 0, starting_points>;
    }
}


void iterate_forward(Edge* edge, std::deque<string> stash, int current_index, &var_store) { //COPY stash and index
    /// iterate forward in the graph


    if (current_index == var_store.path.size()-1) stash.push_back(edge.get_head_node()); //at the end

    //might remove entirely? change completely
    if (stash.size() == 2) {
           // var_store.found_patterns.push_back(stash);  // a whole path is found! ONLY P
                                                        //might change if stash is not a deque...
            //HERE: Start searching for matching q!
            //and return true if var_store.return_nodes = false and both p and q is found;
    }

    //do not continue to iterate if at the end of a path (fully finished path). return to the previous "recursion"
    else if (current_index == var_store.path.size()-1) {/*at the end of the path -> start iterating backwards...*/
            current_index = var_store.start_index; //update current_index as we are about to move backwards
            iterate_backward(var_store.start_edge, stash, int current_index, &var_store) } //<??start_index will become current_index

    else {

        if (edge.get_head_node().get_next_edges()) {//make sure it does not point to a nullpointer

            current_index++; //must handle the special case of the first iteration?
            for (Edge* edge: edge.get_head_node().get_next_edges()) {

                if (edge.get_label() == var_store.path[current_index]) { //a match is found. Double check. what is index now?
                    iterate_forward(edge, stash, current_index)
                }
            }
        }
    }
}


void iterate_backward(Edge* edge, std::deque<string> stash, int current_index, &var_store) {
    //we do not go here if we started at index 0

    if (current_index == 0) {
        stash.push_front(edge.get_tail_node()); //does not matter which order the nodes are in?? do not use deque...
        //var_store.found_patterns.push_back(stash); //a whole path found! ONLY p
         //HERE: Start searching for matching q!
    }

    else if (edge.get_tail_node().get_prev_edges()) { // make sure it does not point to a nullpointer
        current_index--;
        for (Edge* edge: edge.get_tail_node().get_prev_edges()) {

            if (edge.get_label() == path[current_index]) { //if match!

                //can move this:
                //if current_index == 0; /...
                //else: iterate_backward

                iterate_backward(edge, stash, current_index, &var_store);
            }

        }
    }
}

//STASH must contain nodes in the order: (start, end)
void search_match(Node* node, std::deque<string> &stash, int current_index, &var_store) { //match node is the last node in the previously found pattern

    if (current_index == var_store.path.size()-1) {//found an entire path, but does the ending point match?

        if (node == stash.back()) { //.back() has O(1) https://www.geeksforgeeks.org/vectorfront-vectorback-c-stl/
            //obs! can we compare the pointers? or do we have to compare the labels...
            var_store.found_patterns.push_back(stash);
            var_store.match=true //obs! does not work with this copy
            if (var_store.return_nodes == false) {
                //..........send to rank 0? but what if only one rank...
                //return true
                //else return false?
            }
        }
    }

    else {
        if (node.get_next_edges()) { //not a nullpointer
            current_index++;
            for (Edge* edge: node_get_next_edges()) {
                if (edge.get_label() == var_store.path[current_index]) {
                    search_match(edge.get_head_node(), &stash, current_index, &var_store);
                }
            }
        }
    }
}


// save variables we will use over and over again instead of sending them back and forth between functions. Place in namespace
struct varStorage{
    //!!innhold av found_patterns må matche med stash som currently er deque
    std::vector<std::vector<Node*>> found_patterns; //start and end nodes connected by both p and q in pairs
    const int start_index; //index from sequence of our starting point
    Edge* start_edge; //instead of node, whwere the traversing starts.

    char path_letter; // p or q
    path path; //actual sequence corresponding to p or q (the one we are searching trough atm). Could point to this->p or this->q...

    //idea draft for finding q immediatly... would make a new instance of varStorage for each q we try to find tho....
    path p;
    path q;

    void switch_parameters() {
        if (this->path_letter == "p") {
            this->path_letter = "q";
            this->path = this->q;
        }
        else {
            this->path_letter == "p";
            this->path = this->p;
        }

    }
    // would need a copy constructor to transfer the initial parameters!

    const bool return_nodes; // user input
    bool match = false; //change to true if p-q match found AND return_nodes=false. Then efficiently exit all recursion
};


} //end namespace

namespace graph{

type T find_pattern(const path p, const path q, bool return_nodes=false) {


    auto [path_letter, start_path_index, starting_points] = analyse_graph(const path &p, const path &q);
    if !starting_points; //Somehow check if starting points is empty:
        return /*No patterns found*/;


/*
    STEP 2. Deleger startpunkter til ranks (parallellisering) --------------


    */
    delete[] starting_points //IMPORTANT!!!
    /*


    STEP 3. Start søk ------------------
    */
    // Create instance of struct, fill in values and send as reference
    varStorage var_store; //each rank has it's pwn
    var_store.start_index = start_path_index;
    var_store.path_letter = path_letter;

    var_store.p = p;
    var_store.q = q;
    var_store.path = (path_letter == "p") ? p : q; //short hand for if-else https://www.w3schools.com/cpp/cpp_conditions_shorthand.asp
    var_store.return_nodes = return_nodes;




    std::deque<string> stash; //current path. Will continously be made several copies. DEQUE or OWN simple IMPLEMENTATION?
                              //stash will only contain start and end node!
                              //CHANGE TO array of set size 2!!!
    int current_index = start_path_index; //each recursion need it's own in it's scope


    //SPECIAL CASE: ONLY 1 EDGE IN PATH!

    // Find all matching patterns of path_letter (p or q)
    for (Edge* edge : starting_points) {

        var_store.start_edge = edge //COPY

        if (var_store.start_index == 0) stash.push_back(edge.get_tail_node()); //if we start at the beginning
        //recursion will handle all else cases


        //using recursive function to iterate trough graph until patterns are found or not found
        iterate_forward(edge, stash, current_index, &var_store);
    }
    //----HERE: found_patterns now contain all found patterns of either p or q


    }

}

