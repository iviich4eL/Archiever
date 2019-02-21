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
    // std::cout << suffixSet->size() << std::endl;
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

inline Archiever::Internal::BufferBlock BWTAlgorithm::CreateDecodedBlock(
    Archiever::Internal::BufferBlock &bufferBlock) {

    BufferBlock encodedBufferBlock;
    encodedBufferBlock.length = bufferBlock.length - 1;

    size_t i = (size_t)bufferBlock.first;
    encodedBufferBlock.buffer[0] = bufferBlock.buffer[i];
    for (long j = 0; j < (long)encodedBufferBlock.length; j++) {
        encodedBufferBlock.buffer[j] = bufferBlock.buffer[i];
        i = TransformationArray[i];
    }

    return encodedBufferBlock;
}

inline void BWTAlgorithm::FillTransformationArray(
    Archiever::Internal::BufferBlock &bufferBlock, unsigned int Count[],
    unsigned int RunningTotal[]) {

    for (long i = 0; i < bufferBlock.length; i++) {
        int index;
        if (i == bufferBlock.last)
            index = 256;
        else
            index = bufferBlock.buffer[i];
        TransformationArray[Count[index] + RunningTotal[index]] = i;
        Count[index]++;
    }
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

    FillTransformationArray(bufferBlock, Count, RunningTotal);
    bufferBlock = CreateDecodedBlock(bufferBlock);
}

} // namespace Internal
} // namespace Archiever
