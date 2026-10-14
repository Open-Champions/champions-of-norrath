// Observed textureAllocateCacheBuffer__Fv at 0x172588, extent 88.
// If uncompCacheAlloc is non-null it is passed to memFree and the words are
// not cleared before the new allocation. memAlloc is called with size
// 0x500100 and the label textureCacheBuffer. The returned pointer is stored
// in uncompCacheAlloc. The same address plus 127, masked with ~127, is stored
// in uncompCacheBuffer. That aligned address plus 112 is stored in uncompCache.
// A null allocation therefore stores 0, 0, and 112.

extern "C" void memFree__FPv(char* block);
extern "C" char* memAlloc__FiPc(int size, char* label);
extern "C" char* uncompCacheAlloc;
extern "C" char* uncompCacheBuffer;
extern "C" char* uncompCache;

extern "C" void textureAllocateCacheBuffer__Fv() {
    char* existing = uncompCacheAlloc;
    if (existing != nullptr) memFree__FPv(existing);
    char* block = memAlloc__FiPc(0x500100, const_cast<char*>("textureCacheBuffer"));
    unsigned long address = reinterpret_cast<unsigned long>(block);
    unsigned long aligned = (address + 127ul) & ~127ul;
    uncompCacheAlloc = block;
    uncompCacheBuffer = reinterpret_cast<char*>(aligned);
    uncompCache = reinterpret_cast<char*>(aligned + 112ul);
}
