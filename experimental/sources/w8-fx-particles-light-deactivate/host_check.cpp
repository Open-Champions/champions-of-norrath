// Host contract for lightDeactivate__FP5Light. Not an EE plan input. Build with
// -DNORRATH_HOST_CONTRACT so 32-bit address words map into a static arena.
#include <cstdio>
#include <cstring>

extern "C" void lightDeactivate__FP5Light(void* light);

alignas(8) static unsigned arena[64];
static unsigned low(const void* p) {
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(p));
}
extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(arena);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static int failures = 0;
static void expect(bool ok, const char* what) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; }
}

int main() {
    // Chain: head word -> A -> B. A sits at arena[8], B at arena[24], head at arena[0].
    unsigned* head = arena;
    unsigned* a = arena + 8;
    unsigned* b = arena + 24;
    std::memset(arena, 0xee, sizeof arena);
    head[0] = low(a);
    a[0] = low(b);   a[1] = low(head);
    b[0] = 0;        b[1] = low(a);
    lightDeactivate__FP5Light(a);
    expect(head[0] == low(b), "previous word takes next");
    expect(b[1] == low(head), "next's previous-link takes the previous-link");
    expect(a[0] == 0u && a[1] == 0u, "own links cleared");
    expect(a[2] == 0xeeeeeeeeu, "other words untouched");

    // Last in the chain: next is zero, nothing is stored through it.
    std::memset(arena, 0xee, sizeof arena);
    b[0] = 0; b[1] = low(head); head[0] = low(b);
    lightDeactivate__FP5Light(b);
    expect(head[0] == 0u, "last node writes zero into previous word");
    expect(b[0] == 0u && b[1] == 0u, "last node cleared");

    // First with no previous-link: only the next side is written.
    std::memset(arena, 0xee, sizeof arena);
    a[0] = low(b); a[1] = 0; b[0] = 0; b[1] = low(a);
    lightDeactivate__FP5Light(a);
    expect(b[1] == 0u, "no previous: next's previous-link becomes zero");
    expect(head[0] == 0xeeeeeeeeu, "nothing else written");

    // Isolated: both zero, nothing dereferenced.
    std::memset(a, 0, 8);
    lightDeactivate__FP5Light(a);
    expect(a[0] == 0u && a[1] == 0u, "isolated stays cleared");

    // The previous-link addresses the light's own next word: next is reloaded
    // after the store, so the next-side write sees the value just stored.
    std::memset(arena, 0xee, sizeof arena);
    a[0] = low(b); a[1] = low(a); b[0] = 0; b[1] = low(a);
    lightDeactivate__FP5Light(a);
    expect(b[1] == low(a), "self-aliased previous-link: next+4 takes the old previous-link");
    expect(a[0] == 0u && a[1] == 0u, "self-aliased light cleared");
    if (failures) return 1;
    std::printf("light-deactivate contract passed\n");
    return 0;
}
