// Asumes that nodes are initializaed somehow correctly
template <typename NodeT>
template <typename OwnedNodesIt>
void DomTreeBuilder<NodeT>::calcDominators(NodeT &Root, OwnedNodesIt NodesBeg,
                                           OwnedNodesIt NodesEnd) {
  static_assert(
      std::is_same<GetTypeFromOwnedNodeIt<OwnedNodesIt>, NodeT>::value,
      "Iterator should be to the same node type as root");
  static_assert(!is_const_iterator<OwnedNodesIt>::value,
      "Nodes should be mutable");
  auto &RootMeta = getMutableMetadata(Root);
  RootMeta.IsDominatedByAll = false;
  RootMeta.Dominators.clear();
  auto RootPtr = static_cast<Node*>(&Root);
  RootMeta.Dominators.insert(RootPtr);
  auto Changed = false;
  do {
    Changed = false;
    auto CalcDomForNode = [&](std::unique_ptr<NodeT> &NodeIt) {
      auto IntersectDoms = [&](const Node *PredPtr) {
        auto &Pred = *static_cast<const NodeT*>(PredPtr);
        auto &CurDomMeta = getMutableMetadata(*NodeIt);
        auto &PredMeta = Pred.getMetadata();
        CurDomMeta.intersect(PredMeta);
      };
      auto [PredBeg, PredEnd] = getPredecessors(*NodeIt.get());
      std::for_each(PredBeg, PredEnd, IntersectDoms);
    };
    std::for_each(NodesBeg, NodesEnd, CalcDomForNode);
  } while (Changed);
}

template <typename NodeT>
template <typename OwnedNodesIt>
void DomTreeBuilder<NodeT>::createDomTree(NodeT &Root, OwnedNodesIt NodesBeg,
                                          OwnedNodesIt NodesEnd) {

}