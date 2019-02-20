// Archiever API Reference
// ToDo: заменить ARCHIEVER_H_ на INCLUDE_ARCHIEVER_H_ в create_cpp_project
#ifndef INCLUDE_ARCHIEVER_H_
#define INCLUDE_ARCHIEVER_H_

#include "../src/BWTAlgorithm/BWTAlgorithm.h"
#include "../src/BufferBlock/BufferBlock.h"
#include "../src/HuffmanAlgorithm/HuffmanAlgorithm.h"
#include "../src/MTFAlgorithm/MTFAlgorithm.h"
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

namespace Archiever {

/*
    include/ folder contains:
    Things that need to be exposed publicly.
*/

class ArchieverAlgorithm {
  public:
    Internal::HuffmanAlgorithm huffmanAlgorithm;
    Internal::BWTAlgorithm BWTAlgorithm;
    void Compress(const std::string &fileName) {
        huffmanAlgorithm.Compress(fileName);
    }
    // void Compress2(const std::string &fileName, const size_t blockSize) {
    //     BWTAlgorithm.Compress(fileName, blockSize);
    // }

    // void Decompress2(const std::string &fileName) {
    //     BWTAlgorithm.Decompress(fileName);
    // }
};

} // namespace Archiever

#endif
