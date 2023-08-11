#include <GraphBuilder.h>

#include <algorithm>
#include <cassert>

size_t ReducibleGraphBuilder::getRandomValue(size_t From, size_t To) {
  assert(To > From);
  Seed ^= Seed << 13;
  Seed ^= Seed >> 7;
  Seed ^= Seed << 17;
  return From + (Seed % (To - From)); 
} 

void ReducibleGraphBuilder::splitNode(Node &NodeToSplit) {
  
}

// adds back edge only if node doesn't have one
void ReducibleGraphBuilder::addBackEdge(Node &NodeForEdge) {

}

void ReducibleGraphBuilder::addPathToSucsessor(Node &NodeForPath) {

}

void ReducibleGraphBuilder::mutate() {
  auto NodesNum = SecondaryNodes.size();
  Node &NodeToMutate = 
    (NodesNum == 0) ? Root
                  : *SecondaryNodes[getRandomValue(0, NodesNum)].get();
  constexpr auto MutationsNum = 3;
  switch (getRandomValue(0, MutationsNum)) {
    case Split:
      splitNode(NodeToMutate);
    break;
    case BackEdge:
      addBackEdge(NodeToMutate);
    break;
    case NewPath:
      addPathToSucsessor(NodeToMutate);
    break;
    default:
      assert(false && "Unreachable");
    break;
  }
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