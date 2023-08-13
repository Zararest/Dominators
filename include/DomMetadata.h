#pragma once

#include <MetadataNode.h>

#include <algorithm>
#include <unordered_set>
#include <limits>

// Metadata with node dominators.
class DomMetadata : public EmptyMetadata {
  // all nodes that dominates current node
  std::unordered_set<const Node*>; Dominators;
  bool IsDominatedByAll = true;

public:
  void intersect(const DomMetadata &PredMeta) {
    if (&PredMeta == this)
      return;
    if (PredMeta.IsDominatedByAll)
      return;
    if (IsDominatedByAll) {
      Dominators = PredMeta.Dominators;
      IsDominatedByAll = false;
      return;
    }
    auto Intersection = std::unordered_set<const Node*>;{};
    auto &SmallerSetRef = 
      Dominators.size() < PredMeta.Dominators.size() ? Dominators
                                                     : PredMeta.Dominators;
    auto &BiggerSetRef =
      Dominators.size() >= PredMeta.Dominators.size() ? Dominators
                                                      : PredMeta.Dominators;
    std::for_each(SmallerSetRef.begin(), SmallerSetRef.end(),
                  [&](const Node *DomNode) {
                    if (BiggerSetRef.find(DomNode) != BiggerSetRef.end()) {
                      auto [_, Inserted] = Intersection.insert(DomNode);
                      assert(Inserted);
                    }
                  });
    assert(Dominators.size() >= Intersection.size());
    std::swap(Dominators, Intersection);
  }

  size_t getNumOfDominators() const {
    if (IsDominatedByAll)
      return std::numeric_limits<size_t>::max();
    return Dominators.size();
  }

  void initRoot(const Node *RootNode) {
    Dominators.clear();
    auto [_, Inserted] = Dominators.insert(RootNode);
    assert(Inserted);
    IsDominatedByAll = false;
  }

  void addDominator(const Node *NewDom) {
    Dominators.insert(NewDom);
  }

  template <typename InsertIt>
  void getDominators(InsertIt It) const {
    std::for_each(Dominators.begin(), Dominators.end(),
                  [&](const Node *DomNode) {
                    It = DomNode;
                  });
  }

  virtual ~DomMetadata() = default;
};

// Metadata for secondary graph node about original node.
class OriginalNodeMetadata : public EmptyMetadata {
  const Node *OriginalNode = nullptr;

public:
  const Node &getOriginalNode() const {
    assert(OriginalNode);
    return *OriginalNode;
  }

  void setOriginalNode(const Node *NewOrigNode) {
    OriginalNode = NewOrigNode;
  }
};