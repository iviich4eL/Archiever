// Archiever API Reference
// ToDo: заменить ARCHIEVER_H_ на INCLUDE_ARCHIEVER_H_ в create_cpp_project
#ifndef INCLUDE_ARCHIEVER_H_
#define INCLUDE_ARCHIEVER_H_

#include "../src/HuffmanAlgorithm/HuffmanAlgorithm.h"
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

class Hello {
  public:
    void Console();
};

class ArchieverAlgorithm {
  public:
    Internal::HuffmanAlgorithm huffmanAlgorithm;
    void Compress(const std::string & fileName) { huffmanAlgorithm.Compress(fileName); }
};

} // namespace Archiever

#endif
