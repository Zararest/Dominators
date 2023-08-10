#include "Node.h"

#include <string>

class NamedNode : public Node {
  std::string Name = "Unnamed";

public: 
  std::string getName() const {
    return Name;
  }

  void setName(const std::string &NewName) {
    Name = NewName;
  }
};