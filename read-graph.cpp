#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include "graph.h"
#include "path.h"
 // MPI Solution
#include <mpi.h>

//#include <omp.h>


int main(int argc, char** argv)
{

       // Initialize parallelization
       MPI_Init(&argc, &argv);

       // how many processes are there?
       int size = 0;
       MPI_Comm_size(MPI_COMM_WORLD, &size);

       // what is the rank of this process?
       int rank = 0;
       MPI_Comm_rank(MPI_COMM_WORLD, &rank);



	   assert(argc >= 2);  //Why is this 3, it throws an error, don't know why


	   //std::ifstream indata(argv[1]);

	   std::map<int, std::string> files = {
           {10, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_10.dat"},
           {50, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_50.dat"},
           {100, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_100.dat"},
           {500, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_500.dat"},
           {1000, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_1000.dat"},
           {5000, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_5000.dat"},
           {10000, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_10000.dat"},
           {50000, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_50000.dat"},
           {100000, "C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_100000.dat"}
           };

              // 10 nodes
           std::vector<std::string> p_10{"r4", "r6"};
           std::vector<std::string> q_10{"r4", "r6"};

             // 50 nodes
           std::vector<std::string> p_50{"r0", "r2", "r4", "r6"};
           std::vector<std::string> q_50{"r1", "r2", "r4", "r6"};

             // 100 nodes
           std::vector<std::string> p_100{"r1", "r2", "r4", "r6"};
           std::vector<std::string> q_100{"r1", "r3", "r5", "r6"};

            // 500 nodes
           std::vector<std::string> p_500{"r1", "r3", "r5", "r6", "r9"};
           std::vector<std::string> q_500{"r1", "r3", "r4", "r6", "r9"};

             // 1000 nodes
           std::vector<std::string> p_1000{"r1", "r2", "r4", "r6", "r9"};
           std::vector<std::string> q_1000{"r1", "r2", "r4", "r6", "r9"};

            // 5000 nodes
           std::vector<std::string> p_5000{"r1", "r2", "r4", "r7", "r9"};
           std::vector<std::string> q_5000{"r0", "r2", "r4", "r7", "r9"};

             // 10000 nodes
           std::vector<std::string> p_10000{"r0", "r3", "r4"};
           std::vector<std::string> q_10000{"r0", "r2", "r4"};

             // 50000 nodes
           std::vector<std::string> p_50000{"r0", "r3", "r4"};
           std::vector<std::string> q_50000{"r0", "r2", "r4"};

            // 100000 nodes
           std::vector<std::string> p_100000{"r0", "r3", "r4"};
           std::vector<std::string> q_100000{"r0", "r2", "r4"};

           std::map<int, std::vector<std::vector<std::string>>> sequences = {
               {10, {p_10, q_10}},
               {50, {p_50, q_50}},
               {100, {p_100, q_100}},
               {500, {p_500, q_500}},
               {1000, {p_1000, q_1000}},
               {5000, {p_5000, q_5000}},
               {10000, {p_10000, q_10000}},
               {50000, {p_50000, q_50000}},
               {100000, {p_100000, q_100000}}
           };


           std::ifstream indata("C:\\Users\\hanna\\Documents\\Programmering\\INF205\\Graph generator\\graph generator\\kb_10000.dat");
           if(!indata)
           {
              std::cerr << "Error! File " << argv[1] << " cannot be read.\n";
              return EXIT_FAILURE;
           }
           graph::Graph g;
           indata >> g;
           //std::cout << g;
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


            int num_tests = 1000;

            auto t0 = std::chrono::high_resolution_clock::now();

           for (int i=0; i<num_tests; i++) {
                std::set<std::vector<graph::Node*>> k = g.find_pattern(rank, size, p_10000, q_10000, true); //all ranks
           }

            auto t1 = std::chrono::high_resolution_clock::now();
            MPI_Finalize();
            std::cout << "TIME: " << std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count()/ (double)num_tests << "ms" << std::endl;



        /*
        std::cout <<"is empty: " <<k.empty()<<std::endl;
        for (auto node_pairs: k) {
            std::cout << "Pair: " << node_pairs[0]->get_label() << " - " << node_pairs[1]->get_label() << std::endl;
         }*/

	   return 0;

}
