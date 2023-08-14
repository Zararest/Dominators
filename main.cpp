#include <DomTreeBuilder.h>
#include <GraphBuilder.h>
#include <UserGraphBuilder.h>
#include <Utils.h>

#include <fstream>

using NodeWithDomMeta = MetadataNode<DomMetadata>;

void demo(const std::string &WorkingDir) {
  auto MainDotStream = std::ofstream{WorkingDir + "Graph.dot"};
  auto DomTreeDotStream = std::ofstream{WorkingDir + "DomTree.dot"};
  auto OrderStream = std::ofstream{WorkingDir + "RPO"};
  assert(MainDotStream.is_open() && MainDotStream.is_open() &&
         DomTreeDotStream.is_open());

  auto Root = NodeWithDomMeta::Builder{}.createNode();
  ReducibleGraphBuilder<NodeWithDomMeta> GraphBuilder{*Root};
  for (int i = 0; i < 20; i++) {
    GraphBuilder.mutate();
  }
  auto [Beg, End] = GraphBuilder.getMutableNodes();
  printGraph(Beg, End, *Root, MainDotStream);

  auto RPO = getReversePostOrder(*Root);
  for (auto &It : RPO)
    OrderStream << static_cast<NodeWithDomMeta *>(It)->getName() << " | ";
  OrderStream << std::endl;

  auto DomBuilder = DomTreeBuilder<NodeWithDomMeta>{};
  DomBuilder.createDomTree(*Root, Beg, End);
  auto [DomBeg, DomEnd] = DomBuilder.getNodes();
  auto &DomRoot = DomBuilder.getDomRoot();
  printGraph(DomBeg, DomEnd, DomRoot, DomTreeDotStream);
}

void buildUserGraph(const std::string &WorkingDir,
                    const std::string &UserGraphName) {
  auto MainDotStream = std::ofstream{WorkingDir + "UserGraph.dot"};
  auto DomTreeDotStream = std::ofstream{WorkingDir + "UserDomTree.dot"};
  auto UserGraphStream = std::ifstream(UserGraphName);
  assert(MainDotStream.is_open() && DomTreeDotStream.is_open());

  auto Builder = UserGraphBuilder<NodeWithDomMeta>{std::cerr};
  Builder.buildGraph(UserGraphStream);
  auto [OrigGraphBeg, OrigGraphEnd] = Builder.getMutableNodes();
  printGraph(OrigGraphBeg, OrigGraphEnd, Builder.getRoot(), MainDotStream);

  auto DomBuilder = DomTreeBuilder<MetadataNode<DomMetadata>>{};
  DomBuilder.createDomTree(Builder.getRoot(), OrigGraphBeg, OrigGraphEnd);
  auto [DomGraphBeg, DomGraphEnd] = DomBuilder.getNodes();
  printGraph(DomGraphBeg, DomGraphEnd, DomBuilder.getDomRoot(),
             DomTreeDotStream);
}

void nextArg(int &Argc, char **&Argv) {
  Argc--;
  Argv++;
}

int main(int Argc, char **Argv) {
  auto WorkingDir = std::string{};
#ifdef WORKING_DIR
  WorkingDir = WORKING_DIR;
#endif
  assert(Argc > 0);
  nextArg(Argc, Argv);
  auto InputGraphName = std::string{};
  auto ValidArg = false;
  while (Argc > 0) {
    auto CurArg = std::string{Argv[0]};
    if (CurArg == std::string{"--no-working-dir"}) {
      WorkingDir = "";
      ValidArg = true;
    }
    if (CurArg == std::string{"--input-graph"}) {
      if (Argc < 2) {
        std::cerr << "Error: No input graph specified" << std::endl;
        return 1;
      }
      nextArg(Argc, Argv);
      InputGraphName = Argv[0];
      ValidArg = true;
    }
    if (!ValidArg) {
      std::cerr << "Error: Invalid argument {" << Argv[0] << "}" << std::endl;
    }
    ValidArg = false;
    nextArg(Argc, Argv);
  }

  if (!InputGraphName.empty()) {
    std::cout << "---User graph---" << std::endl;
    buildUserGraph(WorkingDir, InputGraphName);
    return 0;
  }

  std::cout << "---Demo mode---" << std::endl;
  std::cout << "(All geneated files are in the Dominators/bin directory)"
            << std::endl;
  demo(WorkingDir);
}