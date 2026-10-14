// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define memset w9_memset_stub
static unsigned asked;
static bool fail_alloc;
static std::size_t cleared;
extern "C" void* __wrap_malloc(unsigned size) {
    asked = size;
    if (fail_alloc) return nullptr;
    void* p = std::aligned_alloc(16, 2576);
    for (int i = 0; i < 2576; ++i) static_cast<unsigned char*>(p)[i] = 0xaa;
    return p;
}
extern "C" void* memset(void* d, int v, unsigned c) {
    cleared = c;
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned i = 0; i < c; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
#include "gsm_create.cpp"

int main() {
    int failures = 0;
    unsigned char* p = static_cast<unsigned char*>(gsm_create());
    if (!p || asked != 2568u || cleared != 2568u) { std::printf("size wrong\n"); return 1; }
    for (int i = 0; i < 2568; ++i) {
        bool field = i >= 2472 && i < 2480;
        unsigned char want = field ? (i == 2472 ? 40 : 0) : 0;
        if (p[i] != want) { std::printf("byte %d = %d\n", i, p[i]); ++failures; break; }
    }
    if (p[2568] != 0xaa) { std::printf("wrote past the object\n"); ++failures; }
    fail_alloc = true; cleared = 0;
    if (gsm_create() != nullptr || cleared != 0) { std::printf("null must pass through unwritten\n"); ++failures; }
    if (failures) return 2;
    std::printf("gsm_create cases passed\n");
    return 0;
}
