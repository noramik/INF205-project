#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>
#include <vector>
#include <unordered_map>


#include "graph.h"


//using namespace graph;
using namespace std::chrono_literals;
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

void program()
{
		/*
	   assert(argc >= 2);  //Why is this 3, it throws an error, don't know why

	   std::ifstream indata(argv[1]);
	   if(!indata)
	   {
	      std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
	      //return EXIT_FAILURE;
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

		graph::Graph g;


		for (int i = 1; i < 100; i++)
		{
			std::string head = "head";
			std::string tail = "tail";
			head += std::to_string(i);
			tail += std::to_string(i);
			std::string label = "label";
			label += std::to_string(i);
			g.create_edge(label, head, tail);

		}
		graph::Graph t = g;
		std::cout << t;
}

int main()
{
	graph::Graph g;


			for (int i = 1; i < 100; i++)
			{
				std::string head = "head";
				std::string tail = "tail";
				head += std::to_string(i);
				tail += std::to_string(i);
				std::string label = "label";
				label += std::to_string(i);
				g.create_edge(label, head, tail);


			}
			//graph::Graph t;
			//t.create_edge("Edge1", "Node1", "Node2" );
			//graph::Graph t;
			//t.create_edge("Edge1", "Node1", "Node2" );
			graph::Graph t = g;
			//std::map <std::string, graph::Node*> node_map = t.get_nodes();
			std::cout << t;
			/*
			for (auto it: node_map)
			{
				std::cout << it.first << "\n";
			}


			std::unordered_map <std::string, std::vector<graph::Edge*> > edge_map = t.get_edges();
			for (auto it: edge_map)
			{
				std::cout << it.first << "\n";
			}

			std::unordered_map <std::string, std::vector<graph::Edge*> > edge_map2 = g.get_edges();
						for (auto it: edge_map2)
						{
							std::cout << it.first << "\n";
						}
			*/


	//program();
	/*
	for (int i = 1; i < 100; i++)
	{
		std::cout << i << "\n";
		std::this_thread::sleep_for(1s);
	}
	*/
}



	//std::cout << g;

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





