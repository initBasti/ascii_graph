libboost = dependency('boost', modules : ['regex'], required : true)

ascii_graph_deps = [
  libboost,
]

print_coord_lib = static_library('print_coord', 'print_coordinates.cpp',
                                 include_directories: ascii_graph_includes)
graph_lib = static_library('graph', 'graph.cpp',
                           link_with: print_coord_lib,
                           include_directories: ascii_graph_includes)
parser_lib = static_library('parser', 'parser.cpp',
                            link_with: graph_lib,
                            include_directories: ascii_graph_includes,
                            dependencies: libboost)

executable('ascii_graph',
           'ascii_graph.cpp',
           link_with: parser_lib,
           include_directories: ascii_graph_includes,
           dependencies: ascii_graph_deps,
           install : true)
