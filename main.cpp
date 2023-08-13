#include <DomTreeBuilder.h>
#include <GraphBuilder.h>
#include <Utils.h>

#include <fstream>

using NodeWithDomMeta = MetadataNode<DomMetadata>;

int main() {
  auto MainDotStream = std::ofstream{"../bin/Graph.dot"};
  auto DomDotstream = std::ofstream{"../bin/DomGraph.dot"};
  auto OrderStream = std::ofstream{"../bin/RPO"};
  assert(MainDotStream.is_open() && MainDotStream.is_open() &&
         DomDotstream.is_open());
  auto Root = NodeWithDomMeta::Builder{}.createNode();
  ReducibleGraphBuilder<NodeWithDomMeta> GraphBuilder{*Root};
  for (int i = 0; i < 10; i++) {
    GraphBuilder.mutate();
  }
  auto [Beg, End] = GraphBuilder.getMutableNodes();
  printGraph(Beg, End, *Root, MainDotStream);

  auto RPO = getReversePostOrder(*Root);
  for (auto &It : RPO)
    OrderStream << static_cast<NodeWithDomMeta *>(It)->getName() << " | ";
  OrderStream << std::endl;

  auto DomBuilder = DomTreeBuilder<MetadataNode<DomMetadata>>{};
  DomBuilder.createDomGraph(*Root, Beg, End);
  auto [DomBeg, DomEnd] = DomBuilder.getNodes();
  auto &DomRoot = DomBuilder.getDomRoot();
  printGraph(DomBeg, DomEnd, DomRoot, DomDotstream);
}