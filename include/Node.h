# pragma once

#include <vector>
#include <utility>

class Node;

struct ConstNodesRange {
  std::vector<Node*>::const_iterator begin;
  std::vector<Node*>::const_iterator end;
};

class Node {
protected:
  std::vector<Node*> Sucsessors;
  std::vector<Node*> Predecessors;

  struct NodeBuilder_ {
    ConstNodesRange getSucsessors(const Node &Node) const {
      return {Node.Sucsessors.begin(), Node.Sucsessors.end()};
    }

    ConstNodesRange getPredecessors(const Node &Node) const {
      return {Node.Predecessors.begin(), Node.Predecessors.end()};
    }

    Node createNode() const { return {}; }

    void addSucsessor(Node &CurNode, Node &NextNode) const {
      CurNode.Sucsessors.push_back(&NextNode);
      NextNode.Predecessors.push_back(&CurNode);
    }

    void addPredecessor(Node &CurNode, Node &PrevNode) const {
      CurNode.Predecessors.push_back(&PrevNode);
      PrevNode.Sucsessors.push_back(&CurNode);
    }

    virtual ~NodeBuilder_() {}
  };

  Node(){}

public:
  struct NodeBuilder : public NodeBuilder_ {};

  Node(const Node&) = delete;
  Node operator=(const Node&) = delete;

  virtual ~Node(){}
};
