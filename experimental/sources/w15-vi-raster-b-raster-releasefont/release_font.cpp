// Observed: ReleaseFont__8VIRasteriR12VIDictionary at 0x01113cb0 is 164 bytes in the pinned SLUS-20565 image. Arguments: the raster
// (this), a slot index and a dictionary. The font pool is the sub-object at raster + 19508 (0x4c34); its table pointer is the pool's
// word at +8 and slots are 12 bytes with the font pointer in the first word. The routine loads the font, decrements the word at font
// +4 and stores it back. If the new count is above zero it returns 0. Otherwise, if the font's first word is non-zero it calls
// Remove__12VIDictionaryR10VIResource(dictionary, font) and returns -1 when that is negative. It then calls the destructor
// _$_7VIRFont(font, 3) and Erase__t6VIPool1ZP7VIRFonti(pool, index) and returns 0 when Erase result is not negative, else -1.
// Inferred: +4 of a font is a reference count (see ShareSurface and SharePrimBuffer for the matching increments), the first word is a
// flag or key that says whether the font is registered in the dictionary, and flag 3 asks for the deleting form of the destructor.
// Unknown: the VIRFont layout, what the first word is, and the callees' behaviour (all unresolved imports here). The count is
// decremented before the test, so a count that was already zero goes negative and takes the release path, as in the original.
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
void _$_7VIRFont(unsigned char* font, int flags);
int Erase__t6VIPool1ZP7VIRFonti(unsigned char* pool, int index);
int ReleaseFont__8VIRasteriR12VIDictionary(unsigned char* self, int index, unsigned char* dictionary) {
    unsigned char* pool = self + 19508;
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
    unsigned char* font = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(table + index * 12));
    int count = *reinterpret_cast<int*>(font + 4) - 1;
    *reinterpret_cast<int*>(font + 4) = count;
    if (count > 0) return 0;
    if (*reinterpret_cast<int*>(font) != 0) {
        if (Remove__12VIDictionaryR10VIResource(dictionary, font) < 0) return -1;
    }
    _$_7VIRFont(font, 3);
    return Erase__t6VIPool1ZP7VIRFonti(pool, index) < 0 ? -1 : 0;
}
}
