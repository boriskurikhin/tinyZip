
# find the longest match in the running window
def findLongestMatch(searchSpace: bytes, matchBytes: bytes) -> tuple:
    matchLength = len(matchBytes)
    searchSize = len(searchSpace)
    while matchLength > 0:
        lookup = matchBytes[:matchLength]
        if lookup in searchSpace:
            return searchSize - searchSpace.rindex(lookup), matchLength
        matchLength -= 1
    return 0, 0

# convert lz77 compressed file back into bytes
def lz77rev (inp: bytes) -> bytes:
    length, index = len(inp), 0
    output, j = b'', 0
    for i in range(0, length, 3):
        relativePos      = int(inp[i] | ((inp[i+1] & 0x0F) << 8))
        matchLength      = int((inp[i + 1] & 0xF0) >> 4)
        nextCharacter    = inp[i + 2] & 0xFF
        if matchLength:
            output += output[j-relativePos:j-relativePos+matchLength]
            j += matchLength
        elif nextCharacter:
            output += nextCharacter.to_bytes(1, byteorder='little')
            j += 1
    return output

def lz77 (inp: bytes) -> bytes:
    index, output, SEARCH_SIZE, LOOKAHEAD_SIZE = 0, bytes(), 4095, 15

    while index < len(inp):
        searchStart = max(0, index - SEARCH_SIZE)
        lookaheadEnd = min(len(inp), index + LOOKAHEAD_SIZE)
        # run the matching algorithm
        relativePos, matchLength = findLongestMatch(inp[searchStart: index], inp[index: lookaheadEnd])
        # relative offset
        nextChar = 0 if index + matchLength >= len(inp) else inp[index + matchLength]

        output += (0xFF & relativePos).to_bytes(1, byteorder='little') # 8 bits of relative position
        output += ((0xF00 & relativePos) >> 8 | (0xF & matchLength) << 4).to_bytes(1, byteorder='little') # 4 bits of position and 4 bits of length
        output += nextChar.to_bytes(1, byteorder='little') # 8 bits of next character
        index += max(1, matchLength)
    return output

# f = open('file.txt', 'rb')
# enc = lz77(bytes(f.read()))
# print(lz77rev(enc))