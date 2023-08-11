#include "Node.h"

#include <string>

class NamedNode : public Node {
  std::string Name = "Unnamed";
  
  class Builder_ : public Node::Builder {
    static size_t NodesCreated; 

  public:
    std::unique_ptr<NamedNode> createNode();
  };

  NamedNode(const std::string &Name) : Name{Name} {} 

public: 
  struct Builder : public Builder_ {};

  std::string getName() const {
    return Name;
  }

  virtual ~NamedNode() {}
};