#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include "parser.h"
#include "test.h"

using namespace ascii_graph;

class ParserTest : public Test
{
protected:
        int init()
        {
                /* create a good test graph */
                std::vector<std::string> good_example;
                good_example.push_back("// test comment\n");
                good_example.push_back("graph test {\n");
                good_example.push_back("        a -- b -- c;\n");
                good_example.push_back("        b -- c;\n");
                good_example.push_back("}\n");

                good_file = "/tmp/ascii_graph.test.XXXXXX";
                int fd = mkstemp(&good_file.front());
                if (fd < 0) {
                        std::cout << "Test failed: creation of temp."
                                  << " file failed, fd = " << fd
                                  << std::endl;
                        return TestFail;
                }
                if (setup_file(good_file, good_example) != TestPass)
                        return TestFail;

                /* create a bad test graph */
                std::vector<std::string> bad_example;
                bad_example.push_back("graph test {\n");
                bad_example.push_back("        a -- b --;\n");
                bad_example.push_back("        b -- c\n");
                bad_example.push_back("}\n");

                bad_file = "/tmp/ascii_graph.test_2.XXXXXX";
                fd = mkstemp(&bad_file.front());
                if (fd < 0) {
                        std::cout << "Test failed: creation of temp."
                                  << " file failed, fd = " << fd
                                  << std::endl;
                        return TestFail;
                }
                if (setup_file(bad_file, bad_example) != TestPass)
                        return TestFail;

                return TestPass;
        }

        int run()
        {
                if (!parser.parse(good_file, &graph)) {
                        std::cout << "Test failed: Parse of file at "
                                  << good_file << " was not successful."
                                  << std::endl;
                        return TestFail;
                }

                reset_objects();

                if (parser.parse(bad_file, &graph)) {
                        std::cout << "Test failed: Parse of file at "
                                  << bad_file << " should not succeed."
                                  << std::endl;
                        return TestFail;
                }
                return TestPass;
        }

        int setup_file(std::string name, std::vector<std::string> content)
        {
                std::ofstream myfile(name);
                if (!myfile.is_open()) {
                        std::cout << "Test failed: temp. file is not open"
                                  << std::endl;
                        return TestFail;
                }

                for (auto& line:content) {
                        myfile << line;
                }
                myfile.close();

                return TestPass;
        }

        void reset_objects()
        {
                (&parser)->~DotParser();
                new (&parser) DotParser();
                (&graph)->~Graph();
                new (&graph) Graph();
        }

        void cleanup()
        {
                unlink(good_file.c_str());
                unlink(bad_file.c_str());
        }
private:
        DotParser parser;
        Graph graph;
        std::vector<std::string> _contents;
        std::string good_file;
        std::string bad_file;
};

TEST_REGISTER(ParserTest)
