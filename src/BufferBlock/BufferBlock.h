#ifndef BUFFERBLOCK_H
#define BUFFERBLOCK_H
#include <fstream>
#include <iostream>
#include <vector>

namespace Archiever {
namespace Internal {

// const int BLOCK_SIZE = 200000;

class BufferBlock {
  public:
    unsigned char buffer[20000 + 1];
    // std::vector<char> buffer; //(20000 + 1);
    long length;
    long first;
    long last;

    BufferBlock() {
        // buffer.reserve(2000);
        first = last = length = 0;
    }
    bool ReadBlockFromStream(std::istream &input, const size_t);
    bool ReadCompressedBlock(std::istream &);
    bool WriteCompressedBlock(std::ostream &);
    void Print() {
        std::cout << length;
        for (int i = 0; i < length; i++)
            std::cout << (buffer[i]);
        std::cout << first << last << std::endl;
    }
};
} // namespace Internal
} // namespace Archiever

#endif