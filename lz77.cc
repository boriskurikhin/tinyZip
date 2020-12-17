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

void computeLPSArray(std::string pat, int* lps);

int KMP (std::string pat, std::string txt)  { 
    int M = pat.size();
    int N = txt.size(); 
    int lps[M]; 
  
    computeLPSArray(pat, lps); 
  
    int i = 0; // index for txt[] 
    int j = 0; // index for pat[] 
    while (i < N) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
  
        if (j == M)
            return i - j;
  
        else if (i < N && pat[j] != txt[i]) { 
            if (j != 0) j = lps[j - 1]; 
            else i = i + 1; 
        } 
    }
    return -1;
} 
  
void computeLPSArray(std::string pat, int* lps)  { 
    int len = 0; 
    lps[0] = 0;
    int i = 1; 
    while (i < pat.size()) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else { 
             if (len != 0) { 
                len = lps[len - 1]; 
            } else { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 

// find the longest match
std::pair<int, int> findLongest(char *buffer, int SEARCH_SIZE, int LOOKAHEAD_SIZE, int idx, int N) {

    // this is what we will look for
    std::string find = std::string(
        buffer + idx, 
        idx + LOOKAHEAD_SIZE >= N ? N - idx : LOOKAHEAD_SIZE
    );

    //construct the search space string
    //might have to re-do this at some point
    std::string search = std::string(
        buffer + max(0, (idx - SEARCH_SIZE)),
        idx - max(0, (idx - SEARCH_SIZE))
    );


    //since we write 3 bytes per match, we will look for words 
    //that have length > 3

    while (find.size()) {
        int matchIndex = KMP(find, search);
        if (matchIndex != -1) {
            // std::cout << matchIndex << std::endl;
            return std::make_pair(search.size() - matchIndex, find.size());
        }
        find.pop_back();
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

    int i = 0, j = 0;
    while (i < fileSize) {
        char nextByte, a, b;

        std::pair<int, int> result = findLongest(buffer, SEARCH_SIZE, LOOKAHEAD_SIZE, i, fileSize);
        // std::cout << i << std::endl;
        nextByte = i + result.second >= fileSize ? 0 : buffer[i + result.second];

        a = 0xFF & result.first;
        b = ((0xF00 & result.first) >> 8) | ((0xF & result.second) << 4);

        fwrite(&a, 1, 1, outputFile);
        fwrite(&b, 1, 1, outputFile);
        // std::cout << b << std::endl;
        fwrite(&nextByte, 1, 1, outputFile);
        
        i += max(result.second, 1);
    }

    return 0;
}