#include "BWTAlgorithm.h"
#include <cstdlib>
#include <iostream>

namespace Archiever {
namespace Internal {

inline void BWTAlgorithm::InsertDataToSuffixSet(
    Archiever::Internal::BufferBlock &bufferBlock) {

    unsigned char *buffer = bufferBlock.buffer;

    for (int i = 0; i <= bufferBlock.length; i++) {
        suffixSet->insert(&buffer[0] + i);
    }
    //std::cout << suffixSet->size() << std::endl;
}

void BWTAlgorithm::PrintSuffixSet(
    Archiever::Internal::BufferBlock &bufferBlock) {

    unsigned char *buffer = bufferBlock.buffer;
    long length = bufferBlock.length;
    int i;

    std::set<unsigned char *, BoundedCompare>::iterator suffixSetIterator;
    for (suffixSetIterator = suffixSet->begin(), i = 0;
         suffixSetIterator != suffixSet->end(); suffixSetIterator++, i++) {

        unsigned char *s = *suffixSetIterator;
        fprintf(stderr, "%d : ", i);
        unsigned char prefix;

        if (s == &buffer[0])
            prefix = '?';
        else
            prefix = *(s - 1);
        // prefix = s[-1];

        if (isprint(prefix))
            fprintf(stdout, "%c", prefix);
        else
            fprintf(stdout, "<%d>", prefix);

        fprintf(stdout, ": ");
        int stop = (int)((&buffer[0] - s) + length);
        if (stop > 30)
            stop = 30;

        if (stop == 0) // Имрользуется для вывода конечного символа
            fprintf(stdout, "?");

        for (int j = 0; j < stop; j++) {

            if (isprint(*s))
                fprintf(stdout, "%c", *s);
            else
                fprintf(stdout, "<%d>", *s);
            s++;
        }

        fprintf(stdout, "\n");
    }
}

inline Archiever::Internal::BufferBlock BWTAlgorithm::CreateEncodedBlock(
    Archiever::Internal::BufferBlock &bufferBlock) {

    BufferBlock encodedBufferBlock;
    encodedBufferBlock.length = suffixSet->size();

    int i;
    std::set<unsigned char *, BoundedCompare>::iterator suffixSetIterator;

    for (suffixSetIterator = suffixSet->begin(), i = 0;
         suffixSetIterator != suffixSet->end(); suffixSetIterator++, i++) {
        if (*suffixSetIterator == (&bufferBlock.buffer[0] + 1))
            encodedBufferBlock.first = i;

        if (*suffixSetIterator == &bufferBlock.buffer[0]) {
            encodedBufferBlock.last = i;
            encodedBufferBlock.buffer[i] = '?';
        }

        else
            encodedBufferBlock.buffer[i] = (*suffixSetIterator)[-1];
    }

    return encodedBufferBlock;
}

void BWTAlgorithm::EncodeByRefParam(
    Archiever::Internal::BufferBlock &bufferBlock) {

    InsertDataToSuffixSet(bufferBlock);
  //  PrintSuffixSet(bufferBlock);
    // bufferBlock.Print();

    bufferBlock = CreateEncodedBlock(bufferBlock);
    suffixSet->erase(suffixSet->begin(), suffixSet->end());
}

void BWTAlgorithm::DecodeByRefParam(
    Archiever::Internal::BufferBlock &bufferBlock) {

    unsigned int Count[257];
    unsigned int RunningTotal[257];
    unsigned int i;

    for (i = 0; i < 257; i++)
        Count[i] = 0;

    for (long i = 0; i < bufferBlock.length; i++)
        if (i == bufferBlock.last)
            Count[256]++;
        else
            Count[bufferBlock.buffer[i]]++;

    int sum = 0;
    for (long i = 0; i < 257; i++) {
        RunningTotal[i] = sum;
        sum += Count[i];
        // if (Count[i]) {
        //     if (isprint(i))
        //         fprintf(stderr, "%c  : ", i);
        //     else
        //         fprintf(stderr, "%3d: ", i);
        //     fprintf(stderr, "%u\n", (long)RunningTotal[i]);
        // }
        Count[i] = 0;
    }

    for (long i = 0; i < bufferBlock.length; i++) {
        int index;
        if (i == bufferBlock.last)
            index = 256;
        else
            index = bufferBlock.buffer[i];
        T[Count[index] + RunningTotal[index]] = i;
        Count[index]++;
    }

    i = (size_t)bufferBlock.first;
    for (size_t j = 0; j < (size_t)(bufferBlock.length - 1); j++) {
       // ToDo: дописать присваивание bufferBlock
       putc(bufferBlock.buffer[i], stdout);
        i = T[i];
    }
}

// void BWTAlgorithm::Decode(Archiever::Internal::BufferBlock &bufferBlock) {
//     unsigned int Count[257];
//     unsigned int RunningTotal[257];
//     unsigned char order[256];

//     // std::fstream inputFile(fileName, std::ios::in | std::ios::binary);

//     while (ReadCompressedBlock(inputFile)) {
//         // InsertDataToSuffixSet();
//         // PrintSuffixSet();

//         // if (!WriteCompressedBlock(std::cout))
//         // break;
//         // std::cout << std::endl;

//         unsigned int i;
//         for (i = 0; i < 257; i++)
//             Count[i] = 0;
//         for (i = 0; i < (unsigned int)length; i++)
//             if (i == last)
//                 Count[256]++;
//             else
//                 Count[buffer[i]]++;

//         fprintf(stderr, "Byte Counts:\n");
//         for (i = 0; i <= 256; i++) {
//             if (Count[i]) {
//                 if (isprint(i))
//                     fprintf(stderr, "%c  : ", i);
//                 else
//                     fprintf(stderr, "%3d: ", i);
//                 fprintf(stderr, "%u\n", Count[i]);
//             }
//         }

//         int sum = 0;
//         for (i = 0; i < 257; i++) {
//             RunningTotal[i] = sum;
//             sum += Count[i];
//             if (Count[i]) {
//                 if (isprint(i))
//                     fprintf(stderr, "%c  : ", i);
//                 else
//                     fprintf(stderr, "%3d: ", i);
//                 fprintf(stderr, "%u\n", (long)RunningTotal[i]);
//             }
//             Count[i] = 0;
//         }

//         for (i = 0; i < (unsigned int)length; i++) {
//             int index;
//             if (i == last)
//                 index = 256;
//             else
//                 index = buffer[i];
//             T[Count[index] + RunningTotal[index]] = i;
//             Count[index]++;
//         }
//         fprintf(stderr, "T = ");
//         for (i = 0; i < (unsigned int)length; i++)
//             fprintf(stderr, "%u ", T[i]);
//         fprintf(stderr, "\n");

//         unsigned int j;
//         i = (unsigned int)first;
//         for (j = 0; j < (unsigned int)(length - 1); j++) {
//             putc(buffer[i], stdout);
//             i = T[i];
//         }

//         suffixSet.erase(suffixSet.begin(), suffixSet.end());
//     }
// }

// inline Archiever::Internal::BufferBlock &BWTAlgorithm::CreateCompressedBlock(
//     Archiever::Internal::BufferBlock &BufferBlock) {
//     long length = suffixSet->size(); // - 1; // потеря точности
//     if (!output.write((char *)&length, sizeof(long)))
//         return false;

//     int i;
//     std::set<unsigned char *, BoundedCompare>::iterator suffixSetIterator;
//     for (i = 0, suffixSetIterator = suffixSet.begin(); /* i != length;*/
//          suffixSetIterator != suffixSet.end(); i++, suffixSetIterator++) {
//         if (*suffixSetIterator == (&buffer[0] + 1))
//             first = i; // i - 1; // потому что нумерация идет с 0 и
//                        // мы не учитываем 0 элемент set
//         if (*suffixSetIterator == &buffer[0]) {
//             last = i;
//             // if (!output.write((char *)&(buffer[length - 1]),
//             sizeof(char)))
//             // return false;

//             // fputc('?', stdout);
//             // fputc(buffer[buffer.size() - 1], stdout);
//             // if (!output.write((char *)&buffer[buffer.size() - 1],
//             // sizeof(char))) return false;
//             char q = '?';
//             if (!output.write((char *)&q, sizeof(char)))
//                 return false;

//         } else if (i != length) {
//             if (!output.write((char *)&((*suffixSetIterator)[-1]),
//                               sizeof(char)))
//                 return false;
//         }
//     }

//     if (!output.write((char *)&first, sizeof(long)) ||
//         !output.write((char *)&last, sizeof(long)))
//         return false;

//     return true;
// }

// inline bool BWTAlgorithm::ReadCompressedBlock(std::istream &input) {
//     if (!input.read((char *)&length, sizeof(long)))
//         return false;

//     std::cout << length;
//     if (!input.read((char *)&buffer[0], length)) {
//         fprintf(stderr, "Detected corrupted data");
//         return false;
//     }

//     for (int i = 0; i < length; i++)
//         std::cout << buffer[i];
//     // std::cout << buffer;

//     if (!input.read((char *)&first, sizeof(long))) {
//         fprintf(stderr, "Detected corrupted data");
//         return false;
//     }

//     if (!input.read((char *)&last, sizeof(long))) {
//         fprintf(stderr, "Detected corrupted data");
//         return false;
//     }
//     std::cout << first << last << std::endl;
//     return true;
// }

// void BWTAlgorithm::Compress(const std::string &fileName,
// const size_t blockSize) {
// std::ifstream inputFile(fileName, std::ios::in | std::ios::binary);
// std::string fileName2 = "output_bwt.txt";
// std::ofstream outputFile(fileName2, std::ios::out | std::ios::binary);

// while (ReadDataToBuffer(inputFile, blockSize)) {

//     InsertDataToSuffixSet();
//     PrintSuffixSet();

//     if (!WriteCompressedBlock(std::cout))
//         break;
//     std::cout << std::endl;
//     if (!WriteCompressedBlock(outputFile))
//         break;

//     suffixSet.erase(suffixSet.begin(), suffixSet.end());
// }
//} // namespace Internal

} // namespace Internal
} // namespace Archiever
