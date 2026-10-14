// Observed: ReleasePrimBuffer__8VIRasteriR12VIDictionary at 0x011133c0 is 196 bytes in the pinned SLUS-20565 image. Arguments: the
// raster (this), a slot index and a dictionary. The primitive-buffer pool is the sub-object at raster + 19320 (0x4b78); its table
// pointer is the pool's word at +8 and slots are 12 bytes with the buffer pointer in the first word. The routine loads the buffer
// pointer (and keeps it in a stack slot), decrements the word at buffer +4 and stores it back. If the new count is above zero it
// returns 0. Otherwise, if the buffer's first word is non-zero it calls Remove__12VIDictionaryR10VIResource(dictionary, buffer) and
// returns -1 when that is negative. It then stores the raster word at +17880 into buffer +16, calls
// Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer(list at raster + 19344, &buffer slot) (-1 on a negative result) and finally
// Erase__t6VIPool1ZP12VIPrimBufferi(pool, index), returning 0 when that is not negative and -1 otherwise.
// Inferred: +4 is a reference count, the buffer is parked on the list at raster +19344 (a free or deferred list) stamped with the
// raster word at +17880 (maybe a frame or time stamp) before its pool slot is erased; the first word says whether it is registered in
// the dictionary.
// Unknown: the buffer layout, what +16 and the raster word at +17880 mean, and the callees' behaviour (unresolved imports here).
// Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
int Remove__12VIDictionaryR10VIResource(unsigned char* dictionary, unsigned char* resource);
int Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer(unsigned char* list, const unsigned* element);
int Erase__t6VIPool1ZP12VIPrimBufferi(unsigned char* pool, int index);
int ReleasePrimBuffer__8VIRasteriR12VIDictionary(unsigned char* self, int index, unsigned char* dictionary) {
    unsigned char* pool = self + 19320;
    unsigned char* table = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
    unsigned bufferWord = *reinterpret_cast<unsigned*>(table + index * 12);
    unsigned char* buffer = NORRATH_WORD_TO_POINTER(bufferWord);
    int count = *reinterpret_cast<int*>(buffer + 4) - 1;
    *reinterpret_cast<int*>(buffer + 4) = count;
    if (count > 0) return 0;
    if (*reinterpret_cast<int*>(buffer) != 0) {
        if (Remove__12VIDictionaryR10VIResource(dictionary, buffer) < 0) return -1;
    }
    *reinterpret_cast<int*>(buffer + 16) = *reinterpret_cast<int*>(self + 17880);
    if (Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer(self + 19344, &bufferWord) < 0) return -1;
    return Erase__t6VIPool1ZP12VIPrimBufferi(pool, index) < 0 ? -1 : 0;
}
}
