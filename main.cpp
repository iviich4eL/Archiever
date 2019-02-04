#include "include/Archiever.h"
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <thread>
#include <vector>

using Archiever::ArchieverAlgorithm;
using std::cout;
using std::endl;
using std::greater;
using std::ifstream;
using std::list;
// using std::lock_guard;
using std::map;
// using std::move;
// using std::mutex;
using std::ofstream;
using std::priority_queue;
using std::string;
using std::thread;
using std::vector;

int main(int argc, char const *argv[]) {
    // std::cout << "Hello world!" << std::endl;
    // Archiever::Hello *hello = new Archiever::Hello();
    // hello->Console();
    // delete hello;

    /*
        Write code, which uses API from "include/Archiever.h", here.
    */

    ArchieverAlgorithm archieverAlgorithm;
    archieverAlgorithm.Compress("input.txt");

    // char c = 65;
    // // c = c | 65;
    // c = c | (1 << 6);
    // cout << c << " - " << int(c) << endl;
    return 0;
}

// while (!huffmanQueue.empty()) {
//         cout << huffmanQueue.top()->frequency << " :: \'"
//              << huffmanQueue.top()->symbol << "\'" << endl;
//         huffmanQueue.pop();
//     }