#include "HuffmanAlgorithm.h"
#include "Node.h"
#include <cstdlib>
#include <iostream>

namespace Archiever {
namespace Internal {

/*
    src/ folder contains:
    All the source code and the header files that are for internal use only.
*/

void HuffmanAlgorithm::CreateMap(std::ifstream &inputFile) {
    inputFile >> std::noskipws;
    char symbol;

    while (inputFile >> symbol) {
        charToFrequencyMap[symbol]++;
    }

    for (auto item : charToFrequencyMap) {
        if (item.first == '\n')
            std::cout << "\'"
                      << "\\n"
                      << "\'"
                      << " : " << item.second << std::endl;
        else
            std::cout << "\'" << item.first << "\'"
                      << " : " << item.second << std::endl;
    }
}

void HuffmanAlgorithm::CreateList() {
    for (auto item : charToFrequencyMap) {
        nodeList.push_back(new Internal::Node(item.first, item.second));
        // huffmanQueue.push(new Internal::Node(item.first, item.second));
    }
}

void HuffmanAlgorithm::CreateHuffmanTree() {
    while (nodeList.size() != 1) {
        nodeList.sort(Internal::NodeComparator2());

        Internal::Node *nodeLeft = nodeList.front();
        nodeList.pop_front();

        Internal::Node *nodeRight = nodeList.front();
        nodeList.pop_front();

        int frequency = nodeLeft->frequency + nodeRight->frequency;

        Internal::Node *parent =
            new Internal::Node(frequency, nodeLeft, nodeRight);

        nodeList.push_back(parent);
    }

    root = nodeList.front();
}

void HuffmanAlgorithm::PrintHuffmanTree(Internal::Node *root,
                                        unsigned space = 0) {
    if (root != nullptr) {
        PrintHuffmanTree(root->left, space + 3);
        for (unsigned i = 0; i < space; i++)
            std::cout << "  ";

        if (root->symbol != '$')
            std::cout << root->frequency << " (" << root->symbol << ")"
                      << std::endl;
        else
            std::cout << root->frequency << std::endl;
        PrintHuffmanTree(root->right, space + 3);
    }
}

void HuffmanAlgorithm::BuildCodeTable(Internal::Node *root) {
    if (root->left != nullptr) {
        code.push_back(0);
        BuildCodeTable(root->left);
    }

    if (root->right != nullptr) {
        code.push_back(1);
        BuildCodeTable(root->right);
    }

    if (root->symbol && root->symbol != '$')
        charToCodeMap[root->symbol] = code;

    code.pop_back();
}

void HuffmanAlgorithm::PrintCodeTable() {
    for (auto item : charToCodeMap) {
        if (item.first == '\n')
            std::cout << "\'"
                      << "\\n"
                      << "\'"
                      << " : ";
        else
            std::cout << "\'" << item.first << "\'"
                      << " : ";
        for (auto code : item.second)
            std::cout << code;
        std::cout << std::endl;
    }
}

void HuffmanAlgorithm::CompressInputAndWriteToOutput(
    std::ifstream &inputFile, std::ofstream &outputFile) {

    int count = 0;
    // char symbol;
    char byte = 0;

    // while (inputFile >> symbol) {
    //     std::vector<bool> cachedSymbol = charToCodeMap[symbol];

    //     // for (auto number : cachedSymbol)
    //     //     std::cout << number;

    //     for (int i = 0; i < cachedSymbol.size(); i++) {
    //         byte = byte | cachedSymbol[i] << (7 - count);
    //         count++;

    //         if (count == 8) {
    //             count = 0;
    //             outputFile << byte;
    //             byte = 0;
    //         }
    //     }
    // }

    while (!inputFile.eof()) {
        char symbol;
        inputFile >> symbol;
        std::vector<bool> cachedSymbol = charToCodeMap[symbol];

        for (int i = 0; i < cachedSymbol.size(); i++) {
            byte = byte | cachedSymbol[i] << (7 - count);
            count++;

            if (count == 8) {
                count = 0;
                outputFile << byte;
                byte = 0;
            }
        }
    }
}

void HuffmanAlgorithm::Compress(const std::string &fileName) {
    {
        std::ifstream inputFile(fileName);

        CreateMap(inputFile);
        CreateList();
        CreateHuffmanTree();
        PrintHuffmanTree(root);
        BuildCodeTable(root);
        PrintCodeTable();

        inputFile.clear();
        inputFile.seekg(0);
        std::ofstream outputFile(fileName + ".bin");

        CompressInputAndWriteToOutput(inputFile, outputFile);
    }
    // std::cout << std::endl;
    std::ifstream inputFile(fileName + ".bin");
    Internal::Node *node = root;
    int count = 0;
    char byte;
    // алгоритм не доарзивирует строку
    while (inputFile >> byte) {

        for (int count = 0; count < 8; count++) {
            bool bit = byte & (1 << 7 - count);

            if (bit)
                node = node->right;
            else
                node = node->left;

            if (node->left == nullptr && node->right == nullptr) {
                std::cout << node->symbol;
                node = root;
            }
        }
    } 

    // std::cout.clear();
    // inputFile >> byte;
    // while (!inputFile.eof()) {
    //     bool bit = byte & (1 << 7 - count);

    //     if (bit)
    //         node = node->right;
    //     else
    //         node = node->left;

    //     if (node->left == nullptr && node->right == nullptr) {
    //         std::cout << node->symbol;
    //         node = root;
    //     }
    //     count++;
    //     if (count == 8) {
    //         count = 0;
    //         inputFile >> byte;
    //     }
    // }
    inputFile.close();
    // std::ifstream outputFile(fileName + ".bin");
    // outputFile >> std::noskipws;
}

} // namespace Internal
} // namespace Archiever

// while (huffmanQueue.size() != 1) {
//     std::cout << huffmanQueue.top()->frequency << " :: ";
//     std::cout << huffmanQueue.top()->symbol << std::endl;
//     huffmanQueue.pop();
// }
// std::cout << std::endl;
// nodeList.sort(Internal::NodeComparator2());
// for (auto i : nodeList) {
//     std::cout << i->frequency << " :: ";
//     std::cout << i->symbol << std::endl;
// }

// while (huffmanQueue.size() != 1) {
//     // nodeList.sort(Internal::PtrNodeCompare());

//     Internal::Node *nodeLeft = huffmanQueue.top();
//     huffmanQueue.pop();

//     Internal::Node *nodeRight = huffmanQueue.top();
//     huffmanQueue.pop();

//     int frequency = nodeLeft->frequency + nodeRight->frequency;

//     Internal::Node *parent =
//         new Internal::Node(frequency, nodeLeft, nodeRight);

//     huffmanQueue.push(parent);
// }
// root = huffmanQueue.top();
