#pragma once

#include <NamedNode.h>

#include <memory>
#include <iostream>

using OwnedNode = std::unique_ptr<NamedNode>; 

// Mutates given node and owns secondary nodes
class ReducibleGraphBuilder : public Node::NodeBuilder {
  std::vector<OwnedNode> SecondaryNodes;
  Node &Root;
  size_t Seed = 1u;

  enum Mutations {
    Split,
    BackEdge,
    NewPath
  };

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

  void printGraph(std::ostream &Stream);
};