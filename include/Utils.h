#pragma once

#include <NamedNode.h>
#include <unordered_set>

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

template <typename T, typename InsertIt>
void intersectSets(const std::unordered_set<T> &Lhs,
                   const std::unordered_set<T> &Rhs, InsertIt Intersection) {
  auto &SmallerSetRef = Lhs.size() < Rhs.size() ? Lhs : Rhs;
  auto &BiggerSetRef = Lhs.size() >= Rhs.size() ? Lhs : Rhs;
  std::for_each(SmallerSetRef.begin(), SmallerSetRef.end(), [&](const T &Obj) {
    if (BiggerSetRef.find(Obj) != BiggerSetRef.end())
      Intersection = Obj;
  });
}

template <typename T>
std::unordered_set<T> subtractSets(const std::unordered_set<T> &Minuend,
                                   const std::unordered_set<T> &Subtrahend) {
  auto Result = std::unordered_set<T>{};
  std::copy_if(
      Minuend.begin(), Minuend.end(), std::inserter(Result, Result.begin()),
      [&](const T &Obj) { return Subtrahend.find(Obj) == Subtrahend.end(); });
  return Result;
}