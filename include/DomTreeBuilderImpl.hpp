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
  auto RootPtr = static_cast<Node*>(&Root);
  RootMeta.initRoot(RootPtr);
  auto Changed = false;
  do {
    Changed = false;
    auto CalcDomForNode = [&](std::unique_ptr<NodeT> &CurNode) {
      auto IntersectDoms = [&](const Node *PredPtr) {
        auto &Pred = *static_cast<const NodeT*>(PredPtr);
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
void DomTreeBuilder<NodeT>::createDomTree(NodeT &Root, OwnedNodesIt NodesBeg,
                                          OwnedNodesIt NodesEnd) {
  calcDominators(Root, NodesBeg, NodesEnd);
  
  auto DomNodesSet = std::unordered_set<const Node*>{};
  std::for_each(NodesBeg, NodesEnd, 
                [&](std::unique_ptr<NodeT> &CurNode) {
                  auto &CurNodeMeta = getMutableMetadata(*CurNode);
                  CurNodeMeta.getDominators(std::inserter(DomNodesSet, 
                                                          DomNodesSet.begin()));
                });
  
  auto CreateDomNode = [&](const Node *InitialiNode) {
    auto *InitNamedNodePtr = static_cast<const NamedNode*>(InitialNode);
    auto DomNode = NamedNode::Builder{}.createNode();
    setName(*DomNode, InitNamedNodePtr->getName());
    return DomNode;
  };
  auto RootNode = DomNodeSet.extract(&Root);
  assert(RootNode);
  DomRoot.reset(CreateDomNode(RootNode));

  auto MapDomNodesToInitNodes = 
    std::unordered_map<NamedNode*, const Node*>{};
  auto MapInitNodesToDomNodes = 
    std::unordered_map<const Node*, NamedNode*>{};
  MapInitNodesToDomNodes.emplace(RootNode, DomRoot.get());
  std::for_each(DomNodesSet.begin(), DomNodesSet.end(),
                [&](const Node *InitialNode) {
                  DomTreeNodes.emplace_back(CreateDomNode(InitialiNode));
                  MapInitNodesToDomNodes.emplace(DomTreeNodes.back().get(),
                                                 InitialNode);
                  MapDomNodesToInitNodes.emplace(InitialNode,
                                                 DomTreeNodes.back().get());
                });
  createDomRelations(MapInitNodesToDomNodes);
}

template <typename NodeT>
void DomTreeBuilder<NodeT>::createDomRelations(std::unordered_map<NamedNode*, const Node*> &DomNodesToNodesMapping) {
  auto AddNodeDominators = 
    [&](std::unique_ptr<NamedNode> DomNode) {
      assert(DomNodesToNodesMapping.find(DomNode.get()) != 
            DomNodesToNodesMapping.end());
      auto [_, InitNode] = DomNodesToNodesMapping.find(DomNode.get());
      auto InitMetadataNodePtr = static_cast<NodeT*>(InitNode);
      auto &DomMeta = getMutableMetadata(*InitMetadataNodePtr);
      auto InitNodeDominators = std::vector<const Node*>{};
      DomMeta.getDominators(std::back_inserter(InitNodeDominators));
      std::for_each(InitNodeDominators.begin(), InitNodeDominators.end(),
                    [&](const Node *Dominator) {

                    });
    };
  std::for_each(DomTreeNodes.begin(), DomTreeNodes.end(),
                );
}