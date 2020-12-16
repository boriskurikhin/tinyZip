#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

std::string data;

/* KMP */
int fastSearch (std::string X, std::string Y) {
    int m = X.length();
    int n = Y.length();

    if (n == 0) return -1;
    if (m < n) return -1;

    // next[i] stores the index of next best partial match
    int next[n + 1];

    for (int i = 0; i < n + 1; i++)
        next[i] = 0;

    for (int i = 1; i < n; i++) {
        int j = next[i + 1];

        while (j > 0 && Y[j] != Y[i])
            j = next[j];

        if (j > 0 || Y[j] == Y[i])
            next[i + 1] = j + 1;
    }

    for (int i = 0, j = 0; i < m; i++) {
        if (X[i] == Y[j]) {
            if (++j == n)
                return i - j + 1;
        }
        else if (j > 0) {
            j = next[j];
            i--; // since i will be incremented in next iteration
        }
    }
    return -1;
}

// find the longest match
std::pair<int, int> findLongest(char *buffer, int matchEnd, int idx)
{
    int matchLength = matchEnd - idx;
    // int searchSize = idx - searchStart;

    // std::string space = data.substr(searchStart, searchSize), match = "";
    std::string match = "";

    /* string in which we will be looking for stuff */
    // for (int i = searchStart; i < idx; i++)
    //     space += buffer[i];

    /* what we're looking for */
    for (int i = idx; i < matchEnd; i++)
        match += buffer[i];

    while (match.size())
    {
        int found = fastSearch(data, match);
        if (found >= 0)
            return std::make_pair(data.size() - found, match.size());
        match.pop_back();
    }

    return std::make_pair(0, 0);
}

int main()
{

    FILE *inputFile = fopen("input.txt", "rb");
    FILE *outputFile = fopen("output.b", "wb");

    int SEARCH_SIZE = 4095, LOOKAHEAD_SIZE = 15, fileSize;

    fseek(inputFile, 0L, SEEK_END);
    fileSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);
    char buffer[fileSize];

    // reading into memory, fix this later
    fread(buffer, 1, fileSize, inputFile);
    data = "";

    int j = 0;
    for (int i = 0; i < fileSize;)
    {
        // int searchStart = max(0, i - SEARCH_SIZE);
        int lookaheadEnd = min(fileSize, i + LOOKAHEAD_SIZE);
        char nextByte, a, b;

        if (data.size() > SEARCH_SIZE)
            data.erase(0, 1);

        std::pair result = findLongest(buffer, lookaheadEnd, i);
        // std::cout << i << std::endl;
        nextByte = i + result.second >= fileSize ? 0 : buffer[i + result.second];

        a = 0xFF & result.first;
        b = ((0xF00 & result.first) >> 8) | ((0xF & result.second) << 4);

        fwrite(&a, 1, 1, outputFile);
        fwrite(&b, 1, 1, outputFile);
        fwrite(&nextByte, 1, 1, outputFile);

        // result.second = 
        int to_add = result.second;

        while (to_add--)
            data += buffer[j++];
        
        i += max(result.second, 1);
    }

    return 0;
}