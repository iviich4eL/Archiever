#include "include/Archiever.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <thread>
#include <vector>

using Archiever::ArchieverAlgorithm;
using Archiever::Internal::BufferBlock;
using Archiever::Internal::BWTAlgorithm;
using Archiever::Internal::MTFAlgorithm;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::map;
using std::ofstream;
using std::priority_queue;
using std::set;
using std::string;
using std::vector;

int main(int argc, char const *argv[]) {

    /*
        Write code, which uses API from "include/Archiever.h", here.
    */

    BufferBlock *bufferBlock = new BufferBlock();
    std::fstream inputFile("input_bwt.txt", std::ios::in | std::ios::binary);
    while (bufferBlock->ReadBlockFromStream(inputFile, 7)) {
        // bufferBlock->Print();

        BWTAlgorithm bWTAlgorithm(bufferBlock);
        bWTAlgorithm.EncodeByRefParam(*bufferBlock);
        // bufferBlock->Print();

        MTFAlgorithm mTFAlgorithm;
        mTFAlgorithm.EncodeByRefParam(*bufferBlock);
        // bufferBlock->Print();

        mTFAlgorithm.DecodeByRefParam(*bufferBlock);
        // bufferBlock->Print();

        bWTAlgorithm.DecodeByRefParam(*bufferBlock);
        bufferBlock->Print();
    }
    // bufferBlock->Print();
    
    delete bufferBlock;
    return 0;
}

// while (!huffmanQueue.empty()) {
//         cout << huffmanQueue.top()->frequency << " :: \'"
//              << huffmanQueue.top()->symbol << "\'" << endl;
//         huffmanQueue.pop();
//     }

// for (int i = 0; i < bufferBlock.length; i++) {
//     std::cout << static_cast<int>(bufferBlock.buffer[i]) << " ";
// }
// std::cout << std::endl;

// for (int i = 0; i < bufferBlock.length; i++) {
// std::cout << static_cast<int>(bufferBlock.buffer[i]) << " ";
// }
// std::cout << std::endl;

// MTFAlgorithm mTFAlgorithm;
// for (int i = 0; i < bufferBlock.length; i++) {
//     std::cout << static_cast<char>(bufferBlock.buffer[i]);
// }
// std::cout << std::endl;