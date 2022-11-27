#include <iostream>
#include <fstream>
#include <cassert>

#include "graph.h"
#include "path.h"
#include <omp.h>

int main(int argc, char** argv)
{


	   assert(argc >= 2);  //Why is this 3, it throws an error, don't know why

        int num_threads = 1;
        num_threads = std::atoi(argv[2]);
        omp_set_num_threads(num_threads);


	   std::ifstream indata(argv[1]);
	   if(!indata)
	   {
	      std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
	      //return EXIT_FAILURE;
	   }


	   graph::Graph g;
	   indata >> g;
	   std::cout << g;
	   /* This is how a path would be read when you need it.
	   std::ifstream pathdata(argv[2]);
	   if(!pathdata)
	   {
	      std::cerr << "Error! File " << argv[2] << " cannot be read.\n";
	      //return EXIT_FAILURE;
	   }

	   graph::Path p;
	   pathdata >> p;
	   std::vector <std::string> path_lbl = p.return_path_labels();
	   for (auto it = path_lbl.begin(); it !=path_lbl.end();it++)
	   {
	   std::cout << *it << "\n";
	   }*/


	   std::cout << "RUNNING ALGORITHM" <<std::endl;

	   std::vector<std::string> p{"is", "is"};
	   std::vector<std::string> q{"is", "is"};
	   std::set<std::vector<graph::Node*>> k = g.find_pattern(num_threads, p, q, true);

}
