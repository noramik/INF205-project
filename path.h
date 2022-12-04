#ifndef PATH_H_
#define PATH_H_


#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace graph
{
/*
 * Our path class is based on query struct from lecture code - directed graph (Martin Horsch).
 */
class Path
{
public:
bool read_path(std::string* token, std::istream* source); // Based on query example from directed graph (lecture code)
void in(std::istream* source); // Based on query example from directed graph(lecture code)
std::vector<std::string> return_path_labels() const {return this->path_labels;} // returns the path_labels vector


private:
std::vector<std::string> path_labels = std::vector<std::string>(); // The path as a vector of edge_labels (strings)
};
}

std::istream& operator>>(std::istream& is, graph::Path& p);// instream operator overloader


#endif
