# ASCII Graph

Basic implementation of the graph data structure, together with a parser for DOT-format files and an ASCII-representation console-printer.

## Installation

```bash
git clone https://github.com/initBasti/ascii_graph
meson build
ninja -C build
./build/src/ascii_graph -h
```

For a global installation:
```bash
ninja -C build install
ascii_graph -h
```

## Usage

`ascii_graph [options]`

#### options:
+ `-f` {/path/to/file.dot} [Insert a graph from a file containing the [DOT-format](https://www.graphviz.org/doc/info/lang.html)]
+ `-d` [Use a dummy graph to play around with the options]
+ `-m` [Print the adjacency matrix of the graph]
+ `-p` [Print the ASCII-representation of the graph]
+ `-i` [Enter interactive mode to play around with the graph]

## Motivation

This project was mainly created as a method to learn more about the Graph data structure, the C++ language, and the meson build system.
