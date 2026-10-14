// Observed: ParseWorldTree__10VIESFParseR11VIWorldTree at 0x01176940, 392 bytes. Arguments: parse (a0, object file word at +36) and
// tree (a1). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result, or a tag other than 0x8230, returns -1 (a tag mismatch does
//      not call ReadEnd);
//   2. Read__9VIObjFileRi twice into two locals, A then B;
//   3. Init__11VIWorldTreeii(tree, A, B): a negative result returns -1;
//   4. for each index i below A (A is the local, read again after every pass), the tree word at +4 is a record base and record i, 24
//      bytes long, is filled from the file: four floats (Read__9VIObjFileRf) at offsets +0, +4, +8 and +12, then two unsigned
//      words (Read__9VIObjFileRUi) at +16 and +20; the base word is read again before every field;
//   5. for each index i below B (read again after every pass), Read__9VIObjFileRi(file, (the tree word at +12) + 4 * i);
//   6. ReadEnd__9VIObjFile(file): a negative result returns -1, otherwise 0.
// Inferred: a spatial tree of A nodes (24 bytes each: a 4-float box or plane plus two words) and B integer entries; the tag names the
// world tree record.
// Unknown: the member names, what the node floats and words are, and the values of A and B that Init accepts.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Init__11VIWorldTreeii(void* tree, int nodeCount, int entryCount);

int ParseWorldTree__10VIESFParseR11VIWorldTree(unsigned char* parse, unsigned char* tree) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    unsigned* treeWords = reinterpret_cast<unsigned*>(tree);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short second;
    int nodeCount;
    int entryCount;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8230) return -1;
    Read__9VIObjFileRi(file, &nodeCount);
    Read__9VIObjFileRi(file, &entryCount);
    if (Init__11VIWorldTreeii(tree, nodeCount, entryCount) < 0) return -1;
    for (int i = 0; i < nodeCount; ++i) {
        int offset = i * 24;
        Read__9VIObjFileRf(file, reinterpret_cast<float*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset));
        Read__9VIObjFileRf(file, reinterpret_cast<float*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset + 4));
        Read__9VIObjFileRf(file, reinterpret_cast<float*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset + 8));
        Read__9VIObjFileRf(file, reinterpret_cast<float*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset + 12));
        Read__9VIObjFileRUi(file, reinterpret_cast<unsigned*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset + 16));
        Read__9VIObjFileRUi(file, reinterpret_cast<unsigned*>(static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(treeWords[1])) + offset + 20));
    }
    for (int i = 0; i < entryCount; ++i) {
        Read__9VIObjFileRi(file, reinterpret_cast<int*>(NORRATH_WORD_ADDRESS(treeWords[3])) + i);
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
