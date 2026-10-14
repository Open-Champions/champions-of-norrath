// Observed lumpUnload__FPv at 0x00164800, 528 bytes. The argument is a lump data
// pointer, the word at offset 16 of a 40-byte record of `lumps` (0x006cb280).
// 1. The first of the nmLumps (0x00483f8c) records whose offset 16 word equals
//    the argument is chosen. If none is, nothing else happens.
// 2. A record whose offset 20 word has bit 0 clear is released with
//    freeLump__FPv(argument). A record with bit 0 set is a single-file lump: the
//    text at the argument is a name, strrchr finds its last '.', and the text after
//    it picks the release call, each on the address argument + 256: "tex"
//    textureFreeTexture__FP8_texture, "world" worldFreeWorld__FP12_worldHeader,
//    "uni" textRemoveText__FPv. Any other extension, or no '.', releases nothing.
// 3. A nonzero word at offset 36 of the record calls memFree__FPv(argument).
// 4. The records after it move down one place, nmLumps is read again (after the
//    calls) and stored decreased by one.
// Inferred: the layout of the 256-byte name header comes from the +256 offset
// and the strrchr on the argument; the record fields have no names in the image.
// Unknown: what the offset 36 word is, and who else relies on the order of calls.
extern "C" {
struct LumpRecord {
    unsigned word[10];
};
extern LumpRecord lumps[];
extern int nmLumps;

char* strrchr(const char* text, int character);
int strcmp(const char* left, const char* right);
void freeLump__FPv(void* lump);
void memFree__FPv(void* block);
void textureFreeTexture__FP8_texture(void* texture);
void worldFreeWorld__FP12_worldHeader(void* world);
void textRemoveText__FPv(void* text);

#ifdef NORRATH_W8_HOST
extern unsigned norrath_w8_pointer_to_word(const void* pointer);
#define NORRATH_W8_WORD(pointer) norrath_w8_pointer_to_word(pointer)
#else
#define NORRATH_W8_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

void lumpUnload__FPv(void* lump) {
    const unsigned key = NORRATH_W8_WORD(lump);
    const int count = nmLumps;
    int index = 0;
    while (index < count && lumps[index].word[4] != key) ++index;
    if (index == count) return;

    if ((lumps[index].word[5] & 1u) == 0u) {
        freeLump__FPv(lump);
    } else {
        const char* const dot = strrchr(static_cast<const char*>(lump), '.');
        if (dot != nullptr) {
            void* const data = static_cast<char*>(lump) + 256;
            if (strcmp(dot + 1, "tex") == 0) {
                textureFreeTexture__FP8_texture(data);
            } else if (strcmp(dot + 1, "world") == 0) {
                worldFreeWorld__FP12_worldHeader(data);
            } else if (strcmp(dot + 1, "uni") == 0) {
                textRemoveText__FPv(data);
            }
        }
    }
    if (lumps[index].word[9] != 0u) memFree__FPv(lump);

    const int last = nmLumps - 1;
    for (int move = index; move < last; ++move) lumps[move] = lumps[move + 1];
    nmLumps = last;
}
}
