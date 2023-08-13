#pragma once

#include <MetadataNode.h>
#include <DomMetadata.h>

using DomNode = MetadataNode<DomNodeMetadata>;

struct DomNodeConfigurator : public MetadataNode<DomNodeMetadata>::Builder {
  std::unique_ptr<DomNode> 
  createDomNodeWithMeta(const NamedNode *InitialNode) {
    auto DomNode = DomNode::Builder{}.createNode();
    setName(*DomNode, InitialNode->getName());
    auto &DomNodeMeta = getMutableMetadata(*DomNode);
    DomNodeMeta.setOriginalNode(InitialNode);
    return DomNode;
  }
};

