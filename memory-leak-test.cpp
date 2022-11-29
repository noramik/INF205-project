#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "graph.h"

using namespace std::chrono_literals;

void create_graph()
{
		graph::Graph g;
		for (int i = 1; i < 1000000; i++)
		{
			std::string head = "node";
			std::string tail = "node";
			head += std::to_string(i);
			tail += std::to_string(i+1);
			std::string label = "label";
			label += std::to_string(i);
			g.create_edge(label, head, tail);
		}
}

int main()
{
	create_graph();
	for (int i = 1; i < 100; i++)
	{
		std::cout << i << "\n";
		std::this_thread::sleep_for(1s);
	}
}
