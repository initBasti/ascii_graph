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
#include "print_coordinates.h"

namespace ascii_graph {
bool sort_by_distance(std::tuple<int, int, int> edge1,
                      std::tuple<int, int, int> edge2)
{
        return (std::get<1>(edge1) - std::get<0>(edge1)) <
                (std::get<1>(edge2) - std::get<0>(edge2));
}

void PrintCoordinates::get_edges_with_min_distance()
{
        int row_index = 0;
        for (auto& row : _adj_matrix) {
                int col_index = 0;
                for (auto& col : row) {
                        if (col == 1 && col_index > row_index + 1) {
                                _edges.push_back(std::make_tuple(row_index,
                                                                 col_index,
                                                                 0));
                        }
                        col_index++;
                }
                row_index++;
        }

        /* Set the order of the edges for printing */
        sort(_edges.begin(), _edges.end(), sort_by_distance);
        int row_count = 0;
        for (auto& edge : _edges) {
                std::get<2>(edge) = row_count++;
        }
}

void PrintCoordinates::print_head()
{
        for (auto it = std::begin(_vertices); it != std::end(_vertices); ++it) {
                std::cout << *it;
                int index = it - _vertices.begin();
                if (it != _vertices.end()-1) {
                        if (_adj_matrix[index][index + 1] == 1)
                                std::cout << _connect;
                        else
                                std::cout << _space;
                }
        }
        std::cout << std::endl;
        get_edges_with_min_distance();
}

int PrintCoordinates::print_point(int row, int col)
{
        int max_col = static_cast<int>(_vertices.size());

        if (row > rows()) {
                std::cout << "invalid row " << row << "edges " << rows()
                          << std::endl;
                return 1;
        }
        if (col >= max_col) {
                std::cout << "invalid column " << col << "vertices "
                          << max_col << std::endl;
                return 1;
        }

        std::tuple<int, int, int> active_edge = get_active_edge(row);
        char c = get_char_for_point(row, col);
        std::cout << c;
        if (col == max_col - 1) {
                std::cout << std::endl;
                return 0;
        }
        if (c == '|' || c == ' ') {
                std::cout << _space;
                return 0;
        }
        if (c == '-' || c == '+') {
                std::cout << _connect;
                return 0;
        }
        if (c == 'O') {
                if (col < std::get<1>(active_edge))
                        std::cout << _connect;
                else
                        std::cout << _space;
                return 0;
        }
        return 1;
}

char PrintCoordinates::get_char_for_point(int row, int col)
{
        std::tuple<int, int, int> active_edge = get_active_edge(row);
        char character = ' ';

        /* Is point within the range of the active_edge */
        if (col >= std::get<0>(active_edge) &&
            col <= std::get<1>(active_edge)) {
                int active_edge_start = std::get<0>(active_edge);
                int active_edge_end = std::get<1>(active_edge);
                if (active_edge_start == col || active_edge_end == col)
                        return 'O';
                character = '-';
                for (auto& edge : _edges) {
                        int edge_start = std::get<0>(edge);
                        int edge_end = std::get<1>(edge);
                        int edge_row = (std::get<2>(edge) * 2) + 1;
                        /* Is there any vertical connection intersecting the
                         * active edge */
                        if (edge_row > row) {
                                if (edge_start == col || edge_end == col)
                                        character = '+';
                        }
                }
                return character;
        }
        for (auto& edge : _edges) {
                int edge_start = std::get<0>(edge);
                int edge_end = std::get<1>(edge);
                int edge_row = (std::get<2>(edge) * 2) + 1;
                if (edge_row > row) {
                        if (edge_start == col || edge_end == col)
                                character = '|';
                }
        }
        return character;
}

std::tuple<int, int, int> PrintCoordinates::get_active_edge(int row)
{
        for (auto& edge : _edges) {
                if (((std::get<2>(edge) * 2) + 1) == row) {
                        return edge;
                }
        }
        return std::make_tuple(-1, -1, -1);
}
} /* namespace ascii_graph */
