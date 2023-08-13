#pragma once

#include <DomMetadata.h>
#include <MetadataNode.h>
#include <Utils.h>

#include <unordered_map>

using DomNode = MetadataNode<OriginalNodeMetadata>;

struct DomNodeConfigurator
    : public MetadataNode<OriginalNodeMetadata>::Builder {
  std::unique_ptr<DomNode> createDomNodeWithMeta(const NamedNode *InitialNode) {
    auto DomNode = DomNode::Builder{}.createNode();
    setName(*DomNode, InitialNode->getName());
    auto &DomNodeMeta = getMutableMetadata(*DomNode);
    DomNodeMeta.setOriginalNode(InitialNode);
    return DomNode;
  }
};

template <typename NodeT>
class DomTreeBuilder : public MetadataNode<DomMetadata>::Builder {
  static_assert(std::is_base_of_v<DomMetadata, typename NodeT::MetaDataT>,
                "Metadata should contain dominators");

  std::unique_ptr<DomNode> DomRoot;
  std::vector<std::unique_ptr<DomNode>> DomTreeNodes;

  void createIDomRelations(
      std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping);

  void createDomRelations(
      std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping);

  template <typename OwnedNodeIt>
  std::unordered_map<const Node *, DomNode *> 
  createUnrelatedDomNodes(NodeT &Root, OwnedNodeIt NodesBeg,
                          OwnedNodeIt NodesEnd);

public:
  // Changes metadata in the initial tree.
  template <typename OwnedNodeIt>
  void calcDominators(NodeT &Root, OwnedNodeIt NodesBeg,
                      OwnedNodeIt NodesEnd);

  // Builds graph where m->n means that m is in Dom(n).
  template <typename OwnedNodeIt>
  void createDomGraph(NodeT &Root, OwnedNodeIt NodesBeg,
                      OwnedNodeIt NodesEnd);

  // Builds tree where m->n means that m is in IDom(n).
  template <typename OwnedNodeIt>
  void createDomTree(NodeT &Root, OwnedNodeIt NodesBeg, OwnedNodeIt NodesEnd);

  ConstOwnedNodesRange<DomNode> getNodes() const {
    return {DomTreeNodes.begin(), DomTreeNodes.end()};
  }

  const DomNode &getDomRoot() const { return *DomRoot; }
};

#include <DomTreeBuilderImpl.hpp>