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
#ifndef __PRINT_COORDS_H__
#define __PRINT_COORDS_H__

#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

namespace ascii_graph {
class PrintCoordinates
{
public:
        PrintCoordinates(std::vector<char> v, std::vector< std::vector<int> > m)
        {
                this->_vertices = v;
                this->_adj_matrix = m;
        }
        void print_head();
        int print_point(int row, int col);
        int rows()
        {
                return (2 * static_cast<int>(_edges.size()));
        }
private:
        char get_char_for_point(int row, int col);
        void get_edges_with_min_distance();
        std::tuple<int, int, int> get_active_edge(int row);
        std::vector<char> _vertices;
        std::vector< std::vector<int> > _adj_matrix;
        std::vector<std::tuple<int, int, int>> _edges;
        std::string _connect = "----";
        std::string _space = "    ";
};

bool sort_by_distance(std::tuple<int, int, int> edge1,
                      std::tuple<int, int, int> edge2);
// int add_special_x(int a, int b) { return a + b; }
} /* namespace ascii_graph */
#endif
