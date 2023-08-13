#pragma once

#include <MetadataNode.h>
#include <Utils.h>

#include <algorithm>
#include <limits>
#include <unordered_set>

// Metadata with node dominators.
class DomMetadata : public EmptyMetadata {
  // all nodes that dominates current node
  std::unordered_set<const Node *> Dominators;
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
    auto Intersection = std::unordered_set<const Node *>{};
    intersectSets(Dominators, PredMeta.Dominators,
                  std::inserter(Intersection, Intersection.begin()));
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

  void addDominator(const Node *NewDom) { Dominators.insert(NewDom); }

  const auto getDominators() const {
    return std::make_pair(Dominators.begin(), Dominators.end());
  }

  const std::unordered_set<const Node *> getDominatorsSet() const {
    return Dominators;
  }

  bool isDominatedByNode(const Node *ProbDom) {
    return Dominators.find(ProbDom) != Dominators.end();
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

  void setOriginalNode(const Node *NewOrigNode) { OriginalNode = NewOrigNode; }
};