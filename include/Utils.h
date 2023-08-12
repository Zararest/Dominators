#pragma once

#include <NamedNode.h>

std::vector<Node*> getReversePostOrder(Node &Root);

// Works with unique ptrs
template <typename It, typename RootT>
void printGraph(It NodesBeg, It NodesEnd, RootT &Root, std::ostream &Stream) {
  Stream << "digraph Dump {\n" <<
            "node[color=red,fontsize=14, style=filled]\n" <<
            "\"" << &Root << "\"" << " [label = \"Root\" fillcolor=green]\n";
  auto PrintSucsessors = [&Stream](const Node &CurNode) {
    auto SucsRange = Node::Builder{}.getSucsessors(CurNode);
    std::for_each(SucsRange.begin, SucsRange.end, 
                  [&CurNode, &Stream](const Node *NodePtr) {
                    Stream << "\"" << &CurNode << "\"" << " -> " << 
                              "\"" << NodePtr << "\"" << std::endl;
                  });
  };
  PrintSucsessors(Root);
  std::for_each(NodesBeg, NodesEnd, 
                [&](auto &CurNode) {
                  if constexpr(std::is_base_of_v<NamedNode, 
                              typename std::iterator_traits<It>
                                ::value_type::element_type>) {
                    Stream << "\"" << CurNode.get() << "\"" << " [label = \"" <<
                    CurNode->getName() << "\" ]" << std::endl;
                  }
                  PrintSucsessors(*CurNode.get());
                });
  Stream << "}" << std::endl;
}