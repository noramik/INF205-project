#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include "graph.h"
#include "path.h"
#include <omp.h> // OpenMP Solution -------------------

// Run with OpenMP: compiled_file.exe C:\path\to\graph.dat NUMBER_OF_NODES


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

       int num_threads = 1;
       num_threads = std::atoi(argv[2]);
       omp_set_num_threads(num_threads);

	   std::cout << "RUNNING ALGORITHM" <<std::endl;
	   const std::vector<std::string> p{"r0", "r2", "r4", "r6"};
	   const std::vector<std::string> q{"r1", "r2", "r4", "r6"};

       int num_tests = 1;

	   auto t0 = std::chrono::high_resolution_clock::now();

	   for (int i=0; i<num_tests; i++) {
            std::set<std::vector<const graph::Node*>> k = g.find_pattern(num_threads, p, q, true);
            std::cout << "IN LOOP" << std::endl;
	   }

        auto t1 = std::chrono::high_resolution_clock::now();
        std::cout << "Time measurement: " << std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count()/ (double)num_tests << "ms" << std::endl;


	   return 0;

}
