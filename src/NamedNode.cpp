#include <NamedNode.h>

size_t NamedNode::Builder_::NodesCreated = 0;

void NamedNode::Builder_::configNode(NamedNode &Node) {
  NodesCreated++;
  Node.setName(std::to_string(NodesCreated));
}

std::unique_ptr<NamedNode> NamedNode::Builder_::createNode() {
  auto NewNode = std::unique_ptr<NamedNode>(new NamedNode());
  configNode(*NewNode.get());
  return NewNode;
}
