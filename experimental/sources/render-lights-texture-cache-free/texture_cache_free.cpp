// Observed textureFreeCacheBuffer__Fv at 0x172558, extent 48.
// uncompCacheAlloc at 0x4840a4 and uncompCacheBuffer at 0x4840a8 are 4-byte
// file zeros. uncompCache at 0x486974 has no file bytes; decodeQHead begins
// four bytes later. A null allocation pointer returns without a free and
// without clearing either word. A non-null pointer is passed to memFree, and
// only then are the allocation and buffer words cleared. uncompCache is left
// unchanged. These addresses are not objects already authored on this branch.

extern "C" void memFree__FPv(char* block);

extern "C" {
char* uncompCacheAlloc;
char* uncompCacheBuffer;
char* uncompCache;
}

extern "C" void textureFreeCacheBuffer__Fv() {
    char* block = uncompCacheAlloc;
    if (block != nullptr) {
        memFree__FPv(block);
        uncompCacheAlloc = nullptr;
        uncompCacheBuffer = nullptr;
    }
}
