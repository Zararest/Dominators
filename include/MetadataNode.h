#pragma once

#include <NamedNode.h>

struct DefaultMeta {
  virtual ~DefaultMeta() = default;
};

template <typename Meta> 
class MetadataNode : public NamedNode {
protected:
  static_assert(std::is_default_constructible<Meta>::value,
                "Metadata should be default constructable");

  std::unique_ptr<Meta> Metadata;

  class Builder_ : public NamedNode::Builder_ {
  protected:
    void configNode(MetadataNode &Node) {
      NamedNode::Builder_::configNode(Node);
      Node.Metadata.reset(new Meta);
    }

    Meta &getMutableMetadata(MetadataNode &Node) { 
      assert(Node.Metadata.get());
      return *Node.Metadata; 
    }

  public:
    std::unique_ptr<MetadataNode> createNode() {
      auto NewNode = std::unique_ptr<MetadataNode>(new MetadataNode);
      configNode(*NewNode);
      return NewNode;
    }

    virtual ~Builder_() = default;
  };

  MetadataNode() {}

public:
  using MetaDataT = Meta;

  struct Builder : public Builder_ {
    virtual ~Builder() = default;
  };

  const Meta &getMetadata() const { 
    assert(Metadata.get());
    return *Metadata; 
  }

  virtual ~MetadataNode() = default;
};