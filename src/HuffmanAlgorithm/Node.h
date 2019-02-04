#ifndef NODE_H_
#define NODE_H_

namespace Archiever {
namespace Internal {

class Node {
  public:
    char symbol;
    int frequency;
    Node *left;
    Node *right;

    Node() {}

    Node(char symbol, int frequency)
        : Node(symbol, frequency, nullptr, nullptr) {}

    Node(int frequency, Node *left, Node *right)
        : Node('$', frequency, left, right) {}

    Node(char symbol, int frequency, Node *left, Node *right)
        : symbol(symbol), frequency(frequency), left(left), right(right) {}
};

class NodeComparator {
  public:
    bool operator()(const Node *lhs, const Node *rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

class NodeComparator2 {
  public:
    bool operator()(const Node *lhs, const Node *rhs) {
        return lhs->frequency < rhs->frequency;
    }
};

} // namespace Internal
} // namespace Archiever

#endif