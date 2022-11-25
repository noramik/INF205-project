#include <iostream>
#include <fstream>
#include <cassert>

#include "graph.h"

int main(int argc, char** argv)
{


	   assert(argc >= 2);  //Why is this 3, it throws an error, don't know why

	   std::ifstream indata(argv[1]);
	   if(!indata)
	   {
	      std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
	      //return EXIT_FAILURE;
	   }
	   graph::Graph g;
	   indata >> g;
	   std::cout << g;

	   std::cout << "RUNNING ALGORITHM" <<std::endl;

        //SCENARIO: p and q equal!
	   std::vector<std::string> p{"is", "has"};
	   std::vector<std::string> q{"is", "is"};
	   std::set<std::vector<graph::Node*>> k = g.find_pattern(p, q, true);

}
