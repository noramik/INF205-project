#include <math.h> // floor
#include <array>

//Helper method, only used by this file?
namespace {

std::tuple<char, *std::vector<Edge*>> analyse_graph(const path &p, const path &q) {
    ///Returns : which sequence pattern to initially search for with the corresponding starting points as edge-pointers

    // create map with keys as edge labels and number of instances as value
    std::map<string,int> counted_instances;
    for (/*edge_label in p+q*/) { // iterate over both loops (or create two loops)
        if (!counted_instance.count(edge_label)) {
            int num_instances = /*length of list belonging to map edges in graph*/;

            // Special case; If we have any unique edges, we choose the first appearance and stop analysing
            if (num_instances == 1) {
                char sequence_letter = /* find which sequence this label is in (p or q)*/;
                std::vector<Edge*, 1> *starting_point = new std::vector<Edge*>();  //HEAP
                starting_point->push_back(/*this->edges[edge_label]*/);

                return std::tuple<sequence_letter, starting_point>;
            }

            // Add number of instances to the mapping
            counted_instances[edge_label] = num_instances;
        }



    // Special case for starting and ending points. We do not allow this procedure if the starting/ending edge labels of p and q are equal
    //Idea. user a function called compare_edges()?
    // start
    if (counted_instances[p[0]] != counted_instances[q[0]]) {

        //int higher = std::max(counted_instances[p.back()], counted_instances[p[0]]) //changing the order, incase

        // We find nodes connected to both p_0 and q_0 if the edge with the fewest appearances is lower than a mathematical requirement
        int requirement = floor (this->tot_num_edges/3) //Must choose this math carefully. Dummy value now

        if (counted_instances[p[0]] <= counted_instances[q[0]] && counted_instances[p[0]] < requirement) {
            //.... check for matching nodes
            std::vector<Edge*> *starting_points = new std::vector<Edge*>(); //HEAP

            for (/* edge_pointer in this->edges[p[0]]*/) {
                Node* node = edge_pointer.get_tail_node();
                std::vector<Edge*> potential_edges = node.get_next_edges();

                std::vector<int>::iterator it;
                it = std::find(potential_edges.begin(), potential_edges.end(), q[0]);
                if (it != potential_edges.end()) {// if found
                        starting_points->push_back(edge_pointer);
                }

            }
            char sequence_letter = /* find which sequence is the largest*/;
            return std::tuple<sequence_letter, starting_points>;

        }
        else if (counted_instances[q[0]) < requirement) {
            //...check for matching nodes
            std::vector<Edge*> *starting_points = new std::vector<Edge*>();

            for (/* edge_pointer in this->edges[q[0]]*/) {
                Node* node = edge_pointer.get_tail_node();
                std::vector<Edge*> potential_edges = node.get_next_edges();

                std::vector<int>::iterator it;
                it = std::find(potential_edges.begin(), potential_edges.end(), p[0]);
                if (it != potential_edges.end()) {// if found
                        starting_points->push_back(edge_pointer);
                }

            }
            char sequence_letter = /* find which sequence is the largest*/;
            return std::tuple<sequence_letter, starting_points>;

        }
        else {
            char sequence_letter = ""; //empty
            std::vector<Edge*> *starting_points = nullptr;

            return std::tuple<"", starting_points>;

    }

    }

}

bool chcek_requirement
T analyse_path_edges(string p_label, string q_label) {


}

}



type T find_pattern(const path p, const path q, bool return_nodes=False) {


    //STEP 1: ANALYSE ----------------
    auto [sequence, starting_points] = analyse_graph(const path &p, const path &q);
    if !starting_points; //Somehow check if starting points is empty:
        return /*No patterns found*/;

/*
    1.1 Count instances
        1.1.1 Find unique edges in p and q
        1.1.2 (gå inn i edge_mappingen of hent ut lengden av listene)
                Lagre antallet i en ny midlertidig map

    1.2 Sjekk requirements

        1.2.1 if any instances == 1: Velg dette som startpunkt
        1.2.2 elif (p_1, p_n, q_1, q_m) < Et eller annet: Sjekk felles noder, bruk dette som startpunkt (evt. stopp hele koden)
        1.2.3 else velg laveste antall av forekomst i den lengste pathen


    STEP 2. Deleger startpunkter til ranks (parallellisering) --------------

    STEP 3. Start søk ------------------

    stash = []; // skal lagre current path av noder her
    all_paths_found = [];
    const start_node;
    current_index;

    for edge in start_edges:
        stash.append(edge.tail_node og edge.head_node)

        iterate_forward(head_node, stash);

}

iterate_forward(node, stash);

    for edge in node.next_edges():

        if edge == match:
            current_index ++;
            stash.append(edge.head_node())

            if hele path funnet:
                all_paths_found.push_back(stash)

            if current_index == len(p)-1:
                iterate_backwards(start_node, stash)



            iterate_forward(edge.head_node, stash)



*/


