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
#include <set>
#include <tuple>
#include <queue>
#include <iostream>
#include <algorithm>
#include "graph.h"
#include "print_coordinates.h"

using namespace ascii_graph;

namespace ascii_graph {
void Graph::create_vertex(char value)
{
        _vertices.push_back(value);
        _adj_matrix.resize(_vertices.size());
        for (auto& row : _adj_matrix) {
                row.resize(_vertices.size());
        }
}

int Graph::link_two_vertices_undirected(int vertex_one, int vertex_two)
{
        if (vertex_one > static_cast<int>(_vertices.size() - 1) ||
            vertex_two > static_cast<int>(_vertices.size() - 1))
                return -1;

        _adj_matrix[vertex_one][vertex_two] = 1;
        _adj_matrix[vertex_two][vertex_one] = 1;

        return 0;
}

std::vector<int> Graph::adjacent_vertices(int vertex)
{
        std::vector<int> adjacent;
        int index = 0;
        for (auto& row : _adj_matrix) {
                if (index != vertex) {
                        index++;
                        continue;
                }
                int row_index = 0;
                for (auto& column : row) {
                        if (column != 1 || row_index == vertex) {
                                row_index++;
                                continue;
                        }
                        adjacent.push_back(row_index++);
                }
                index++;
        }
        return adjacent;
}

std::vector<int> Graph::breadth_first_search(int start_index, int goal_index)
{
        int current;
        std::queue<int> queue;
        std::vector<int> visited;
        std::vector<int> origin;
        std::vector<int> path;
        if (start_index == goal_index) {
                path.push_back(start_index);
                return path;
        }

        queue.push(start_index);
        visited.push_back(start_index);
        origin.push_back(-1);

        while (!queue.empty()) {
                current = queue.front();
                if (current == goal_index)
                        break;
                queue.pop();
                for (auto& adj : this->adjacent_vertices(current)) {
                        if (std::find(visited.begin(), visited.end(), adj)
                            != visited.end())
                                continue;
                        visited.push_back(adj);
                        origin.push_back(current);
                        queue.push(adj);
                }
        }

        if (std::find(visited.begin(), visited.end(), goal_index)
            == visited.end()) {
                path.push_back(-1);
                return path;
        }

        int next = goal_index;
        while (next >= 0) {
                int index = 0;
                for (auto& visit : visited) {
                        if (visit == next) {
                                next = origin[index];
                                path.push_back(visit);
                        }
                        index++;
                }
        }
        std::reverse(path.begin(), path.end());

        return path;
}

std::vector<char> Graph::get_shortest_path(char point_a, char point_b)
{
        std::vector<char> vertex_path;
        int index = 0;
        int start = -1;
        int goal = -1;
        for (auto& vertex : _vertices) {
                if (vertex == point_a)
                        start = index;
                if (vertex == point_b)
                        goal = index;
                index++;
        }

        std::vector<int> path = breadth_first_search(start, goal);

        for (auto& path_element : path) {
                index = 0;
                for (auto& vertex : _vertices) {
                        if (index++ == path_element)
                                vertex_path.push_back(vertex);
                }
        }

        return vertex_path;
}

void Graph::print_graph()
{
        /* print the head */
        PrintCoordinates printer(_vertices, _adj_matrix);

        printer.print_head();
        for (int row = 0 ; row < printer.rows() ; row++) {
                int columns = static_cast<int>(_vertices.size());
                for (int col = 0 ; col < columns ; col++) {
                        printer.print_point(row, col);
                }
        }
}

void Graph::print_matrix()
{
        std::cout << "  | ";
        for (auto& vertex : _vertices) {
                std::cout << vertex << " ";
        }
        std::cout << std::endl;
        std::cout << "--|-";
        int columns = static_cast<int>(_vertices.size());
        for (int col = 0 ; col < columns ; col++) {
                std::cout << "--";
        }
        std::cout << std::endl;

        int row_index = 0;
        for (auto& row : _adj_matrix) {
                std::cout << _vertices[row_index++] << " | ";
                for (auto& col : row) {
                        std::cout << col << " ";
                }
                std::cout << std::endl;
        }
        std::cout << std::endl;
}
} /* namespace ascii_graph */
