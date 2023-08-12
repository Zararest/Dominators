#pragma once

#include <NamedNode.h>

template <typename Meta>
class MetadataNode : public NamedNode {
protected:
  static_assert(std::is_default_constructible<Meta>::value, 
    "Metadata should be default constructable");
  Meta Metadata;

  class Builder_ : public NamedNode::Builder_ {
  protected:
    void configNode(MetadataNode &Node) {
      NamedNode::Builder_::configNode(Node);
    }

    Meta &getMutableMetadata(MetadataNode &Node) {
      return Node.Metadata;
    }

  public:  
    std::unique_ptr<MetadataNode> createNode() {
      auto NewNode = std::unique_ptr<MetadataNode>(new MetadataNode);
      configNode(*NewNode.get());
      return NewNode;
    }

    virtual ~Builder_() = default;
  };

  MetadataNode() {}

public:
  struct Builder : public Builder_ {
    virtual ~Builder() = default;
  };

  const Meta &getMetadata() const {
    return Metadata;
  }

  virtual ~MetadataNode() = default;
};