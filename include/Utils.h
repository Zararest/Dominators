#pragma once

#include <NamedNode.h>

template <typename It>
using GetTypeFromOwnedNodeIt =
    typename std::iterator_traits<It>::value_type::element_type;

template <typename Iterator> struct is_const_iterator {
  using PtrT = typename std::iterator_traits<Iterator>::pointer;
  static const bool value =
      std::is_const<typename std::remove_pointer<PtrT>::type>::value;
};

std::vector<Node *> getReversePostOrder(Node &Root);

// Works with unique ptrs
template <typename It, typename RootT>
void printGraph(It NodesBeg, It NodesEnd, RootT &Root, std::ostream &Stream) {
  Stream << "digraph Dump {\n"
         << "node[color=red,fontsize=14, style=filled]\n"
         << "\"" << &Root << "\""
         << " [label = \"Root\" fillcolor=green]\n";
  auto PrintSucsessors = [&Stream](const Node &CurNode) {
    auto SucsRange = Node::Builder{}.getSucsessors(CurNode);
    std::for_each(SucsRange.Begin, SucsRange.End,
                  [&CurNode, &Stream](const Node *NodePtr) {
                    Stream << "\"" << &CurNode << "\""
                           << " -> "
                           << "\"" << NodePtr << "\"" << std::endl;
                  });
  };
  PrintSucsessors(Root);
  std::for_each(NodesBeg, NodesEnd, [&](auto &CurNode) {
    if constexpr (std::is_base_of_v<NamedNode, GetTypeFromOwnedNodeIt<It>>) {
      Stream << "\"" << CurNode.get() << "\""
             << " [label = \"" << CurNode->getName() << "\" ]" << std::endl;
    }
    PrintSucsessors(*CurNode);
  });
  Stream << "}" << std::endl;
}