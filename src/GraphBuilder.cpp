#include <GraphBuilder.h>

#include <algorithm>

void ReducibleGraphBuilder::mutate() {

}

void ReducibleGraphBuilder::printGraph(std::ostream &Stream) {
  Stream << "digraph Dump {\n" <<
            "node[color=red,fontsize=14, style=filled]\n" <<
            &Root << " [label = \"Root\" fillcolor=green]\n" << std::endl;
  auto PrintSucsessors = [&Stream](const Node &CurNode) {
    auto SucsRange = Node::NodeBuilder{}.getSucsessors(CurNode);
    std::for_each(SucsRange.begin, SucsRange.end, 
                  [&CurNode, &Stream](const Node *NodePtr) {
                    Stream << "\"" << &CurNode << "\"" << " -> " << 
                              "\"" << NodePtr << "\"" << std::endl;
                  });
  };
  PrintSucsessors(Root);
  std::for_each(SecondaryNodes.begin(), SecondaryNodes.end(), 
                [&](const OwnedNode &CurNode) {
                  Stream << "\"" << &CurNode << "\"" << " [label = \"" <<
                  CurNode->getName() << "\" ]" << std::endl;
                  PrintSucsessors(*CurNode.get());
                });
}