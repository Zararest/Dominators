#include <GraphBuilder.h>

int main() {
  auto Root = Node::Builder{}.createNode();
  ReducibleGraphBuilder<NamedNode> Obj{*Root.get()};
  Obj.mutate();
  Obj.printGraph(std::cout);
}