template <typename NodeT>
template <typename OwnedNodesIt>
void DomTreeBuilder<NodeT>::calcDominators(NodeT &Root, 
                                           OwnedNodesIt NodesBeg, OwnedNodesIt NodesEnd) {
  static_assert(std::is_same<GetTypeFromOwnedNodeIt<OwnedNodesIt>, NodeT>::value,
    "Iterator should be to the same node type as root");
}

template <typename NodeT>
template <typename OwnedNodesIt>
void DomTreeBuilder<NodeT>::createDomTree(NodeT &Root, OwnedNodesIt NodesBeg, OwnedNodesIt NodesEnd) {

}