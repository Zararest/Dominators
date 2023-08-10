#include <GraphBuilder.h>

int main() {
  auto Root = Node::NodeBuilder{}.createNode();
  ReducibleGraphBuilder Obj{Root};
  Obj.mutate();
}