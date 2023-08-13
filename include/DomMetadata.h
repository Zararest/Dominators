#pragma once

#include <MetadataNode.h>

#include <algorithm>
#include <unordered_set>
#include <limits>

using DomSetT = std::unordered_set<const Node*>;

class DomMetadata : public DefaultMeta {
  // all nodes that dominates current node
  DomSetT Dominators;
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
    auto Intersection = DomSetT{};
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