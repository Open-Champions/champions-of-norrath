// __wrap_malloc at 0x001449a8, 52 bytes.
// A zero enableMallocWrapper byte tail-calls malloc with the same size.
// Any other byte, including 0x80, tail-calls memAlloc with the size and the
// unsymbolized text "fake malloc" at 0x00461ea8. The result is the callee's
// pointer.
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4, "32-bit malloc size");

extern "C" {
extern unsigned char enableMallocWrapper;
void* malloc(unsigned size);
void* memAlloc__FiPc(int bytes, char* name);
void* __wrap_malloc(unsigned size);
}

namespace {
char allocation_name[] = {'f', 'a', 'k', 'e', ' ', 'm', 'a', 'l', 'l', 'o', 'c', 0};
}

extern "C" void* __wrap_malloc(unsigned size) {
    if (enableMallocWrapper != 0)
        return memAlloc__FiPc(static_cast<int>(size), allocation_name);
    return malloc(size);
}
