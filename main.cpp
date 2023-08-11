#include <GraphBuilder.h>

int main() {
  auto Root = Node::Builder{}.createNode();
  ReducibleGraphBuilder<NamedNode> Obj{*Root.get()};
  for (int i = 0; i < 100; i++) {
    Obj.mutate();
  }
  Obj.printGraph(std::cout);
}