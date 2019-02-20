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

/*
    Huffman EOF -- особая константа,
    необходимая для правильной записи/считывания последнего байта в файле.
*/

const char H_EOF = 0;

void HuffmanAlgorithm::CreateMap(std::fstream &inputFile) {
    inputFile >> std::noskipws;
    char symbol;

    while (inputFile >> symbol) {
        charToFrequencyMap[symbol]++;
    }
    charToFrequencyMap[H_EOF]++;

    for (auto item : charToFrequencyMap) {
        if (item.first == H_EOF)
            std::cout << "\'"
                      << "H_EOF"
                      << "\'"
                      << " : " << item.second << std::endl;
        else if (item.first == '\n')
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

        if (root->symbol == H_EOF)
            std::cout << root->frequency << " ( H_EOF )" << std::endl;
        else if (root->symbol != '$')
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

    if (root->symbol == H_EOF && root->symbol != '$')
        charToCodeMap[root->symbol] = code;

    code.pop_back();
}

void HuffmanAlgorithm::PrintCodeTable() {
    for (auto item : charToCodeMap) {
        if (item.first == H_EOF)
            std::cout << "\'"
                      << "H_EOF"
                      << "\'"
                      << " : ";
        else if (item.first == '\n')
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
    std::fstream &inputFile, std::ofstream &outputFile) {
    int count = 0;
    char byte = 0;

    while (!inputFile.eof()) {
        char symbol = inputFile.get();
        std::vector<bool> cachedSymbol = charToCodeMap[symbol];

        for (auto number : cachedSymbol)
            std::cout << number;
        std::cout << " ";

        for (size_t i = 0; i < cachedSymbol.size(); i++) {
            byte = byte | cachedSymbol[i] << (7 - count);
            count++;

            if (count == 8) {
                count = 0;
                outputFile << byte;
                byte = 0;
            }
        }
    }
    // std::cout << "count: " << count << std::endl;
    /*
        В std::cout и std::stream можно записывать только по 8 бит.
        Алгоритм Хаффмана представляет каждый символ в виде уникального кода,
        который на маленьком алфавите вряд ли достигнет 8 бит.
        Это может привести к всего лишь частичному заполнению последнего байта в
        файле. Его оставшаяся часть будет представлять из себя бесполезную
        информацию. Чтобы этого избежать необходимо использовать псевдоконец файла.
    */
    std::vector<bool> cachedSymbol = charToCodeMap[H_EOF];
    int i = 0;
    while (count != 8) {
        byte = byte | cachedSymbol[i++] << (7 - count);
        count++;
    }
    outputFile << byte;
    // std::cout << "i : " << i << std::endl;
}

void HuffmanAlgorithm::Compress(const std::string &fileName) {
    std::fstream inputFile(fileName);
    {

        CreateMap(inputFile);

        CreateList();
        std::cout << std::endl;

        CreateHuffmanTree();

        PrintHuffmanTree(root);
        std::cout << std::endl;

        BuildCodeTable(root);

        PrintCodeTable();
        std::cout << std::endl;

        inputFile.clear();
        inputFile.seekg(0);

        std::ofstream outputFile(fileName + ".bin");

        CompressInputAndWriteToOutput(inputFile, outputFile);
        std::cout << std::endl;
        std::cout << std::endl;
    }
    // std::cout << std::endl;
    std::ifstream F(fileName + ".bin", std::ios::in | std::ios::binary);
    Internal::Node *node = root;
    // int count = 0;
    // алгоритм не доархивирует строку

    Node *p = root;
    int count = 0;
    char byte;
    byte = F.get();
    while (!F.eof()) {
        bool b = byte & (1 << (7 - count));
        if (b)
            p = p->right;
        else
            p = p->left;
        if (p->left == NULL && p->right == NULL) {
            if (p->symbol == H_EOF) {
                break;
            }
            std::cout << p->symbol;
            p = root;
        }
        count++;
        if (count == 8) {
            count = 0;
            byte = F.get();
        }
    }
    F.close();
    // while (inputFile >> byte) {

    //     for (int count = 0; count < 8; count++) {
    //         bool bit = byte & (1 << (7 - count));

    //         if (bit)
    //             node = node->right;
    //         else
    //             node = node->left;

    //         if (node->left == nullptr && node->right == nullptr) {
    //             std::cout << node->symbol;
    //             node = root;
    //         }
    //     }
    // }

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
