// Host contract for textureAllocateCacheBuffer__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" char* uncompCacheAlloc;
extern "C" char* uncompCacheBuffer;
extern "C" char* uncompCache;
extern "C" void textureAllocateCacheBuffer__Fv();

extern "C" char* uncompCacheAlloc = nullptr;
extern "C" char* uncompCacheBuffer = nullptr;
extern "C" char* uncompCache = nullptr;

static int failures = 0;
static int frees = 0;
static int allocs = 0;
static char* freed = nullptr;
static int requested = -1;
static const char* label = nullptr;
static char* alloc_during_alloc = nullptr;
static char* buffer_during_alloc = nullptr;
static char* cache_during_alloc = nullptr;
static char* result = nullptr;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void memFree__FPv(char* block) {
    ++frees;
    freed = block;
}

extern "C" char* memAlloc__FiPc(int size, char* text) {
    ++allocs;
    requested = size;
    label = text;
    alloc_during_alloc = uncompCacheAlloc;
    buffer_during_alloc = uncompCacheBuffer;
    cache_during_alloc = uncompCache;
    return result;
}

static char* align112(char* block) {
    unsigned long address = reinterpret_cast<unsigned long>(block);
    unsigned long aligned = (address + 127ul) & ~127ul;
    return reinterpret_cast<char*>(aligned + 112ul);
}
static char* aligned_only(char* block) {
    unsigned long address = reinterpret_cast<unsigned long>(block);
    unsigned long aligned = (address + 127ul) & ~127ul;
    return reinterpret_cast<char*>(aligned);
}

int main() {
    alignas(128) char storage[256];
    char* base = storage;
    char kept = 0;
    uncompCache = &kept;
    result = base;
    textureAllocateCacheBuffer__Fv();
    expect(frees == 0, "null allocation is not freed");
    expect(allocs == 1 && requested == 0x500100, "allocation size");
    expect(label != nullptr && std::strcmp(label, "textureCacheBuffer") == 0, "allocation label");
    expect(alloc_during_alloc == nullptr && cache_during_alloc == &kept, "words unchanged until alloc returns");
    expect(uncompCacheAlloc == base, "stores the raw pointer");
    expect(uncompCacheBuffer == aligned_only(base), "stores the aligned pointer");
    expect(uncompCache == align112(base), "stores the aligned pointer plus 112");

    result = base + 1;
    uncompCacheBuffer = base + 3;
    textureAllocateCacheBuffer__Fv();
    expect(frees == 1 && freed == base, "frees the previous raw pointer");
    expect(alloc_during_alloc == base && buffer_during_alloc == base + 3, "does not clear before alloc");
    expect(uncompCacheAlloc == base + 1, "stores the unaligned raw pointer");
    expect(uncompCacheBuffer == aligned_only(base + 1), "aligns an unaligned pointer");
    expect(uncompCache == align112(base + 1), "cache follows the new alignment");

    result = nullptr;
    textureAllocateCacheBuffer__Fv();
    expect(uncompCacheAlloc == nullptr, "null raw pointer");
    expect(uncompCacheBuffer == nullptr, "null aligns to zero");
    expect(uncompCache == reinterpret_cast<char*>(112ul), "null cache word is 112");
    if (failures != 0) return 1;
    std::printf("texture-cache-allocate contract passed\n");
    return 0;
}
