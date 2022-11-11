/*

type T find_pattern(path p, path q, bool return_nodes=False) {



    1. Analyse

        1.1 Count instances
            1.1.1 Find unique edges in p and q
            1.1.2 (gå inn i edge_mappingen of hent ut lengden av listene)
                    Lagre antallet i en ny midlertidig map

        1.2 Sjekk requirements

            1.2.1 if any instances == 1: Velg dette som startpunkt
            1.2.2 elif (p_1, p_n, q_1, q_m) < Et eller annet: Sjekk felles noder, bruk dette som startpunkt (evt. stopp hele koden)
            1.2.3 else velg laveste antall av forekomst i den lengste pathen


    2. Deleger startpunkter til ranks (parallellisering)

    3. Start søk

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


