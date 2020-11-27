project('ascii_graph', 'cpp',
    version : '0.0.0',
    default_options : [
        'werror=true',
        'warning_level=2',
        'cpp_std=c++11',
    ],
    license : 'GPL-3+'
)

cxx = meson.get_compiler('cpp')

c_arguments = []
cpp_arguments = []
common_arguments = [
  '-Wshadow'
]

c_arguments += common_arguments
cpp_arguments += common_arguments

add_project_arguments(c_arguments, language : 'c')
add_project_arguments(cpp_arguments, language : 'cpp')
add_project_link_arguments(cpp_arguments, language : 'cpp')

ascii_graph_includes = include_directories('include')

subdir('include')
subdir('src')
if get_option('test')
  subdir('test')
endif
