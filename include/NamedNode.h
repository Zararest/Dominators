#pragma once

#include "Node.h"

#include <string>

class NamedNode : public Node {
protected:
  std::string Name = "Unnamed";

  // Private builders has protected configNode method,
  //  so each sucsessor should be inherited from previous private builder.
  class Builder_ : public Node::Builder_ {
    static size_t NodesCreated;

  protected:
    void configNode(NamedNode &Node);

    void setName(NamedNode &Node, const std::string &NewName) const {
      Node.Name = NewName;
    }

  public:
    std::unique_ptr<NamedNode> createNode();

    virtual ~Builder_() = default;
  };

  NamedNode() {}

public:
  struct Builder : public Builder_ {
    virtual ~Builder() = default;
  };

  std::string getName() const { return Name; }

  virtual ~NamedNode() = default;
};