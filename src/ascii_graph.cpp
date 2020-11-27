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
#include <unistd.h>
#include <iostream>
#include "graph.h"
#include "parser.h"

using namespace ascii_graph;

void create_dummy_graph(Graph *graph)
{
        graph->create_vertex('A');
        graph->create_vertex('B');
        graph->create_vertex('C');
        graph->create_vertex('D');
        graph->create_vertex('E');
        graph->create_vertex('F');
        graph->create_vertex('G');
        graph->create_vertex('H');
        graph->create_vertex('I');
        graph->create_vertex('J');
        graph->link_two_vertices_undirected(0, 1);
        graph->link_two_vertices_undirected(0, 2);
        graph->link_two_vertices_undirected(0, 3);
        graph->link_two_vertices_undirected(1, 3);
        graph->link_two_vertices_undirected(1, 4);
        graph->link_two_vertices_undirected(3, 4);
        graph->link_two_vertices_undirected(3, 7);
        graph->link_two_vertices_undirected(5, 7);
        graph->link_two_vertices_undirected(6, 7);
}

void print_help()
{
        std::cout << "Usage: asciigraph [options]" << std::endl << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "\t-f\t-\tRead a graph from a file with a DOT-format."
                  << std::endl;
        std::cout << "\t-d\t-\tCreate a dummy graph with sample values."
                  << std::endl;
        std::cout << "\t-m\t-\tPrint the adjancency matrix." << std::endl;
        std::cout << "\t-a\t-\tPrint the ASCII graph." << std::endl;
        std::cout << "\t-i\t-\tUse the interactive mode "
                  << "to work with a given graph." << std::endl;
        std::cout << "\t-h\t-\tPrint this text." << std::endl;
}

void interactive_loop(Graph *graph)
{
        std::string command;
        while (command != "q" && command != "end" && command != "quit") {
                std::cout << "Enter command: (l for list of commands)"
                          << std::endl;
                std::cin >> command;
                if (command == "list" || command == "l") {
                        std::cout << "shortest_path (sp)\t\t|\tprint_ascii (p)"
                                  << std::endl
                                  << "print_matrix (m)\t\t|\tlist (l)"
                                  << std::endl
                                  << "quit (q)"
                                  << std::endl;
                } else if (command == "shortest_path" || command == "sp") {
                        char from, to;
                        std::cout << "From: ";
                        std::cin >> from;
                        std::cout << "To: ";
                        std::cin >> to;
                        std::vector<char> path;
                        path = graph->get_shortest_path(from, to);
                        std::cout << "Shortest path from " << from
                                  << " to " << to << ":" << std::endl;
                        for (auto p = path.begin() ; p != path.end() ; ++p) {
                                std::cout << *p;
                                if (p != path.end()-1)
                                        std::cout << "->";
                        }
                        std::cout << std::endl;
                } else if (command == "print_ascii" || command == "p") {
                        graph->print_graph();
                } else if (command == "print_matrix" || command == "m") {
                        graph->print_matrix();
                }
        }
}

int main(int argc, char *argv[])
{
        int opt;
        Graph graph;
        DotParser parser;
        bool with_matrix, with_ascii_graph, interactive;
        with_matrix = with_ascii_graph = interactive = false;

        while ((opt = getopt(argc, argv, "f:dmaih")) != -1) {
                switch (opt) {
                case 'f':
                        parser.parse(optarg, &graph);
                        break;
                case 'd':
                        create_dummy_graph(&graph);
                        break;
                case 'm':
                        with_matrix = true;
                        break;
                case 'a':
                        with_ascii_graph = true;
                        break;
                case 'i':
                        interactive = true;
                        break;
                case 'h':
                        print_help();
                        return 0;
                case '?':
                        std::cerr << "Unknown option: "
                                  << static_cast<char>(optopt)
                                  << std::endl;
                }
        }

        if (with_matrix && !graph.empty())
                graph.print_matrix();
        if (with_ascii_graph && !graph.empty())
                graph.print_graph();
        if (interactive && !graph.empty()) {
                interactive_loop(&graph);
        }
        return 0;
}
