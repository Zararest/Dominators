#include <GraphBuilder.h>
#include <RPO.h>

#include <fstream>

int main() {
  auto DotStream = std::ofstream{"../bin/Graph.dot"};
  auto OrderStream = std::ofstream{"../bin/RPO"};
  assert(DotStream.is_open() && DotStream.is_open());
  auto Root = NamedNode::Builder{}.createNode();
  ReducibleGraphBuilder<NamedNode> Obj{*Root.get()};
  for (int i = 0; i < 30; i++) {
    Obj.mutate();
  }
  Obj.printGraph(DotStream);
  auto RPO = RPO::getOrder(*Root.get());
  for (auto &It : RPO)
    OrderStream << static_cast<NamedNode*>(It)->getName() << " | ";
  OrderStream << std::endl;
}