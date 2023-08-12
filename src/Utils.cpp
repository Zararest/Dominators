#include <Utils.h>

#include <cassert>
#include <unordered_set>

std::vector<Node *> getReversePostOrder(Node &Root) {
  auto Visited = std::unordered_set<Node *>{};
  auto Postorder = std::vector<Node *>{};
  auto NodesTrace = std::vector<Node *>{&Root};
  while (!NodesTrace.empty()) {
    auto [_, Inserted] = Visited.insert(NodesTrace.back());
    assert(NodesTrace.back());
    auto [Beg, End] = Node::Builder{}.getSucsessors(*NodesTrace.back());
    auto FoundUnvisited = false;
    for (; Beg != End; Beg++)
      if (Visited.find(*Beg) == Visited.end()) {
        NodesTrace.push_back(*Beg);
        FoundUnvisited = true;
        break;
      }
    if (!FoundUnvisited) {
      Postorder.push_back(NodesTrace.back());
      NodesTrace.pop_back();
    }
  }
  return {Postorder.rbegin(), Postorder.rend()};
}
