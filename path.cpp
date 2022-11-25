#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "path.h"
using namespace graph;



/* This ends up being a lot closer to version from query example (directed graph) than I intended,
 * but this seemed the only way I could figure out how to do it without relying on knowing how long the path is.
 */

// Code from query example from directed graph (lecture code)
bool Path::read_path(std::string* token, std::istream* source)
{
	char single_symbol = '\0';

	while(single_symbol != '<')
	{
	  *source >> single_symbol;
	  if(single_symbol == '\0') return false;  // format: \0 means that we are done
	}

	   for(*source >> single_symbol; single_symbol != '>'; *source >> single_symbol)
	   {
		  *token += single_symbol;
	   }

	   if(*token != "") return true;

}

// Based on query example from directed graph(lecture code)
void Path::in(std::istream* source) {
	std::string edge_lbl;
	while (read_path(&edge_lbl, source))
	{
		this->path_labels.push_back(edge_lbl);
		edge_lbl = "";
	}
}



std::istream& operator>>(std::istream& is, graph::Path& p)
{
	p.in(&is);
	return is;
}

/*
int main(int argc, char** argv)
{
// This is how you would read in two paths from one file
graph::Path p;
graph::Path q;

std::ifstream indata(argv[1]);

indata >> p;
indata >>q;


}
*/

