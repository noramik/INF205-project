/*
Random thoughts:
    Can we make clusters for the nodes and belonging edges, to prioritze areas for search?
    OR, what about weighting the edges. So instead of searching trough edges randomly, it is done by a prioritatzion...
    Doing random walks trough the graph and saving different paths, using regex or similar to look for pattern matching

Parallelization:
    One channel for each start node
    Split channels for each potential route (edge split)
    Backwards vs. forward search



find_path(graph G, path p): // p or q

    ---- WE INVESTIGATE the first edge label that has the fewest twins in the graph. Highest priority at the start of the path
    1. create unique set of all edgelabels in the path
    2. count the number of instances of the different unique edgelabels.
    2.1 OBS! If the count 1 shows up, stop counting, and choose this as start label. No need for further investigations
    3. choose the lowest count
    4. find the index of the first appearance of the label in the path array

    path_size = len(path p) // or q
    num_matches = 0; #When this is the same as path_size we have a full path match!
    array all_found_paths;

    index;    // holds the position in our paths
    label start_edge = p[index] //label

    map<label, pointer> start_map = G.get_edges[start_edge] // get all the edges (pointers) with label equal to start_edge

    ---DISTRIBUTE starting points between the ranks---------------
    if size >= size_of(start_map):
        // each rank will have it's own starting edge
        // HERE: potential for better optimization with free ranks

    length = len(start_map) //number of possible starting edges
    number of ranks base = round_down(length/size (number of ranks))
    modulus = length%size
    ....

    --- MEMORY SHARED VARIABLE
    patterns_forward; // holds found patterns where each value is {(node_label, STATUS): pointer to path stored WHERE?/maybe just the path of edge labels itself
                    // STATUS: Succesfull path (matches p or q) or unsuccessful)
                // on the heap? should be available both for p and q
    patterns_backwards; //same, but the key is the last node


    ---- FIND PATH (or error if no path)


    --IF ONLY ONE EDGE:
    ..some code

    --IF SEVERAL EDGES
    for edge in starting_map.labels:
        start_node = edge[pointer].get_tail_node //on the heap? needed later
        head_node = edge[pointer].get_head_node
        stash = [start_node, head_node] //currently. Will expand, and write to "patterns"

        if path_size > index+1: #in case we are at the end of our path
            SEARCH_FORWARD(head_node, &stash, index+1)
        else:
            SEARCH_BACKWARDS(head_node, &stash, index-1)

    return array all_found_paths

SEARCH_FORWARD(node, stash, index) //function:. Index is how far along we have come in the path p/q/or our current position

    if node.get_next_edges() # make sure we are not at end end/dangling node


        ## Search for patterns in patterns_forwards starting with our current node_label
        if node in patterns_forwards.keys:
            ## Loop? could be several patterns starting with the same node
                1.check if we match the rest of the pattern
                IF SO:
                1.1 Add to stash
                1.2 num_macthes += len(current_pattern)
                1.2.1 if num_matches = path_size: Add stash to all_found_paths
                1.2.2 else:
                1.3 check if we are at the end of the path
                1.3.1 if at the end; SEARCH_BACKWARDS(starting_node, stash, starting_index) iterate from starting point. SAVE the starting_index somehwere to make it accessible here
                1.3.2 if not; SEARCH_FORWARD(current_pattern[-1], stash, index + len(current_pattern)
                            #current_pattern[-1] the last node in the observed pattern. Where we will continue
                            #index: Move the right number of elements forward
                else: nothing, just continue the code

        for edge in node.get_next_edges():

            if edge[label] == p[index]: // If match!
                stash.push_back(edge.get_head_node())
                num_matches += 1


                if path_size > index+1 #in case we are at the end of our path
                    SEARCH_FORWARD(head_node, stash, index+1) //DEPTH first solution

                else: # Have we successed (END), or do we need to go back to the start node and iterate backwards?
                    if num_matches == path_size: #success
                        all_found_paths.append(stash)
                    else:
                        SEARCH_BACKWARDS(starting_node, stash, starting_index)

            else:
                # search for the current pattern further out in the path p:
                # stash if pattern found.
                if [node_a, node_b] in the same order in [path_p]:
                    patterns.append[(node_a, UNSUCESSFUL): pointer_to_a] #into the shared memory map
                else:
                    don't do anything? just continue the loop

        what if stash haven't grown?
        Must have different stashed for different tracks.. Cannot be by reference as now
    else: #the current node does not have any next_edges
        # search for the current pattern at the end of our path
        # if a match, add to stash, and move backwards

    RETURN what?




SEARCH_BACKWARDS(node, stash, index)









    then repeat for path q






main:
    graph G = build_graph(inputfile)

    /these three steps could also be in a function alone
    find path (Graph G, p);
    find path (Graph G, q);
    common_node_detection(graph G, path p, path q) #compare

    size = (no. of ranks)
    rank = current rank

*/
