#pragma once

#include <NamedNode.h>

#include <iostream>
#include <memory>

/*
  Each node class has builder struct
  in order to separate nodes properties from creation logic.
  ReducibleGraphBuilder requires createNode() function from builder.
*/

// FIXME: move nodes to the separate container
template <typename T> class ReducibleGraphBuilder : public Node::Builder {
  static_assert(std::is_base_of_v<Node, T>, "Graph should contain only Nodes");
  using OwnedNode = std::unique_ptr<T>;

  std::vector<OwnedNode> SecondaryNodes;
  Node &Root;
  size_t Seed = 1u;

  enum Mutations { Split, BackEdge, NewPath };

  size_t getRandomValue(size_t From, size_t To);

  void splitNode(Node &NodeToSplit);
  // adds back edge only if node doesn't have one
  void addBackEdge(Node &NodeForEdge);

  void addPathToSucsessor(Node &NodeForPath);

public:
  ReducibleGraphBuilder(Node &RootNode) : Root{RootNode} {}

  // Performs one of 3 operations on the random node.
  // Operations:
  //  -split node into 2
  //  -create back edge on the node itself
  //  -add another way to the one of the sucsessors
  void mutate();

  ConstOwnedNodesRange<T> getNodes() const {
    return {SecondaryNodes.begin(), SecondaryNodes.end()};
  }

  OwnedNodesRange<T> getMutableNodes() {
    return {SecondaryNodes.begin(), SecondaryNodes.end()};
  }
};

#include <GraphBuilderImpl.hpp>