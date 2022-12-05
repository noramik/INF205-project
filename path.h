#ifndef PATH_H_
#define PATH_H_


#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace graph
{
/*
 * Our path class is based on the files query.cpp and query.h in directed-graph example.
 * This comes from lectures in INF205 and were provided to us by Martin Thomas Horsch.
 *
 */
class Path
{
public:
bool read_path(std::string* token, std::istream* source);
void in(std::istream* source);
std::vector<std::string> return_path_labels() const {return this->path_labels;} // returns the path_labels vector


private:
std::vector<std::string> path_labels = std::vector<std::string>(); // The path as a vector of edge_labels (strings)
};
}

std::istream& operator>>(std::istream& is, graph::Path& p);// instream operator overloader


#endif
