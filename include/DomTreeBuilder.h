#pragma once

#include <DomNodeConfig.h>
#include <DomMetadata.h>
#include <MetadataNode.h>
#include <Utils.h>

#include <unordered_map>

template <typename NodeT>
class DomTreeBuilder : public MetadataNode<DomMetadata>::Builder {
  static_assert(std::is_base_of_v<DomMetadata, typename NodeT::MetaDataT>,
                "Metadata should contain dominators");

  std::unique_ptr<DomNode> DomRoot;
  std::vector<std::unique_ptr<DomNode>> DomTreeNodes;

  void createDomRelations(std::unordered_map<const Node*, DomNode*>
                            &NodesToDomNodesMapping);

public:
  // Changes metadata in the initial tree.
  template <typename OwnedNodesIt>
  void calcDominators(NodeT &Root, OwnedNodesIt NodesBeg,
                      OwnedNodesIt NodesEnd);

  // Builds dom tree from initial tree.
  template <typename OwnedNodesIt>
  void createDomTree(NodeT &Root, OwnedNodesIt NodesBeg, OwnedNodesIt NodesEnd);

  ConstOwnedNodesRange<DomNode> getNodes() const {
    return {DomTreeNodes.begin(), DomTreeNodes.end()};
  }

  const DomNode &getDomRoot() const { return *DomRoot; }
};

#include <DomTreeBuilderImpl.hpp>