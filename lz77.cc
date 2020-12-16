#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

// find the longest match
std::pair<unsigned int, unsigned int> findLongest(char * buffer, int searchStart, int matchEnd, int idx) {
    unsigned int matchLength = matchEnd - idx;
    unsigned int searchSize = idx - searchStart;
    char * buf, * match, * space;

    /* string in which we will be looking for stuff */
    space = (char *) malloc (searchSize);
    for (int j = searchStart, k = 0; j < idx; j++, k++)
        space[k] = buffer[j];

    while (matchLength) {
        // if (buf != NULL) free(buf);
        buf = (char *) malloc(matchLength);

        /* copy the string into here */
        for (int j = 0; j < matchLength; j++)
            buf[j] = buffer[idx + j];

        char * result = (char *) strstr(space, buf);

        if (result != NULL )
            return std::make_pair(searchSize - (result - space), matchLength);
        matchLength--;
    }

    return std::make_pair(0, 0);
}


int main() {

    FILE *inputFile = fopen("input.txt", "rb");
    FILE *outputFile = fopen("output.b", "wb");

    int SEARCH_SIZE = 4095, LOOKAHEAD_SIZE = 15, fileSize;

    fseek(inputFile, 0L, SEEK_END);
    fileSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);
    char buffer[fileSize];

    // reading into memory, fix this later
    fread(buffer, 1, fileSize, inputFile);

    for (int i = 0; i < fileSize; ) {
        int searchStart = max(0, i - SEARCH_SIZE);
        int lookaheadEnd = min(fileSize, i + LOOKAHEAD_SIZE);
        char nextByte, a, b;

        std::pair result = findLongest(buffer, searchStart, lookaheadEnd, i);
        nextByte = i + result.second >= fileSize ? 0 : buffer[i + result.second];

        a = 0xFF & result.first;
        b = ((0xF00 & result.first) >> 8) | ((0xF & result.second) << 4);

        fwrite(&a, 1, 1, outputFile);
        fwrite(&b, 1, 1, outputFile);
        fwrite(&nextByte, 1, 1, outputFile);
        
        i += max(result.second, 1);
    }
    
    return 0;
}