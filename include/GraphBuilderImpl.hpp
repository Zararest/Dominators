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
  auto &NewNodeRef = *NewNode;
  swapSucsessors(NewNodeRef, NodeToSplit);
  addSucsessor(NodeToSplit, NewNodeRef);
  SecondaryNodes.emplace_back(std::move(NewNode));
}

// FIXME: check if this will be reducible graph
// adds back edge only if node doesn't have one
template <typename T>
void ReducibleGraphBuilder<T>::addBackEdge(Node &NodeForEdge) {
  auto [SucsBeg, SucsEnd] = getSucsessors(NodeForEdge);
  auto BackEdgeNode = std::find(SucsBeg, SucsEnd, &NodeForEdge);
  if (BackEdgeNode != SucsEnd)
    return;
  addSucsessor(NodeForEdge, NodeForEdge);
}

template <typename T>
void ReducibleGraphBuilder<T>::addPathToSucsessor(Node &NodeForPath) {
  auto [SucsBeg, SucsEnd] = getSucsessors(NodeForPath);
  auto NumOfSucsessors = std::distance(SucsBeg, SucsEnd);
  if (NumOfSucsessors == 0)
    return;
  auto DuplicatePathTo = getRandomValue(0, NumOfSucsessors);
  auto NewNode = typename T::Builder{}.createNode();
  auto &NewNodeRef = *NewNode;
  addSucsessor(NodeForPath, NewNodeRef);
  auto [NewSucsBeg, _] = getMutableSucsessors(NodeForPath);
  auto SucsPtr = *(NewSucsBeg + DuplicatePathTo);
  addSucsessor(NewNodeRef, *SucsPtr);
  SecondaryNodes.emplace_back(std::move(NewNode));
}

template <typename T> void ReducibleGraphBuilder<T>::mutate() {
  auto NodesNum = SecondaryNodes.size();
  Node &NodeToMutate =
      (NodesNum == 0) ? Root : *SecondaryNodes[getRandomValue(0, NodesNum)];
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