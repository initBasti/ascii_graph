#include <tuple>
#include "print_coordinates.h"
#include "test.h"

using namespace ascii_graph;

class PrintCoordTest : public Test
{
protected:
        int run()
        {
                if (sort_by_distance(
                    std::make_tuple(1, 2, 0),
                    std::make_tuple(1, 3, 0)) == false)
                        return TestFail;
                return TestPass;
        }
};

TEST_REGISTER(PrintCoordTest)
