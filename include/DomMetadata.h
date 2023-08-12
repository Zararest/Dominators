#pragma once

#include <algorithm>
#include <unordered_set>

using DomSetT = std::unordered_set<Node *>;

struct DomMetadata final {
  // all nodes that dominates current node
  DomSetT Dominators;
  bool IsDominatedByAll = true;

  static DomMetadata getDomIntersection(const DomMetadata &Lhs,
                                        const DomMetadata &Rhs) {
    if (Lhs.IsDominatedByAll)
      return Rhs;
    if (Rhs.IsDominatedByAll)
      return Lhs;
    auto IntersectSet = DomSetT{};
    std::set_intersection(Lhs.Dominators.begin(), Lhs.Dominators.end(),
                          Rhs.Dominators.begin(), Rhs.Dominators.end(),
                          std::inserter(IntersectSet, IntersectSet.begin()));
    return {IntersectSet, false};
  }
};