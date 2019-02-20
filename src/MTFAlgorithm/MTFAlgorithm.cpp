#include "MTFAlgorithm.h"
#include <iostream>
namespace Archiever {
namespace Internal {

void MTFAlgorithm::MoveToFront(int i) {
    char t = symbolTable[i];
    for (int z = i - 1; z >= 0; z--)
        symbolTable[z + 1] = symbolTable[z];

    symbolTable[0] = t;
}

void MTFAlgorithm::FillSymbolTable() {
    for (int x = 0; x < 256; x++)
        symbolTable[x] = x;
}

void MTFAlgorithm::EncodeByRefParam(
    Archiever::Internal::BufferBlock &bufferBlock) {
    FillSymbolTable();

    for (int c = 0, count = 0; c < bufferBlock.length; c++, count++) {
        for (int i = 0; i < 256; i++) {
            if (bufferBlock.buffer[c] == symbolTable[i]) {
                bufferBlock.buffer[count] = i;
                MoveToFront(i);
                break;
            }
        }
    }
}

void MTFAlgorithm::DecodeByRefParam(
    Archiever::Internal::BufferBlock &bufferBlock) {
    FillSymbolTable();

    for (int i = 0; i < bufferBlock.length; i++) {
        MoveToFront(bufferBlock.buffer[i]);
        bufferBlock.buffer[i] = symbolTable[0];
    }
}

// std::string
// MTFAlgorithm::Decode2(Archiever::Internal::BufferBlock &bufferBlock) {
//     FillSymbolTable();
//     std::vector<char> output = bufferBlock.buffer;
//     std::string r = "";
//     std::cout << "output.size(): " << output.size() << std::endl;
//     std::cout << "bufferBlock.length: " << bufferBlock.length << std::endl;

//     unsigned char cache;
//     for (int i = 0; i < bufferBlock.length; i++) {
//         // r.append(1, symbolTable[bufferBlock.buffer[i]]);
//         // cache = symbolTable[bufferBlock.buffer[i]];
//         MoveToFront(bufferBlock.buffer[i]);
//         bufferBlock.buffer[i] = symbolTable[0];
//         // bufferBlock.buffer[i] = cache;
//     }

//     for (int i = 0; i < bufferBlock.length; i++) {
//         std::cout << static_cast<int>(bufferBlock.buffer[i]) << " ";
//     }
//     std::cout << std::endl;
//     return r;
// }

} // namespace Internal
} // namespace Archiever
