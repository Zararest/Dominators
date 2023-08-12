template <typename T>
size_t ReducibleGraphBuilder<T>::getRandomValue(size_t From, size_t To) {
  assert(To > From);
  Seed ^= Seed << 13;
  Seed ^= Seed >> 7;
  Seed ^= Seed << 17;
  return From + (Seed % (To - From)); 
} 

template <typename T>
void ReducibleGraphBuilder<T>::splitNode(Node &NodeToSplit) {
  auto NewNode = typename T::Builder{}.createNode();
  auto &NewNodeRef = *NewNode.get();
  auto DefaultBuilder = Node::Builder{};
  DefaultBuilder.swapSucsessors(NewNodeRef, NodeToSplit);
  DefaultBuilder.addSucsessor(NodeToSplit, NewNodeRef);
  SecondaryNodes.emplace_back(std::move(NewNode));
}

// FIXME: check if this will be reducible graph
// adds back edge only if node doesn't have one
template <typename T>
void ReducibleGraphBuilder<T>::addBackEdge(Node &NodeForEdge) {
  auto DefaultBuilder = Node::Builder{};
  auto [SucsBeg, SucsEnd] = DefaultBuilder.getSucsessors(NodeForEdge);
  auto BackEdgeNode = std::find(SucsBeg, 
                                SucsEnd, 
                                &NodeForEdge);
  if (BackEdgeNode != SucsEnd)
    return;
  DefaultBuilder.addSucsessor(NodeForEdge, NodeForEdge);
}

template <typename T>
void ReducibleGraphBuilder<T>::addPathToSucsessor(Node &NodeForPath) {
  auto DefaultBuilder = Node::Builder{};
  auto [SucsBeg, SucsEnd] = DefaultBuilder.getSucsessors(NodeForPath);
  auto NumOfSucsessors = std::distance(SucsBeg, SucsEnd);
  if (NumOfSucsessors == 0)
    return;
  auto DuplicatePathTo = getRandomValue(0, NumOfSucsessors);
  auto NewNode = typename T::Builder{}.createNode();
  auto &NewNodeRef = *NewNode.get();
  DefaultBuilder.addSucsessor(NodeForPath, NewNodeRef);
  auto [NewSucsBeg, _] = DefaultBuilder.getMutableSucsessors(NodeForPath);
  auto SucsPtr = *(NewSucsBeg + DuplicatePathTo);
  DefaultBuilder.addSucsessor(NewNodeRef, *SucsPtr);
  SecondaryNodes.emplace_back(std::move(NewNode));
}

template <typename T>
void ReducibleGraphBuilder<T>::mutate() {
  auto NodesNum = SecondaryNodes.size();
  Node &NodeToMutate = 
    (NodesNum == 0) ? Root
                  : *SecondaryNodes[getRandomValue(0, NodesNum)].get();
  constexpr auto MutationsNum = 3;
  switch (getRandomValue(0, MutationsNum)) {
    case Split:
      splitNode(NodeToMutate);
    break;
    case BackEdge:
      addBackEdge(NodeToMutate);
    break;
    case NewPath:
      addPathToSucsessor(NodeToMutate);
    break;
    default:
      assert(false && "Unreachable");
    break;
  }
}

template <typename T>
void ReducibleGraphBuilder<T>::printGraph(std::ostream &Stream) {
  Stream << "digraph Dump {\n" <<
            "node[color=red,fontsize=14, style=filled]\n" <<
            "\"" << &Root << "\"" << " [label = \"Root\" fillcolor=green]\n";
  auto PrintSucsessors = [&Stream](const Node &CurNode) {
    auto SucsRange = Node::Builder{}.getSucsessors(CurNode);
    std::for_each(SucsRange.begin, SucsRange.end, 
                  [&CurNode, &Stream](const Node *NodePtr) {
                    Stream << "\"" << &CurNode << "\"" << " -> " << 
                              "\"" << NodePtr << "\"" << std::endl;
                  });
  };
  PrintSucsessors(Root);
  std::for_each(SecondaryNodes.begin(), SecondaryNodes.end(), 
                [&](const OwnedNode &CurNode) {
                  if constexpr(std::is_base_of_v<NamedNode, T>) {
                    Stream << "\"" << CurNode.get() << "\"" << " [label = \"" <<
                    CurNode->getName() << "\" ]" << std::endl;
                  }
                  PrintSucsessors(*CurNode.get());
                });
  Stream << "}" << std::endl;
}