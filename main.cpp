#include <GraphBuilder.h>
#include <Utils.h>
#include <MetadataNode.h>
#include <DomMetadata.h>

#include <fstream>

using DomNode = MetadataNode<DomMetadata>;

int main() {
  auto DotStream = std::ofstream{"../bin/Graph.dot"};
  auto OrderStream = std::ofstream{"../bin/RPO"};
  assert(DotStream.is_open() && DotStream.is_open());
  auto Root = DomNode::Builder{}.createNode();
  ReducibleGraphBuilder<DomNode> GraphBuilder{*Root.get()};
  for (int i = 0; i < 30; i++) {
    GraphBuilder.mutate();
  }
  auto [Beg, End] = GraphBuilder.getNodes();
  printGraph(Beg, End, *Root.get(), DotStream);

  auto RPO = getReversePostOrder(*Root.get());
  for (auto &It : RPO)
    OrderStream << static_cast<DomNode*>(It)->getName() << " | ";
  OrderStream << std::endl;
}