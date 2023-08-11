#include <NamedNode.h>

size_t NamedNode::Builder_::NodesCreated = 0;

std::unique_ptr<NamedNode> NamedNode::Builder_::createNode() {
  NodesCreated++;
  return std::unique_ptr<NamedNode>(
          new NamedNode(std::to_string(NodesCreated)));
}
