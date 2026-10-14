// Observed: ParseWorldBaseHeader__10VIESFParseR7VIWorld at 0x01176580, 480 bytes. Arguments: parse (a0, object file word at +36) and
// world (a1). Locals: a 24-byte box (six floats), a three-float vector, all zero-initialised, and the integers and one float below.
// In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &second): a negative result, or a tag other than 0x8220, returns -1;
//   2. when `second` is non-zero Read__9VIObjFileRi reads integer A, otherwise A is 1;
//   3. six Read__9VIObjFileRf calls fill the box in memory order (offsets 0 to 20);
//   4. two Read__9VIObjFileRi calls read integers B and C;
//   5. when `second` (read again) is non-zero Read__9VIObjFileRi reads integer D, otherwise D is 4;
//   6. four Read__9VIObjFileRf calls read a float E and then the three vector components;
//   7. Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(world, A, B, C, D, E, vector, box): a negative result returns -1;
//   8. ReadEnd__9VIObjFile(file): a negative result returns -1, otherwise 0.
// Inferred: this is the world header record; the second ReadBegin output is a format version (non-zero adds A and D, which default
// to 1 and 4 for the older format); the box is the world's bounding box and the vector a size or origin. A tag mismatch does not
// call ReadEnd.
// Unknown: the member names, what A to E mean, and the argument order beyond what the call shows.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(void* world, int a, int b, int c, int d, float e, const float* vector, const float* box);

int ParseWorldBaseHeader__10VIESFParseR7VIWorld(unsigned char* parse, void* world) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    float box[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float vector[3] = {0.0f, 0.0f, 0.0f};
    unsigned short tag;
    unsigned short second;
    int a;
    int b;
    int c;
    int d;
    float e;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x8220) return -1;
    if (second != 0) Read__9VIObjFileRi(file, &a); else a = 1;
    Read__9VIObjFileRf(file, &box[0]);
    Read__9VIObjFileRf(file, &box[1]);
    Read__9VIObjFileRf(file, &box[2]);
    Read__9VIObjFileRf(file, &box[3]);
    Read__9VIObjFileRf(file, &box[4]);
    Read__9VIObjFileRf(file, &box[5]);
    Read__9VIObjFileRi(file, &b);
    Read__9VIObjFileRi(file, &c);
    if (second != 0) Read__9VIObjFileRi(file, &d); else d = 4;
    Read__9VIObjFileRf(file, &e);
    Read__9VIObjFileRf(file, &vector[0]);
    Read__9VIObjFileRf(file, &vector[1]);
    Read__9VIObjFileRf(file, &vector[2]);
    if (Init__7VIWorldiiiifRC7VIVect3RC6VIBBox(world, a, b, c, d, e, vector, box) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
