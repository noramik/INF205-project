#include <iostream>
#include <fstream>
#include <cassert>

#include "graph.h"
#include "path.h"
#include <mpi.h>

int main(int argc, char** argv)
{
       // Initialize parallellization
       MPI_Init(&argc, &argv);

       // how many processes are there?
       int size = 0;
       MPI_Comm_size(MPI_COMM_WORLD, &size);

       // what is the rank of this process?
       int rank = 0;
       MPI_Comm_rank(MPI_COMM_WORLD, &rank);



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
	   std::set<std::vector<graph::Node*>> k = g.find_pattern(rank, size, p, q, true); //all ranks



	   MPI_Finalize();
	   return 0;
}
