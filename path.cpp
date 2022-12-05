#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "path.h"
using namespace graph;



/*
 * Implementation of these methods taken from query.cpp in the directed-graph example by Martin Thomas Horsch
 */

// Code from query.cpp from directed-graph (Martin Horsch)
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

// Code from query.cpp from directed-graph(lecture code) (Martin Horsch)
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


