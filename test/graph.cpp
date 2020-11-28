#include <iostream>
#include <tuple>
#include "graph.h"
#include "test.h"

using namespace ascii_graph;

void error_message(std::string msg, std::vector<char> exp, std::vector<char> act)
{
        std::cout << "Test failed: " << msg << std::endl
                  << "Expected:" << std::endl;
        for (auto& vertex:exp) {
                std::cout << vertex << ", ";
        }
        std::cout << std::endl << "Actual:" << std::endl;
        for (auto& vertex:act) {
                std::cout << vertex << ", ";
        }
        std::cout << std::endl;
}

class GraphTest : public Test
{
protected:
        int init()
        {
                /* create test graph */
                graph.create_vertex('A');
                graph.create_vertex('B');
                graph.create_vertex('C');
                graph.create_vertex('D');
                graph.create_vertex('E');
                graph.create_vertex('F');
                graph.link_two_vertices_undirected(0, 1);
                graph.link_two_vertices_undirected(0, 2);
                graph.link_two_vertices_undirected(1, 3);
                graph.link_two_vertices_undirected(2, 3);
                graph.link_two_vertices_undirected(2, 5);
                graph.link_two_vertices_undirected(3, 4);

                return TestPass;
        }

        int run()
        {
                if (graph.empty())
                        return TestFail;

                std::vector<char> result;
                std::vector<char> result_a_only {'A'};
                if ((result = graph.get_shortest_path('A', 'A'))
                    != result_a_only) {
                        error_message("shortest Path from 'A' to 'A'",
                                      result_a_only, result);
                        return TestFail;
                }

                std::vector<char> result_a_e {'A', 'B', 'D', 'E'};
                if ((result = graph.get_shortest_path('A', 'E'))
                    != result_a_e) {
                        error_message("shortest Path from 'A' to 'E'",
                                      result_a_e, result);
                        return TestFail;
                }

                std::vector<char> result_a_f {'A', 'C', 'F'};
                if ((result = graph.get_shortest_path('A', 'F'))
                    != result_a_f) {
                        error_message("shortest Path from 'A' to 'F'",
                                      result_a_f, result);
                        return TestFail;
                }

                std::vector<char> result_e_f {'E', 'D', 'C', 'F'};
                if ((result = graph.get_shortest_path('E', 'F'))
                    != result_e_f) {
                        error_message("shortest Path from 'E' to 'F'",
                                      result_a_f, result);
                        return TestFail;
                }

                return TestPass;
        }
private:
        Graph graph;
};

TEST_REGISTER(GraphTest)
