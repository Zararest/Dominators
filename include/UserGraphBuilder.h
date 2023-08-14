#pragma once

#include <NamedNode.h>

#include <unordered_map>

template <typename NodeT>
class UserGraphBuilder final : public NamedNode::Builder {
  static_assert(std::is_base_of_v<NamedNode, NodeT>,
                "User graph nodes should have names");

  std::unordered_map<std::string, NodeT *> MapNamesToNodes;
  std::ostream &Err;

  std::unique_ptr<NodeT> UserRoot;
  std::vector<std::unique_ptr<NodeT>> UserGraphNodes;

  void printError(size_t ErrPosition);

  bool parseRoot(std::istream &IStream);

  bool parseRelation(std::istream &IStream, size_t LineNum);

  void checkNodePresence(const std::string &NodeName);

public:
  UserGraphBuilder(std::ostream &ErrStream) : Err{ErrStream} {}

  void buildGraph(std::istream &IStream);

  ConstOwnedNodesRange<NodeT> getNodes() const {
    return {UserGraphNodes.begin(), UserGraphNodes.end()};
  }

  OwnedNodesRange<NodeT> getMutableNodes() {
    return {UserGraphNodes.begin(), UserGraphNodes.end()};
  }

  NodeT &getRoot() { 
    assert(UserRoot);
    return *UserRoot; 
  }
};

#include <UserGraphBuilderImpl.hpp>