// Observed __eh_alloc at 0x002fa4a8, 224 bytes. Allocates the storage for a thrown object.
//   size 0                : abort().
//   otherwise             : __wrap_malloc(size); a non-null result is returned.
//   malloc failed         : falls back to a small emergency pool kept in the per-thread
//                           exception context (__get_eh_context at 0x002fa0e0). Word 52 (byte
//                           offset 208) of the context is a bit mask of used 16-byte units; the
//                           pool itself starts 16 bytes into the context. The request is
//                           rounded up to units = (size + 15) >> 4 and, when that is 13 or more,
//                           __terminate() is called. One unit is promoted to two. With
//                           n = units, the search runs downward from position 12 - n, testing
//                           ((mask | mask << 1) >> position) & (2^n - 1) == 0 and stopping with
//                           __terminate() when the position reaches 0 (also when it starts at 0).
//                           On a hit the bits (2^n - 1) >> 1 shifted up by the position are set in
//                           the mask and the address context + 16 + position * 16 is returned.
// __terminate is called as a function that returns in the code (nothing after it is
// reachable in the paths observed); the search simply continues after it here.
// Unknown: what the rest of the context holds; the release side. Nothing was executed on the EE.
extern "C" {
void abort();
void* __wrap_malloc(unsigned size);
void* __get_eh_context();
void __terminate();

void* __eh_alloc(unsigned size) {
    if (size == 0) abort();
    void* result = __wrap_malloc(size);
    if (result != 0) return result;
    unsigned char* context = static_cast<unsigned char*>(__get_eh_context());
    unsigned mask = *reinterpret_cast<unsigned*>(context + 208);
    unsigned units = (size + 15) >> 4;
    if (units >= 13) __terminate();
    unsigned both = mask | (mask << 1);
    unsigned n = units + (units == 1 ? 1u : 0u);
    unsigned bits = (1u << n) - 1;
    unsigned position = 12 - n;
    if (position == 0) __terminate();
    unsigned window = both >> position;
    while ((window & bits) != 0) {
        --position;
        if (position == 0) {
            __terminate();
            break;
        }
        window = both >> position;
    }
    *reinterpret_cast<unsigned*>(context + 208) = mask | ((bits >> 1) << position);
    return context + 16 + position * 16;
}
}
