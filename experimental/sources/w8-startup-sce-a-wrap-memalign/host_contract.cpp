#define memalign norrath_host_library_memalign
#include "wrap_memalign.cpp"
#undef memalign

#include <cstdio>
#include <cstring>

unsigned char enableMallocWrapper = 0;
static unsigned lib_alignment, lib_size;
static int mem_bytes;
static char mem_label[16];
static int token;
extern "C" void* norrath_host_library_memalign(unsigned a, unsigned s) { lib_alignment = a; lib_size = s; return &token; }
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    mem_bytes = bytes;
    std::strncpy(mem_label, label, 15);
    return &mem_label;
}

int main() {
    enableMallocWrapper = 0;
    if (__wrap_memalign(64, 240) != &token || lib_alignment != 64 || lib_size != 240 || mem_bytes != 0) return 1;
    enableMallocWrapper = 1;
    if (__wrap_memalign(64, 1000) != static_cast<void*>(&mem_label)) return 2;
    if (mem_bytes != 1000 || std::strcmp(mem_label, "fake malloc") != 0 || lib_size != 240) return 3;
    std::puts("wrap memalign contract passed");
    return 0;
}
