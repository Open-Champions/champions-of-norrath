// Observed: SetBlendMatrices__9VIHSpriteR8VIRaster at 0x01062ee0 is 368 bytes in the pinned SLUS-20565 image. Arguments: the sprite
// (this) and a raster. It builds a zeroed 64-byte VIMatrix44 on its stack (inlined constructor: sixteen float zero stores plus four
// 128-bit zero stores) and destroys it with _$_10VIMatrix44(matrix, 2) before returning 0; the matrix is never read in between. In
// between: the attachment pool at sprite +96 (a VIPool of VIHSpriteAttachment, table pointer at pool +8, 16-byte records) is walked
// with Begin__Ct6VIPool1Z19VIHSpriteAttachment / End__Ct6VIPool1Z19VIHSpriteAttachment (called again before every test) /
// Next__Ct6VIPool1Z19VIHSpriteAttachmenti until a record whose word at +4 is -1 is found. Only when one is found, the sprite array at
// +64 (count at +64, pointer at +72, entries of 256 bytes) is walked: for entry i,
// Mul__10VIMatrix44RC10VIMatrix44T1(second array data + 64 * i, entry + 160, entry + 48) is called, the count being re-read every
// iteration, and then
// SetObjectBlends__8VIRasteriP10VIMatrix44(raster, count at +64, second array data) is called; the second array is the sub-object at
// sprite +76 whose data pointer is the word at +84.
// Inferred: this builds one blend matrix (product of two matrices of each skeleton entry) per entry for skinned drawing and hands
// them to the raster; the sprite then needs at least one attachment record whose word at +4 is -1.
// Unknown: the layout of the 256-byte entries, why the two matrix offsets 48 and 160 are used, the meaning of the -1 marker, the
// attachment record, and the callees' behaviour (imports of this unit). Nothing here ran on the EE.
//
// A count of zero or below skips the Mul loop (signed test) but SetObjectBlends is still called with that count.
extern "C" {
#ifdef NORRATH_HOST_ARENA
unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int End__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* pool);
int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const unsigned char* pool, int index);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* self, const unsigned char* first, const unsigned char* second);
void SetObjectBlends__8VIRasteriP10VIMatrix44(unsigned char* raster, int count, unsigned char* matrices);
void _$_10VIMatrix44(unsigned char* matrix, int flags);
int SetBlendMatrices__9VIHSpriteR8VIRaster(unsigned char* self, unsigned char* raster) {
    alignas(16) float matrix[16];
    for (int i = 0; i < 16; ++i) matrix[i] = 0.0f;
    bool found = false;
    unsigned char* pool = self + 96;
    int index = Begin__Ct6VIPool1Z19VIHSpriteAttachment(pool);
    while (index != End__Ct6VIPool1Z19VIHSpriteAttachment(pool)) {
        unsigned char* record = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + index * 16;
        if (*reinterpret_cast<int*>(record + 4) == -1) {
            found = true;
            break;
        }
        index = Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, index);
    }
    if (found) {
        unsigned char* first = self + 64;
        unsigned char* second = self + 76;
        if (*reinterpret_cast<int*>(first) > 0) {
            int i = 0;
            do {
                unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(first + 8)) + i * 256;
                unsigned char* out = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(second + 8)) + i * 64;
                i++;
                Mul__10VIMatrix44RC10VIMatrix44T1(out, entry + 160, entry + 48);
            } while (i < *reinterpret_cast<int*>(first));
        }
        SetObjectBlends__8VIRasteriP10VIMatrix44(raster, *reinterpret_cast<int*>(first), NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(second + 8)));
    }
    _$_10VIMatrix44(reinterpret_cast<unsigned char*>(matrix), 2);
    return 0;
}
}
