#include "Node.h"

#include <string>

class NamedNode : public Node {
  std::string Name = "Unnamed";
  
  struct Builder_ : public Node::Builder {
    NamedNode createNamedNode(const std::string &Name) {
      return {Name};
    }
  };

  NamedNode(const std::string &Name) : Name{Name} {} 

public: 
  struct Builder : public Builder_ {};

  std::string getName() const {
    return Name;
  }

  virtual ~NamedNode() {}
};