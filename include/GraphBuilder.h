#pragma once

#include <NamedNode.h>

#include <memory>
#include <iostream>

using OwnedNode = std::unique_ptr<NamedNode>; 

// Mutates given node and owns secondary nodes
class ReducibleGraphBuilder : public NamedNode::NodeBuilder {
  std::vector<OwnedNode> SecondaryNodes;
  Node &Root;

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