#ifndef MTFALGORITHM_H_
#define MTFALGORITHM_H_
#include "../BufferBlock/BufferBlock.h"
#include <string>

namespace Archiever {
namespace Internal {

class MTFAlgorithm {
  private:
    void MoveToFront(int i);
    void FillSymbolTable();
    char symbolTable[256];

  public:
    void EncodeByRefParam(Archiever::Internal::BufferBlock &);
    void DecodeByRefParam(Archiever::Internal::BufferBlock &);
};

} // namespace Internal
} // namespace Archiever

#endif