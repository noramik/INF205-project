#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include "graph.h"
#include "path.h"
/* // MPI Solution
#include <mpi.h>
*/
#include <omp.h>


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

     int num_tests = 1000;

	   auto t0 = std::chrono::high_resolution_clock::now();
     
	   for (int i=0; i<num_tests; i++) {
            // std::set<std::vector<graph::Node*>> k = g.find_pattern(rank, size, p, q, false); //MPI Solution
            std::set<std::vector<graph::Node*>> k = g.find_pattern(num_threads, p, q, false);
	   }
        // MPI_Finalize(); //MPI Solution
        auto t1 = std::chrono::high_resolution_clock::now();
        std::cout << "Time measurement: " << std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count()/ (double)num_tests << "ms" << std::endl;



        /*
        std::cout <<"is empty: " <<k.empty()<<std::endl;
        for (auto node_pairs: k) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
         }*/




	   return 0;

}
