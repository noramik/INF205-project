#ifndef PATH_H_
#define PATH_H_


#include <fstream>
#include <string>
#include <vector>
#include <iostream>

namespace graph
{
class Path
{
public:
/* This ends up being a lot closer to version from query example (directed graph) than I intended,
 * but this seemed the only way I could figure out how to do it without relying on knowing how long the path is.
 */

// Based on query example from directed graph (lecture code)
bool read_path(std::string* token, std::istream* source);

// Based on query example from directed graph(lecture code)
void in(std::istream* source);

std::vector<std::string> return_path_labels() const {return this->path_labels;}

private:
std::vector<std::string> path_labels = std::vector<std::string>();

};
}

std::istream& operator>>(std::istream& is, graph::Path& p);


#endif /* PATH_H_ */
