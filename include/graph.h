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
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>

namespace ascii_graph {
class Graph
{
public:
        void create_vertex(char value);
        int link_two_vertices_undirected(int vertex_one, int vertex_two);
        void print_graph();
        void print_matrix();
        std::vector<char> get_shortest_path(char point_a, char point_b);
        bool empty() { return _vertices.empty(); }
private:
        std::vector<int> adjacent_vertices(int vertex);
        std::vector<int> breadth_first_search(int start_index, int goal_index);
        std::vector<char> _vertices;
        std::vector< std::vector<int> > _adj_matrix;
};
} /* namespace ascii_graph */

#endif /* __GRAPH_H__ */
