#ifndef BWTALGORITHM_H_
#define BWTALGORITHM_H_
#include "../BufferBlock/BufferBlock.h"
#include <cstring>
#include <fstream>
#include <set>
#include <string>
#include <vector>

namespace Archiever {
namespace Internal {

const int BLOCK_SIZE = 200000;

class BoundedCompare {
  private:
    unsigned char *buffer;
    long length;

  public:
    BoundedCompare(unsigned char *buffer, long length)
        : buffer(buffer), length(length) {}

    bool operator()(const unsigned char *p1, const unsigned char *p2) const {
        unsigned int l1 = (unsigned int)((buffer - p1) + length);
        unsigned int l2 = (unsigned int)((buffer - p2) + length);
        int result = memcmp(p1, p2, (l1 < l2 ? l1 : l2));

        if (result < 0)
            return 1;
        if (result > 0)
            return 0;
        return l1 > l2;
    }
};

class BWTAlgorithm {
  private:
    unsigned int TransformationArray[Archiever::Internal::BLOCK_SIZE + 1];
    std::set<unsigned char *, BoundedCompare> *suffixSet;

    inline void InsertDataToSuffixSet(Archiever::Internal::BufferBlock &);
    inline Archiever::Internal::BufferBlock
    CreateEncodedBlock(Archiever::Internal::BufferBlock &);
    inline void
    FillTransformationArray(Archiever::Internal::BufferBlock &bufferBlock,
                            unsigned int Count[], unsigned int RunningTotal[]);
    inline Archiever::Internal::BufferBlock
    CreateDecodedBlock(Archiever::Internal::BufferBlock &);
    inline void PrintSuffixSet(Archiever::Internal::BufferBlock &bufferBlock);

  public:
    BWTAlgorithm(Archiever::Internal::BufferBlock *bufferBlock) {
        BoundedCompare boundedCompare(bufferBlock->buffer, bufferBlock->length);
        /*
          Оно специально создаётся в конструкторе и удаляется в деструкторе,
          чтобы сократить количесво системных вызовов.
        */
        suffixSet =
            new std::set<unsigned char *, BoundedCompare>(boundedCompare);
    }
    void EncodeByRefParam(Archiever::Internal::BufferBlock &);
    void DecodeByRefParam(Archiever::Internal::BufferBlock &);
    ~BWTAlgorithm() { delete suffixSet; }
};

} // namespace Internal
} // namespace Archiever

#endif
