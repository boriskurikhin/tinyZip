#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

// find the longest match
std::pair<int, int> findLongest(char * buffer, int searchStart, int matchEnd, int idx) {
    int matchLength = matchEnd - idx;
    int searchSize = idx - searchStart;

    std::string space = "", match = "";

    /* string in which we will be looking for stuff */
    for (int i = searchStart; i < idx; i++)
        space += buffer[i];
    
    /* what we're looking for */
    for (int i = idx; i < matchEnd; i++)
        match += buffer[i];

    while (match.size()) {
        int found = space.rfind(match);
        if (found != std::string::npos)
            return std::make_pair(searchSize - found, match.size());
        match.pop_back();
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