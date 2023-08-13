#pragma once

#include <DomMetadata.h>
#include <DomNodeConfig.h>
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

  void createDomRelations(
      std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping);

public:
  // Changes metadata in the initial tree.
  template <typename OwnedNodesIt>
  void calcDominators(NodeT &Root, OwnedNodesIt NodesBeg,
                      OwnedNodesIt NodesEnd);

  // Builds graph where m->n means that m is in Dom(n).
  template <typename OwnedNodesIt>
  void createDomGraph(NodeT &Root, OwnedNodesIt NodesBeg,
                      OwnedNodesIt NodesEnd);

  // Builds tree where m->n means that m is in IDom(n).
  template <typename OwnedNodesIt>
  void createDomTree(NodeT &Root, OwnedNodesIt NodesBeg, OwnedNodesIt NodesEnd);

  ConstOwnedNodesRange<DomNode> getNodes() const {
    return {DomTreeNodes.begin(), DomTreeNodes.end()};
  }

  const DomNode &getDomRoot() const { return *DomRoot; }
};

#include <DomTreeBuilderImpl.hpp>