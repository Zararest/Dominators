#pragma once

#include <MetadataNode.h>

#include <algorithm>
#include <unordered_set>

using DomSetT = std::unordered_set<Node*>;

struct DomMetadata : public DefaultMeta {
  // all nodes that dominates current node
  DomSetT Dominators;
  bool IsDominatedByAll = true;

  bool intersect(const DomMetadata &PredMeta) {
    if (&PredMeta == this)
      return false;
    if (PredMeta.IsDominatedByAll)
      return false;
    if (IsDominatedByAll) {
      Dominators = PredMeta.Dominators;
      IsDominatedByAll = false;
      return true;
    }
    auto Intersection = DomSetT{};
    auto &SmallerSetRef = 
      Dominators.size() < PredMeta.Dominators.size() ? Dominators
                                                     : PredMeta.Dominators;
    auto &BiggerSetRef =
      Dominators.size() >= PredMeta.Dominators.size() ? Dominators
                                                      : PredMeta.Dominators;
    std::for_each(SmallerSetRef.begin(), SmallerSetRef.end(),
                  [&](Node *DomNode) {
                    if (BiggerSetRef.find(DomNode) != BiggerSetRef.end()) {
                      auto [_, Inserted] = Intersection.insert(DomNode);
                      assert(Inserted);
                    }
                  });
    assert(Dominators.size() >= Intersection.size());
    std::swap(Dominators, Intersection);
    if (Dominators.size() != Intersection.size())
      return true;
    return false;
  }

  virtual ~DomMetadata() = default;
};