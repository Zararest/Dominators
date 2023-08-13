// Asumes that nodes are initializaed somehow correctly
template <typename NodeT>
template <typename OwnedNodeIt>
void DomTreeBuilder<NodeT>::calcDominators(NodeT &Root, OwnedNodeIt NodesBeg,
                                           OwnedNodeIt NodesEnd) {
  static_assert(
      std::is_same<GetTypeFromOwnedNodeIt<OwnedNodeIt>, NodeT>::value,
      "Iterator should be to the same node type as root");
  static_assert(!is_const_iterator<OwnedNodeIt>::value,
                "Nodes should be mutable");
  auto &RootMeta = getMutableMetadata(Root);
  auto RootPtr = static_cast<Node *>(&Root);
  RootMeta.initRoot(RootPtr);
  auto Changed = false;
  do {
    Changed = false;
    auto CalcDomForNode = [&](std::unique_ptr<NodeT> &CurNode) {
      auto IntersectDoms = [&](const Node *PredPtr) {
        auto &Pred = *static_cast<const NodeT *>(PredPtr);
        auto &CurDomMeta = getMutableMetadata(*CurNode);
        auto &PredMeta = Pred.getMetadata();
        auto NumOfDomBefore = CurDomMeta.getNumOfDominators();
        CurDomMeta.intersect(PredMeta);
        CurDomMeta.addDominator(CurNode.get());
        Changed |= NumOfDomBefore > CurDomMeta.getNumOfDominators();
        assert(NumOfDomBefore >= CurDomMeta.getNumOfDominators());
      };
      auto [PredBeg, PredEnd] = getPredecessors(*CurNode);
      std::for_each(PredBeg, PredEnd, IntersectDoms);
    };
    std::for_each(NodesBeg, NodesEnd, CalcDomForNode);
  } while (Changed);
}

template <typename NodeT>
template <typename OwnedNodeIt>
void DomTreeBuilder<NodeT>::createDomGraph(NodeT &Root, OwnedNodeIt NodesBeg,
                                           OwnedNodeIt NodesEnd) {
  calcDominators(Root, NodesBeg, NodesEnd);
  auto MapOrigNodesToDomNodes =
      createUnrelatedDomNodes(Root, NodesBeg, NodesEnd);
  createDomRelations(MapOrigNodesToDomNodes);
}

template <typename NodeT>
template <typename OwnedNodeIt>
void DomTreeBuilder<NodeT>::createDomTree(NodeT &Root, OwnedNodeIt NodesBeg,
                                          OwnedNodeIt NodesEnd) {
  calcDominators(Root, NodesBeg, NodesEnd);
  auto MapOrigNodesToDomNodes =
      createUnrelatedDomNodes(Root, NodesBeg, NodesEnd);
  createIDomRelations(MapOrigNodesToDomNodes);
}

template <typename NodeT>
void DomTreeBuilder<NodeT>::createDomRelations(
    std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping) {
  auto AddNodeDominators = [&](std::unique_ptr<DomNode> &DomNode) {
    auto &OrigNode = DomNode->getMetadata().getOriginalNode();
    auto &DomMeta = static_cast<const NodeT *>(&OrigNode)->getMetadata();
    auto [OrigNodeDomBeg, OrigNodeDomEnd] = DomMeta.getDominators();
    std::for_each(OrigNodeDomBeg, OrigNodeDomEnd, [&](const Node *Dominator) {
      auto *DomNodeDominator = NodesToDomNodesMapping[Dominator];
      if (DomNodeDominator == DomNode.get())
        return;
      addSucsessor(*DomNodeDominator, *DomNode);
    });
  };
  std::for_each(DomTreeNodes.begin(), DomTreeNodes.end(), AddNodeDominators);
}

template <typename NodeT>
template <typename OwnedNodeIt>
std::unordered_map<const Node *, DomNode *> 
DomTreeBuilder<NodeT>::createUnrelatedDomNodes(NodeT &Root, 
                                               OwnedNodeIt NodesBeg,
                                               OwnedNodeIt NodesEnd) {
  using OwnedNodeT = typename std::iterator_traits<OwnedNodeIt>::value_type;
  auto CreateDomNode = [&](const Node *InitialNode) {
    auto *InitNamedNodePtr = static_cast<const NamedNode *>(InitialNode);
    return DomNodeConfigurator{}.createDomNodeWithMeta(InitNamedNodePtr);
  };
  DomRoot.reset(CreateDomNode(&Root).release());

  auto MapOrigNodesToDomNodes = std::unordered_map<const Node *, DomNode *>{};
  MapOrigNodesToDomNodes.emplace(&Root, DomRoot.get());
  std::for_each(NodesBeg, NodesEnd,
      [&](OwnedNodeT &InitialNode) {
        DomTreeNodes.emplace_back(CreateDomNode(InitialNode.get()));
        MapOrigNodesToDomNodes[InitialNode.get()] = DomTreeNodes.back().get();
      });
  return MapOrigNodesToDomNodes;
}

template <typename NodeT>
void DomTreeBuilder<NodeT>::createIDomRelations(
    std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping) {
  auto AddNodeImmDom = [&](std::unique_ptr<DomNode> &DomNode) {
    auto &OrigNode = DomNode->getMetadata().getOriginalNode();
    auto &DomMeta = static_cast<const NodeT *>(&OrigNode)->getMetadata();
    auto [OrigNodeDomBeg, OrigNodeDomEnd] = DomMeta.getDominators();
    auto OrigNodeDomSet =
        std::unordered_set<const Node *>{OrigNodeDomBeg, OrigNodeDomEnd};
    auto CheckIDom = [&](const Node *OrigNodeDom) {
      if (OrigNodeDom == &OrigNode)
        return;
      auto &DomMetaOfDom =
          static_cast<const NodeT *>(OrigNodeDom)->getMetadata();
      auto DomsBetweenOrigAndCurNodes =
          subtractSets(OrigNodeDomSet, DomMetaOfDom.getDominatorsSet());
      // std::swap(OrigNodeDomSet, DomsBetweenOrigAndCurNodes)
      if (DomsBetweenOrigAndCurNodes.size() == 1) {
        assert(DomsBetweenOrigAndCurNodes.find(&OrigNode) !=
               DomsBetweenOrigAndCurNodes.end());
        auto *IDom = NodesToDomNodesMapping[OrigNodeDom];
        addSucsessor(*IDom, *DomNode);
      }
    };
    std::for_each(OrigNodeDomBeg, OrigNodeDomEnd, CheckIDom);
  };
  std::for_each(DomTreeNodes.begin(), DomTreeNodes.end(), AddNodeImmDom);
}
