template <typename NodeT>
void UserGraphBuilder<NodeT>::printError(size_t ErrPosition) {
  for (size_t i = 0; i < ErrPosition - 1 && ErrPosition > 0; i++)
    Err << '~';
  Err << '^' << std::endl;
}

template <typename NodeT>
bool UserGraphBuilder<NodeT>::parseRoot(std::istream &IStream) {
  auto RootName = std::string{};
  IStream >> RootName;
  if (RootName.size() == 0) {
    Err << "Error: Empty input" << std::endl;
    return false;
  }
  if (RootName.front() != '{' || RootName.back() != '}') {
    Err << "Error: Incorrect root format:" << std::endl;
    Err << RootName << std::endl;
    auto ErrPos = RootName.front() != '{' ? 0 : RootName.size() - 1;
    printError(ErrPos);
    return false;
  }
  RootName.erase(RootName.begin(), std::next(RootName.begin()));
  RootName.erase(RootName.size() - 1, 1);
  UserRoot = typename NodeT::Builder{}.createNode();
  setName(*UserRoot, RootName);
  MapNamesToNodes[RootName] = UserRoot.get();
  return true;
}

template <typename NodeT>
bool UserGraphBuilder<NodeT>::parseRelation(std::istream &IStream,
                                            size_t LineNum) {
  auto NodeName = std::string{};
  if (!(IStream >> NodeName))
    return false;
  checkNodePresence(NodeName);

  auto RelationSymbol = std::string{};
  if (!(IStream >> RelationSymbol)) {
    Err << "Error: Can't parse '->' on line " << LineNum << std::endl;
    return false;
  }
  if (RelationSymbol != std::string{"->"}) {
    Err << "Error: Incorrect format on line " << LineNum << std::endl;
    Err << RelationSymbol << std::endl;
    printError(RelationSymbol.size());
    return false;
  }

  auto SucsessorName = std::string{};
  if (!(IStream >> SucsessorName)) {
    Err << "Error: Can't parse sucsessor name on line " << LineNum << std::endl;
    return false;
  }
  checkNodePresence(SucsessorName);
  auto *Node = MapNamesToNodes[NodeName];
  auto *Sucsessor = MapNamesToNodes[SucsessorName];
  addSucsessor(*Node, *Sucsessor);
  return true;
}

template <typename NodeT>
void UserGraphBuilder<NodeT>::checkNodePresence(const std::string &NodeName) {
  if (MapNamesToNodes.find(NodeName) == MapNamesToNodes.end()) {
    UserGraphNodes.emplace_back(
        std::move(typename NodeT::Builder{}.createNode()));
    setName(*UserGraphNodes.back(), NodeName);
    MapNamesToNodes[NodeName] = UserGraphNodes.back().get();
  }
}

template <typename NodeT>
void UserGraphBuilder<NodeT>::buildGraph(std::istream &IStream) {
  parseRoot(IStream);
  auto LineNum = 2ul;
  while (parseRelation(IStream, LineNum))
    LineNum++;
}