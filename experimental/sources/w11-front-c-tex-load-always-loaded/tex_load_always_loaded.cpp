// Observed texLoadAlwaysLoadedPages__Fv at 0x001703a0, 408 bytes, one caller
// (worldAllocateSegments). It runs once per entry i in 0 .. nmAlwaysPages-1.
// Entry i of alwaysLoadedPage (8 bytes: a page number, then a pointer) names a
// page p. The world header `world` holds a base number at +88 and a pointer at
// +100 to 8-byte records (a disc location, then a byte size); the record for p
// is at index p - base. Per entry:
//   size   = (record.size + 2047) & -2048
//   block  = memAlloc(size, "always loaded page")
//   entry.pointer = block; texturePageTable[p] = i + 9 (a byte);
//   texturePageLoc[i + 9] = block (stored while the name is being formatted)
//   sprintf(name, "%s.tex", engineCurrentLevelName)
//   cdLoad(name, block, size, record.location)   (world re-read after sprintf)
//   a 256-byte local is filled with k at index k, high to low
//   the word at block is a count; for each of that many 64-byte records
//   starting at block + 64: textureInitTexture(world, record, local, block)
// `world` is re-read after every call. Inferred: i + 9 is a slot number above
// slots 0..8 of texturePageLoc; the local is an identity table. Unknown: the
// type of the 64-byte records and what the 256 bytes are used for.
struct _worldHeader;
struct _texture;
struct NorrathAlwaysPage {
    int page;
    void* block;
};
extern "C" {
extern _worldHeader* world;
extern int nmAlwaysPages;
extern NorrathAlwaysPage alwaysLoadedPage[];
extern signed char texturePageTable[];
extern int texturePageLoc[];
extern char engineCurrentLevelName[80];
void* memAlloc__FiPc(int bytes, char* label);
int sprintf(char* destination, const char* format, ...);
void cdLoad__FPcPvii(char* name, void* destination, int size, int location);
void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(_worldHeader* header, _texture* texture,
                                                         unsigned char* table, char* block);
}

extern "C" void texLoadAlwaysLoadedPages__Fv() {
    for (int i = 0; i < nmAlwaysPages; ++i) {
        char name[112];
        unsigned char table[256];
        int page = alwaysLoadedPage[i].page;
        unsigned char* header = reinterpret_cast<unsigned char*>(world);
        int* record = reinterpret_cast<int**>(header + 100)[0] + (page - *reinterpret_cast<int*>(header + 88)) * 2;
        int size = (record[1] + 2047) & -2048;
        char* block = static_cast<char*>(memAlloc__FiPc(size, const_cast<char*>("always loaded page")));
        alwaysLoadedPage[i].block = block;
        texturePageTable[page] = static_cast<signed char>(i + 9);
        sprintf(name, "%s.tex", engineCurrentLevelName);
        texturePageLoc[i + 9] = reinterpret_cast<long>(block);
        header = reinterpret_cast<unsigned char*>(world);
        record = reinterpret_cast<int**>(header + 100)[0] + (page - *reinterpret_cast<int*>(header + 88)) * 2;
        cdLoad__FPcPvii(name, block, size, record[0]);
        for (int k = 255; k >= 0; --k) table[k] = static_cast<unsigned char>(k);
        int count = *reinterpret_cast<int*>(block);
        for (int n = 0; n < count; ++n)
            textureInitTexture__FP12_worldHeaderP8_texturePUcPc(
                world, reinterpret_cast<_texture*>(block + 64 + n * 64), table, block);
    }
}
