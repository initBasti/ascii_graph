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
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <boost/regex.hpp>
#include "parser.h"

using submatch_value = const boost::match_results
                        <__gnu_cxx::__normal_iterator
                                <const char*, std::__cxx11::basic_string<char> >
                        >;

static char submatch_to_char(submatch_value sm)
{
        auto val = sm;
        std::string s = val[0];
        return s.at(0);
}

static int get_vertex_index(char vertex, std::vector<char> vertices)
{
        auto it = std::find(vertices.begin(), vertices.end(), vertex);
        if (it == vertices.end())
                return -1;
        return it - vertices.begin();
}

/*
 * match any character that is preceeded by a
 * tab/space/opening curly bracket and followed by a
 * semi-colon/space/closing curly bracket
 */
static auto const char_regex = boost::regex(
        "(?<=\\t|\\s|\\{)\\w(?=\\;|\\s|\\})"
);

namespace ascii_graph {

bool DotParser::open_dot_file()
{
        if (_path.empty())
                return false;

        _file.open(_path, std::ios::in);
        if (!_file)
                return false;

        return true;
}

void DotParser::read_content()
{
        std::string line;
        if (_file.is_open()) {
                while (std::getline(_file, line)) {
                        _content.push_back(line);
                }
                _file.close();
        }
}

/**
 * is_dot_format
 * Perform basic checks to determine if the file contains a DOT-format.
 *
 * The file should start with a declaration of the type of graph:
 * (directed = digraph | undirected = graph), followed by an opening
 * curly bracket ('{'). The lines in between those brackets are terminated
 * by an semicolon (';'). And the file has to end with a matching closing
 * curly bracket ('}').
 * This method expects that the _content variable is filled by read_content
 * before a call to this method.
 */
bool DotParser::is_dot_format()
{
        if (_content.empty()) {
                std::cerr << "ERROR: Call `read_content` before checking"
                          << " the file format." << std::endl;
                return false;
        }
        /* Search patterns */
        auto const dot_header_pattern = boost::regex("(digraph|graph) \\w* {");
        auto const close_bracket = boost::regex("}");
        auto const end_semicol = boost::regex(".*;$");
        int index = 0;
        bool closed = false;
        for (auto& line : _content) {
                /* skip comments */
                if (line.rfind("//", 0) == 0)
                        continue;
                if (index++ == 0) {
                        if (!boost::regex_match(line, dot_header_pattern)) {
                                std::cerr << "ERROR: Dot-format header doesn't"
                                          << " match: " << std::endl
                                          << "got: " << line << std::endl
                                          << "expected: (digraph|graph) * {"
                                          << std::endl;
                                return false;
                        }
                } else {
                        if (boost::regex_match(line, close_bracket)) {
                                closed = true;
                                break;
                        }
                        if (!boost::regex_match(line, end_semicol)) {
                                std::cout << "DEBUG: no semicol" << std::endl;
                                std::cerr << "ERROR: Each line inbetween '{}'"
                                          << " has to end with an ';'."
                                          << std::endl
                                          << "got: " << line << std::endl;
                                return false;
                        }
                }
        }
        if (!closed) {
                std::cerr << "ERROR: File needs to end with an '}'"
                          << std::endl;
                return false;
        }
        return true;
}

/**
 * get_syntax_lines
 * Fetch all lines from the DOT-Parser inbetween the curly braces.
 */
void DotParser::get_syntax_lines()
{
        auto const start_bracket = boost::regex(".* {");
        auto const end_bracket = boost::regex("}");
        bool start_syntax = false;

        for (auto& line : _content) {
                if (!start_syntax) {
                        if (!boost::regex_match(line, start_bracket))
                                continue;
                        start_syntax = true;
                        continue;
                }
                if (boost::regex_match(line, end_bracket))
                        break;
                _syntax_lines.push_back(line);
        }
}

/**
 * get_vertices
 * Search for a unique set of vertices within the DOT-format.
 *
 * Currently only checks for characters and doesn't care about
 * any attributes like labels.
 */
std::vector<char> DotParser::get_vertices()
{
        std::vector<char> vertices;

        for (auto& line : _syntax_lines) {
                boost::sregex_iterator i(line.begin(), line.end(), char_regex);
                boost::sregex_iterator j;
                if (i == j) {
                        std::cout << "WARNING: No upper/lower-case character"
                                  << " located within: " << line << std::endl;
                }

                while (i != j) {
                        char c = submatch_to_char(*i++);
                        if (std::find(vertices.begin(), vertices.end(), c)
                            != vertices.end())
                                continue;
                        vertices.push_back(c);
                }
        }
        return vertices;
}

std::vector< std::pair<int, int> > DotParser::get_links(std::vector<char> vert)
{
        std::vector< std::pair<int, int> > links;

        auto const undir_link = boost::regex(
                "(?<=\\t\\w | \\w )--(?= \\w;| \\w )");

        for (auto& line : _syntax_lines) {
                boost::sregex_iterator v(line.begin(), line.end(), char_regex);
                boost::sregex_iterator ul(line.begin(), line.end(), undir_link);
                boost::sregex_iterator e;

                int link_count = 0;
                int index_1, index_2;
                char c;
                while (v != e) {
                        if (link_count == 0) {
                                c = submatch_to_char(*v++);
                                index_1 = get_vertex_index(c, vert);
                        } else {
                                index_1 = index_2;
                        }
                        if (ul++ != e) {
                                if (v != e) {
                                        c = submatch_to_char(*v++);
                                        index_2 = get_vertex_index(c, vert);
                                        links.push_back(
                                                std::make_pair(index_1, index_2)
                                        );
                                        link_count++;
                                }
                        }
                }
        }

        return links;
}

bool DotParser::parse(std::string path, Graph* graph)
{
        _path = path;
        if (!open_dot_file()) {
                std::cerr << "ERROR: Failed to open the file at "
                          << path << " ." << std::endl;
                return false;
        }
        read_content();

        if (!is_dot_format()) {
                std::cerr << "ERROR: File at " << _path
                          << " contains no valid DOT format."
                          << std::endl;
                _file.close();
                return false;
        }

        get_syntax_lines();

        std::vector<char> vertices = get_vertices();
        if (vertices.empty()) {
                std::cerr << "ERROR: Invalid graph nodes, use single "
                          << "characters." << std::endl;
                _file.close();
                return false;
        }

        for (auto& vertex : vertices) {
                graph->create_vertex(vertex);
        }

        std::vector< std::pair<int, int> > links = get_links(vertices);
        if (links.empty()) {
                std::cerr << "ERROR: Invalid syntax within the DOT-format at '"
                          << path << "' ." << std::endl;
                _file.close();
                return false;
        }

        for (auto& link : links) {
                graph->link_two_vertices_undirected(link.first, link.second);
        }

        _file.close();
        return true;
}
} /* namespace ascii_graph */
