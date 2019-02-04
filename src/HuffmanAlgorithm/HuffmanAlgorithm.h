#ifndef HUFFMANALGORITHM_H_
#define HUFFMANALGORITHM_H_

#include "Node.h"
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <string>

namespace Archiever {
namespace Internal {

class HuffmanAlgorithm {
  private:
    std::map<char, int> charToFrequencyMap;
    std::list<Internal::Node *> nodeList;
    std::priority_queue<Internal::Node *, std::vector<Internal::Node *>,
                        Internal::NodeComparator>
        huffmanQueue;
    Internal::Node *root;

    std::vector<bool> code;
    std::map<char, std::vector<bool>> charToCodeMap;

    void CreateMap(std::ifstream &);
    void CreateList();
    void CreateHuffmanTree();
    void PrintHuffmanTree(Internal::Node *, unsigned);
    void BuildCodeTable(Internal::Node *);
    void PrintCodeTable();
    void CompressInputAndWriteToOutput(std::ifstream &, std::ofstream &);

  public:
    void Compress(const std::string &);
};

} // namespace Internal
} // namespace Archiever

#endif
