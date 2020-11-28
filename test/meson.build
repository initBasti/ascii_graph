subdir('libtest')

public_tests = [
    ['print_coordinates', 'print_coordinates.cpp'],
    ['graph', 'graph.cpp'],
    ['parser', 'parser.cpp']
]

test_includes_public += ascii_graph_includes
test_libraries += parser_lib

foreach t : public_tests
    exe = executable(t[0], t[1],
                     link_with : test_libraries,
                     include_directories : test_includes_public)

    test(t[0], exe)
endforeach
