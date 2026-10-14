// Observed: CreateMaterialPal__8VIRasterii at 0x0110e078 is 340 bytes in the pinned SLUS-20565 image. Arguments: the raster (this),
// an entry count and a flag. It allocates 16 bytes with __builtin_new; if that succeeds it zeroes the words at +4 then +0 and
// constructs an int array (__t7VIArray1Zi) at +8, and an allocation failure gives a null palette. A null palette returns -1. Then
// Init__t7VIArray1Zii(palette + 8, count) (negative -> -1) and
// Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(raster + 19428, &palette word) whose result is the slot index (negative -> -1).
// With count above zero it then runs count iterations: Add__t6VIList1Z10VIMaterial(raster + 19416) returns a material pointer or a
// negative value; a pointer also gets the 64-bit value at raster + 17872 copied to material +24; the pointer or -1 is stored in
// entry i of the array whose data pointer is the palette word at +12 (the address is computed before the call); a negative stored
// entry returns -1. After the loop a non-zero flag increments the palette word at +4. It returns the slot index.
// Inferred: the palette is a 16-byte record {registered flag, reference count, int array {count, data}} holding material list
// pointers; the 64-bit value at raster +17872 is a stamp (the same value GetTime-like code reads); the flag asks for an initial
// reference. A failed iteration leaves earlier materials and the pool slot allocated.
// Unknown: the array and list classes, what the raster stamp is, and the callees' behaviour (unresolved imports here).
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_POINTER_TO_WORD(pointer) norrath_host_address(pointer)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_POINTER_TO_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif
extern "C" {
void* __builtin_new(unsigned size);
void __t7VIArray1Zi(unsigned char* array);
int Init__t7VIArray1Zii(unsigned char* array, int count);
int Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(unsigned char* pool, const unsigned* element);
unsigned Add__t6VIList1Z10VIMaterial(unsigned char* list);
int CreateMaterialPal__8VIRasterii(unsigned char* self, int count, int flag) {
    unsigned char* palette = static_cast<unsigned char*>(__builtin_new(16));
    if (palette) {
        *reinterpret_cast<int*>(palette + 4) = 0;
        *reinterpret_cast<int*>(palette) = 0;
        __t7VIArray1Zi(palette + 8);
    }
    unsigned paletteWord = palette ? NORRATH_POINTER_TO_WORD(palette) : 0;
    if (!palette) return -1;
    if (Init__t7VIArray1Zii(palette + 8, count) < 0) return -1;
    int slot = Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(self + 19428, &paletteWord);
    if (slot < 0) return -1;
    if (count > 0) {
        int i = 0;
        do {
            unsigned char* held = NORRATH_WORD_TO_POINTER(paletteWord);
            unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(held + 12)) + i * 4;
            i++;
            unsigned material = Add__t6VIList1Z10VIMaterial(self + 19416);
            int stored = -1;
            if (static_cast<int>(material) >= 0) {
                stored = static_cast<int>(material);
                *reinterpret_cast<unsigned long long*>(NORRATH_WORD_TO_POINTER(material) + 24) = *reinterpret_cast<unsigned long long*>(self + 17872);
            }
            *reinterpret_cast<int*>(entry) = stored;
            unsigned char* again = NORRATH_WORD_TO_POINTER(paletteWord);
            if (*reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(again + 12)) + (i - 1) * 4) < 0) return -1;
        } while (i < count);
    }
    if (flag != 0) {
        unsigned char* held = NORRATH_WORD_TO_POINTER(paletteWord);
        *reinterpret_cast<int*>(held + 4) += 1;
    }
    return slot;
}
}
