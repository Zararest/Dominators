#include <GraphBuilder.h>
#include <Utils.h>
#include <DomTreeBuilder.h>

#include <fstream>

using DomNode = MetadataNode<DomMetadata>;

int main() {
  auto DotStream = std::ofstream{"../bin/Graph.dot"};
  auto OrderStream = std::ofstream{"../bin/RPO"};
  assert(DotStream.is_open() && DotStream.is_open());
  auto Root = DomNode::Builder{}.createNode();
  ReducibleGraphBuilder<DomNode> GraphBuilder{*Root};
  for (int i = 0; i < 30; i++) {
    GraphBuilder.mutate();
  }
  auto [Beg, End] = GraphBuilder.getMutableNodes();
  printGraph(Beg, End, *Root, DotStream);

  auto RPO = getReversePostOrder(*Root);
  for (auto &It : RPO)
    OrderStream << static_cast<DomNode*>(It)->getName() << " | ";
  OrderStream << std::endl;

  auto DomBuilder = DomTreeBuilder<MetadataNode<DomMetadata>>{};
  DomBuilder.calcDominators(*Root, Beg, End);
}