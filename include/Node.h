# pragma once

#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>
#include <memory>
#include <iostream>

class Node;

struct ConstNodesRange {
  std::vector<Node*>::const_iterator begin;
  std::vector<Node*>::const_iterator end;
};

struct NodesRange {
  std::vector<Node*>::iterator begin;
  std::vector<Node*>::iterator end;
};

class Node {
protected:
  std::vector<Node*> Sucsessors;
  std::vector<Node*> Predecessors;

  struct Builder_ {
    ConstNodesRange getSucsessors(const Node &Node) const {
      return {Node.Sucsessors.begin(), Node.Sucsessors.end()};
    }

    ConstNodesRange getPredecessors(const Node &Node) const {
      return {Node.Predecessors.begin(), Node.Predecessors.end()};
    }

    NodesRange getMutableSucsessors(Node &Node) const {
      return {Node.Sucsessors.begin(), Node.Sucsessors.end()};
    }

    NodesRange getMutablePredecessors(Node &Node) const {
      return {Node.Predecessors.begin(), Node.Predecessors.end()};
    }

    std::unique_ptr<Node> createNode() const { 
      return std::unique_ptr<Node>(new Node); 
    }

    void addSucsessor(Node &CurNode, Node &NextNode) const {
      CurNode.Sucsessors.push_back(&NextNode);
      NextNode.Predecessors.push_back(&CurNode);
    }

    void swapSucsessors(Node &Lhs, Node &Rhs) const {
      auto RebindPredecessorForSucsessors = [](Node &SrcNode, Node &NewNode) {
        std::for_each(SrcNode.Sucsessors.begin(), SrcNode.Sucsessors.end(),
                      [&NewNode, &SrcNode](Node *Sucs) {
                        auto PredToChange = 
                          std::find(Sucs->Predecessors.begin(),
                                    Sucs->Predecessors.end(), 
                                    &SrcNode);
                        assert(PredToChange != Sucs->Predecessors.end());
                        *PredToChange = &NewNode;
                      });
      };
      RebindPredecessorForSucsessors(Lhs, Rhs);
      RebindPredecessorForSucsessors(Rhs, Lhs);
      std::swap(Lhs.Sucsessors, Rhs.Sucsessors);
    }

    virtual ~Builder_() = default;
  };

  Node(){}

public:
  struct Builder : public Builder_ {
    virtual ~Builder() = default;
  };

  Node(const Node&) = delete;
  Node operator=(const Node&) = delete;

  virtual ~Node() = default;
};
