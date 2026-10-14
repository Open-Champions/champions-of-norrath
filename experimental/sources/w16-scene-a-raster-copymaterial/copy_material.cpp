// Observed: CopyMaterial__8VIRasterii at 0x0110da70 is 432 bytes in the pinned SLUS-20565 image. Arguments: raster, src (an int that the code
// uses as the address of a material record), flag. All offsets are bytes. It calls Add__t6VIList1Z10VIMaterial(raster + 19416) (no
// source argument; the mangled name has no parameters) and treats the result as the new material record; a negative result returns -1.
// If flag is non-zero the word at +4 of the new record is incremented (a reference count). Then it copies from src: the word at +8 (the
// layer count n), the doubleword at +24, the words at +12 and +32. For each of the n layers (record base advanced 144 bytes at a time
// from src and from the new record): words at +48, +52, +56 (a four-byte copy through unaligned load/store), +60; the matrix at +64 by
// __as__10VIMatrix33RC10VIMatrix33(new + 64, src + 64); floats at +100, +104, +108, +112, +116; the word at +120; and 64 bytes from +128
// (four 16-byte copies; for the last layer this reaches 48 bytes past that layer's 144-byte stride). The layer count is re-read from the new
// record after each layer. A second pass over the new record's layers (count re-read from +8) takes each word at +48 + 144 * k: unless it is -1 it
// is an index into the 12-byte slots of the table whose pointer is the word at raster + 19388 (raster + 19380 + 8), and the word at +4 of the
// object that slot's first word points to is incremented. It returns the new record.
// Inferred: this is the material copy-constructor for the raster's material list: the +48 word is a layer's texture/surface id (as in
// GetLayerTexture__10VIMateriali), +56 its colour (as in GetLayerColor), and the +4 words are reference counts, so the copy adds a
// reference to every surface the layers use. The 144-byte layer records start at +48 (so the 64-byte copy from +128 is the tail of
// each record, from record offset 80).
// Unknown: the roles of +12, +24, +32 and of the float and word fields between, the list's failure value (only "negative" is
// tested), and the callees' behaviour (imports). Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Add__t6VIList1Z10VIMaterial(unsigned char* list);
void* __as__10VIMatrix33RC10VIMatrix33(unsigned char* self, const unsigned char* other);

int CopyMaterial__8VIRasterii(unsigned char* raster, int src, int addReference) {
    int created = Add__t6VIList1Z10VIMaterial(raster + 19416);
    if (created < 0) return -1;
    unsigned char* from = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(src));
    unsigned char* to = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(created));
    if (addReference != 0) *reinterpret_cast<int*>(to + 4) += 1;
    int count = *reinterpret_cast<int*>(from + 8);
    unsigned long long pair = *reinterpret_cast<unsigned long long*>(from + 24);
    *reinterpret_cast<int*>(to + 8) = count;
    *reinterpret_cast<unsigned long long*>(to + 24) = pair;
    *reinterpret_cast<int*>(to + 12) = *reinterpret_cast<int*>(from + 12);
    *reinterpret_cast<int*>(to + 32) = *reinterpret_cast<int*>(from + 32);
    unsigned char* s = from;
    unsigned char* d = to;
    for (int i = 0; i < count; ++i) {
        *reinterpret_cast<int*>(d + 48) = *reinterpret_cast<int*>(s + 48);
        *reinterpret_cast<int*>(d + 52) = *reinterpret_cast<int*>(s + 52);
        d[56] = s[56]; d[57] = s[57]; d[58] = s[58]; d[59] = s[59];
        *reinterpret_cast<int*>(d + 60) = *reinterpret_cast<int*>(s + 60);
        __as__10VIMatrix33RC10VIMatrix33(d + 64, s + 64);
        *reinterpret_cast<float*>(d + 100) = *reinterpret_cast<float*>(s + 100);
        *reinterpret_cast<int*>(d + 120) = *reinterpret_cast<int*>(s + 120);
        *reinterpret_cast<float*>(d + 104) = *reinterpret_cast<float*>(s + 104);
        *reinterpret_cast<float*>(d + 108) = *reinterpret_cast<float*>(s + 108);
        *reinterpret_cast<float*>(d + 112) = *reinterpret_cast<float*>(s + 112);
        *reinterpret_cast<float*>(d + 116) = *reinterpret_cast<float*>(s + 116);
        for (int k = 0; k < 4; ++k) {
            *reinterpret_cast<unsigned long long*>(d + 128 + 16 * k) = *reinterpret_cast<unsigned long long*>(s + 128 + 16 * k);
            *reinterpret_cast<unsigned long long*>(d + 136 + 16 * k) = *reinterpret_cast<unsigned long long*>(s + 136 + 16 * k);
        }
        s += 144;
        d += 144;
        count = *reinterpret_cast<int*>(to + 8);
    }
    if (count > 0) {
        const unsigned char* slot = to + 48;
        for (int i = 0; i < *reinterpret_cast<int*>(to + 8); ++i) {
            int id = *reinterpret_cast<const int*>(slot);
            slot += 144;
            if (id != -1) {
                unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(raster + 19380 + 8));
                unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + id * 12));
                *reinterpret_cast<int*>(object + 4) += 1;
            }
        }
    }
    return created;
}
}
