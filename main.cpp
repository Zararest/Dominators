#include <GraphBuilder.h>

int main() {
  auto Root = Node::Builder{}.createNode();
  ReducibleGraphBuilder<Node> Obj{Root};
  Obj.mutate();
  Obj.printGraph(std::cout);
}