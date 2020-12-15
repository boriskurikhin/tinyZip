
# find the longest match in the running window
def findLongestMatch(searchSpace: bytearray, matchBytes: bytearray) -> tuple:
    matchLength = len(matchBytes)
    while matchLength > 0:
        lookup = matchBytes[:matchLength]
        if lookup in searchSpace:
            return searchSpace.index(lookup), matchLength
        matchLength -= 1
    return 0, 0
    
def lz77 (inp: bytearray) -> bytearray:
    index, output, SEARCH_SIZE, LOOKAHEAD_SIZE = 0, bytearray(), 4095, 15

    while index < len(inp):
        searchStart = max(0, index - SEARCH_SIZE)
        lookaheadEnd = min(len(inp), index + LOOKAHEAD_SIZE)

        # run the matching algorithm
        matchIndex, matchLength = findLongestMatch(inp[searchStart: index], inp[index: lookaheadEnd])
        # relative offset
        relativePos = index - matchIndex
        # next character
        nextChar = 0 if index + matchIndex >= len(inp) else inp[index + matchIndex]
        # pack into 3 bytes
        output.append(0x000000FF & relativePos) # 8 bits of relative position
        output.append((0x00000F00 & relativePos) >> 8 | (0x0000000F & matchLength) << 4) # 4 bits of position and 4 bits of length
        output.append(0xFFFFFFFF & nextChar) # 8 bits of next character
        index += max(1, matchLength)
    return output    


k = lz77(bytearray('hello hello', 'utf-8'))
print(k)
# print(unlz77(k))