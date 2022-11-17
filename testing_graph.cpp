//#include <iostream>
//#include <string>


#include "graph.h"
#include "query.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
using namespace graph;
/*
int main()
{
	graph::Graph* g = new graph::Graph;
	std::string in_label = "Cat";
	g->create_node(in_label);
	std::string new_label = "Dog";
	g->create_node(new_label);
	std::map<std::string, graph::Node*> nodes = g->get_nodes();
	for (const auto& x: nodes)
	{
		std::cout << x.first << "\t" << x.second->get_label() << std::endl;
	}
	std::string edge_label = "Hates";
	g->create_edge(edge_label, in_label, new_label);
	g->create_edge(edge_label, new_label, in_label);
	std::unordered_map<std::string, std::vector<Edge*> > edges = g->get_edges();


	for (const auto& x: edges)
	{
	std::cout << x.first << "\t" << x.second.size() << "\n";
	}
	delete g; // Seems like there is something wrong with the destructor. When I comment out this line everything works fine
	// but when it isn't commented, I get an error message when trying to compile.

}
*/

int main(int argc, char** argv)
{
   assert(argc >= 2);  //Why is this 3, it throws an error, don't know why

   std::ifstream indata(argv[1]);
   if(!indata)
   {
      std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
      return EXIT_FAILURE;
   }
   graph::Graph g;
   indata >> g;
   /*
   std::map<std::string, graph::Node*> nodes = g.get_nodes();
	for (const auto& x: nodes)
	{
		std::cout << x.first << "\t"  << std::endl;
	}
	*/
	std::cout << g;

	/*
   std::unordered_map<std::string, std::vector<Edge*> > edges = g.get_edges();
   for (const auto& x: edges)
   	{
   		std::cout << x.first << "\t"  << std::endl;
   	}
	indata.close();
   // std::cout << "\nContent of graph g:\n" << g << "\n";

   /*
   std::ifstream inquery(argv[2]);
   if(!inquery)
   {
      std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
      return EXIT_FAILURE;
   }
   graph::Query q;
   inquery >> q;
   inquery.close();

   g.query(&q, &std::cout);  // apply query q to graph g
   */
}




