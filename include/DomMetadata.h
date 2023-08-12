#pragma once

#include <MetadataNode.h>

#include <algorithm>
#include <unordered_set>

using DomSetT = std::unordered_set<Node*>;

struct DomMetadata : public DefaultMeta {
  // all nodes that dominates current node
  DomSetT Dominators;
  bool IsDominatedByAll = true;

  void intersect(const DomMetadata &PredMeta) {
    if (&PredMeta == this)
      return;
    if (PredMeta.IsDominatedByAll)
      return;
    if (IsDominatedByAll) {
      Dominators = PredMeta.Dominators;
      IsDominatedByAll = false;
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
    std::swap(Dominators, Intersection);
  }

  virtual ~DomMetadata() = default;
};