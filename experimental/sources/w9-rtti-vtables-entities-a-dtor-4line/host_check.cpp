// Host check only. Not an EE build input.
#include <cstdio>
#include <cstring>
#include <cstdint>

extern "C" void dtor_4Line(void* object, int flags) asm("_$_4Line");
static int failures = 0, mem_free_calls = 0, block_free_calls = 0, order = 0, mem_free_order = 0, block_free_order = 0;
static void* mem_freed;
static void* block_freed;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void memFree__FPv(void* pointer) { ++mem_free_calls; mem_freed = pointer; mem_free_order = ++order; }
extern "C" void norrath_block_free(void* object) { ++block_free_calls; block_freed = object; block_free_order = ++order; }

int main() {
    // The word at +20 is a 32-bit pointer; on the host only the low 32 bits of a
    // small value can be compared, so use a value that fits.
    unsigned char object[64];
    const int flags_to_try[] = {0, 1, 2, 3, 4, -1, -2};
    for (int flags : flags_to_try) {
        std::memset(object, 0x6b, sizeof object);
        const unsigned word = 0x00123450u;
        std::memcpy(object + 20, &word, 4);
        unsigned char before[64];
        std::memcpy(before, object, sizeof object);
        mem_free_calls = block_free_calls = order = 0;
        mem_freed = block_freed = nullptr;
        dtor_4Line(object, flags);
        expect(mem_free_calls == 1 && reinterpret_cast<std::uintptr_t>(mem_freed) == word, "the block at +20 is always released");
        expect(block_free_calls == ((flags & 1) ? 1 : 0), "the object is freed only when bit 0 is set");
        if (flags & 1) expect(block_freed == object && mem_free_order < block_free_order, "object freed after the block");
        expect(std::memcmp(object, before, sizeof object) == 0, "the object is not modified");
    }
    // a zero word is still passed on
    std::memset(object, 0, sizeof object);
    mem_free_calls = 0; mem_freed = object;
    dtor_4Line(object, 0);
    expect(mem_free_calls == 1 && mem_freed == nullptr, "a zero pointer is passed to memFree");
    if (failures == 0) std::printf("4Line destructor cases passed\n");
    return failures == 0 ? 0 : 1;
}
