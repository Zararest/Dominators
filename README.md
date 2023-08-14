# Dominator tree generator
This repository contains an implementation of the graph architecture.
An example of its use are graph generator and a dominator tree generator.

1. `Graph generator` makes reducible graph with 3 types of mutations: back edge addition, node spliting and path to sucsessor addition.
(GraphBuilder.h)

2. `Dominator tree generator` makes secondary dominator graph from the input graph.
(DomTreeBuilder.h)

## Examples
1. Build:
```bash
mkdir build
cd build
cmake ..
make
```
2. Demo:
```bash
./Graph 
```
Graph and its dominator tree will be generated.

3. User graph:
```bash
./Graph  --input-graph ../bin/InputExample
```
Graph and its domiantor tree wil be ganerated in the `bin` directory. In order to generate files near binary use `--no-working-dir` option.
