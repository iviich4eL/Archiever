#include "BufferBlock.h"
#include <iostream>

namespace Archiever {
namespace Internal {

bool BufferBlock::ReadBlockFromStream(std::istream &input,
                                      const size_t blockSize) {
    input.read((char *)&buffer[0], blockSize);
    length = input.gcount();
    if (length == 0)
        return false;

    return true;
}
bool BufferBlock::ReadCompressedBlock(std::istream &input) {
    if (!input.read((char *)&length, sizeof(long)))
        return false;

    std::cout << length;
    if (!input.read((char *)&buffer[0], length)) {
        fprintf(stderr, "Detected corrupted data");
        return false;
    }
    // buffer.reserve(length);
    // buffer.assign(std::istreambuf_iterator<char>(input),
    //   std::istreambuf_iterator<char>());

    // fileContents.assign(std::istreambuf_iterator<char>(testFile),
    // std::istreambuf_iterator<char>());
    //for (int i = 0; i < length; i++)
      //  std::cout << (buffer[i]);
    // std::cout << buffer;

    if (!input.read((char *)&first, sizeof(long))) {
        fprintf(stderr, "Detected corrupted data");
        return false;
    }

    if (!input.read((char *)&last, sizeof(long))) {
        fprintf(stderr, "Detected corrupted data");
        return false;
    }
    //std::cout << first << last << std::endl;
    return true;
}

bool BufferBlock::WriteCompressedBlock(std::ostream &output) {
    if (!output.write((char *)&length, sizeof(long)))
        return false;

    if (!output.write((char *)&first, sizeof(long)) ||
        !output.write((char *)&last, sizeof(long)))
        return false;

    return true;
}

} // namespace Internal
} // namespace Archiever