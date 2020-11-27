/*
 * Simple handling of a graph data-structure and ASCII representation printer.
 * Copyright (C) 2020 Sebastian Fricke
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <fstream>
#include "graph.h"

namespace ascii_graph {
class DotParser
{
public:
        bool parse(std::string path, Graph* graph);
        void get_content(std::vector<std::string> content)
        {
                _content = content;
        }
private:
        std::string _path;
        std::fstream _file;
        std::vector<std::string> _content;
        std::vector<std::string> _syntax_lines;
        bool open_dot_file();
        bool is_dot_format();
        void read_content();
        void get_syntax_lines();
        std::vector<char> get_vertices();
        std::vector< std::pair<int, int> > get_links(std::vector<char>);
};
} /* ascii_graph */
#endif /* __PARSER_H__ */
