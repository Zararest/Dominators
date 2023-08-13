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
template <typename OwnedNodesIt>
void DomTreeBuilder<NodeT>::createDomGraph(NodeT &Root, OwnedNodesIt NodesBeg,
                                           OwnedNodesIt NodesEnd) {
  calcDominators(Root, NodesBeg, NodesEnd);

  auto DomNodesSet = std::unordered_set<const Node *>{};
  std::for_each(NodesBeg, NodesEnd, [&](std::unique_ptr<NodeT> &CurNode) {
    auto &CurNodeMeta = getMutableMetadata(*CurNode);
    CurNodeMeta.getDominators(std::inserter(DomNodesSet, DomNodesSet.begin()));
  });

  auto CreateDomNode = [&](const Node *InitialNode) {
    auto *InitNamedNodePtr = static_cast<const NamedNode *>(InitialNode);
    return DomNodeConfigurator{}.createDomNodeWithMeta(InitNamedNodePtr);
  };
  assert(DomNodesSet.extract(&Root));
  DomRoot.reset(CreateDomNode(&Root).release());

  auto MapInitNodesToDomNodes = std::unordered_map<const Node *, DomNode *>{};
  MapInitNodesToDomNodes.emplace(&Root, DomRoot.get());
  std::for_each(
      DomNodesSet.begin(), DomNodesSet.end(), [&](const Node *InitialNode) {
        DomTreeNodes.emplace_back(CreateDomNode(InitialNode));
        MapInitNodesToDomNodes[InitialNode] = DomTreeNodes.back().get();
      });
  createDomRelations(MapInitNodesToDomNodes);
}

template <typename NodeT>
void DomTreeBuilder<NodeT>::createDomRelations(
    std::unordered_map<const Node *, DomNode *> &NodesToDomNodesMapping) {
  auto AddNodeDominators = [&](std::unique_ptr<DomNode> &DomNode) {
    auto &InitNode = DomNode->getMetadata().getOriginalNode();
    auto InitMetadataNodePtr = static_cast<const NodeT *>(&InitNode);
    auto &DomMeta = InitMetadataNodePtr->getMetadata();
    auto InitNodeDominators = std::vector<const Node *>{};
    DomMeta.getDominators(std::back_inserter(InitNodeDominators));
    std::for_each(InitNodeDominators.begin(), InitNodeDominators.end(),
                  [&](const Node *Dominator) {
                    auto *DomNodeDominator = NodesToDomNodesMapping[Dominator];
                    if (DomNodeDominator == DomNode.get())
                      return;
                    addSucsessor(*DomNodeDominator, *DomNode);
                  });
  };
  std::for_each(DomTreeNodes.begin(), DomTreeNodes.end(), AddNodeDominators);
}