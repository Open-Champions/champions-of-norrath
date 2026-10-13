// Observed worldFreeWorld__FP12_worldHeader at 0x001752f8, 328 bytes. Header fields used:
// a record count at +0, a record array pointer at +36 (60-byte records), a texture pointer
// at +108 and an allocation at +124.
// For each record in order: a nonzero signed halfword at record+52 indexes the byte table
// texturePageTable; when that byte is not -1 it selects a base from the word table
// texturePageLoc and textureFreeTexture__FP8_texture is called on base + the word at
// record+32. The count is re-read from the header only after such a call. Then, when bit 7
// (0x80) of the word at record+36 is set and the word at record+0 points to a block whose
// word at +8 is nonzero, the word at +4768 of that pointee is cleared and so is +8 of the
// block. After the loop: the texture at +108 is freed when nonzero (the field is left
// alone); the allocation at +124 is memFree'd and zeroed when nonzero; mt_dyndat and
// mt_throwaway are each memFree'd and zeroed when nonzero; texClearAlwaysLoadedPages__Fv
// is called last. Nothing is returned.
// Inferred: the records are per-texture entries. Unknown: the owners of the +108 and +124
// fields and the meaning of record+36 bit 7.
struct _worldHeader;
struct _texture;
extern signed char texturePageTable[];
extern int texturePageLoc[];
extern void* mt_dyndat;
extern void* mt_throwaway;
extern "C" void textureFreeTexture__FP8_texture(_texture*);
extern "C" void memFree__FPv(void*);
extern "C" void texClearAlwaysLoadedPages__Fv();

extern "C" void worldFreeWorld__FP12_worldHeader(_worldHeader* world) {
    unsigned char* header = reinterpret_cast<unsigned char*>(world);
    int count = *reinterpret_cast<int*>(header);
    for (int i = 0; i < count; ++i) {
        unsigned char* record = *reinterpret_cast<unsigned char**>(header + 36) + i * 60;
        short texture = *reinterpret_cast<short*>(record + 52);
        if (texture != 0) {
            int page = texturePageTable[texture];
            if (page != -1) {
                textureFreeTexture__FP8_texture(reinterpret_cast<_texture*>(
                    texturePageLoc[page] + *reinterpret_cast<int*>(record + 32)));
                count = *reinterpret_cast<int*>(header);
            }
        }
        if (*reinterpret_cast<int*>(record + 36) & 0x80) {
            unsigned char* block = *reinterpret_cast<unsigned char**>(record);
            unsigned char* linked = *reinterpret_cast<unsigned char**>(block + 8);
            if (linked) {
                *reinterpret_cast<int*>(linked + 4768) = 0;
                *reinterpret_cast<int*>(block + 8) = 0;
            }
        }
    }
    if (*reinterpret_cast<_texture**>(header + 108))
        textureFreeTexture__FP8_texture(*reinterpret_cast<_texture**>(header + 108));
    if (*reinterpret_cast<void**>(header + 124)) {
        memFree__FPv(*reinterpret_cast<void**>(header + 124));
        *reinterpret_cast<void**>(header + 124) = 0;
    }
    if (mt_dyndat) {
        memFree__FPv(mt_dyndat);
        mt_dyndat = 0;
    }
    if (mt_throwaway) {
        memFree__FPv(mt_throwaway);
        mt_throwaway = 0;
    }
    texClearAlwaysLoadedPages__Fv();
}
