// Host contract for worldAllocateSegments__FP12_worldHeader. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA; stored pointers are arena offsets.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_alloc_arena = arena;
struct _worldHeader;
extern "C" {
short texmapstart = 0, texmapend = 0;
short texmap[400];
}
static std::vector<std::string> events;
static int allocated = -1;
static std::string alloc_name;
extern "C" void lumpLoad__FPc(char* s) { events.push_back(s); }
extern "C" void texLoadAlwaysLoadedPages__Fv() { events.push_back("<tex>"); }
extern "C" void* memAlloc__FiPc(int size, char* name) { allocated = size; alloc_name = name; return arena + 0x1ff0; }
extern "C" void worldAllocateSegments__FP12_worldHeader(_worldHeader*);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
int main() {
    // header at 0; records at 0x100; name at 0x200; table at 0x300.
    put32(0, 2); put32(36, 0x100);
    put32(0x100, 0x200); put32(0x100 + 36, 0x800); put32(0x100 + 60 + 36, 0x7ff);
    std::strcpy(reinterpret_cast<char*>(arena + 0x200), "alpha");
    put32(96, 1); put32(88, 1); put32(92, 2); put32(100, 0x300);
    put32(0x300 + 4, 0x20000);                      // type 1: one chunk
    put32(0x300 + 8 + 4, 0x20001);                  // type 2: two chunks (rounds up)
    texmapstart = 100; texmapend = 300;
    for (int i = 0; i < 201; ++i) texmap[i] = 1;
    texmap[205 - 100] = 2;                          // map index 205 is row 2, column 5
    auto world = reinterpret_cast<_worldHeader*>(arena);
    put32(96, 0);                                   // zero word at +96: loads only
    worldAllocateSegments__FP12_worldHeader(world);
    if (events.size() != 2 || events[0] != "alpha.lmp" || events[1] != "<tex>" || allocated != -1) return 1;
    put32(96, 1); put32(124, 0x40);                 // already allocated: nothing more
    events.clear();
    worldAllocateSegments__FP12_worldHeader(world);
    if (events.size() != 2 || allocated != -1) return 2;
    put32(124, 0);
    worldAllocateSegments__FP12_worldHeader(world);
    // Valid indices 100..300 cover rows 1 and 2 fully; a block centred on row 1 or 2 holds six
    // one-chunk cells, and one of them (205) counts two.
    if (allocated != (7 << 17) || alloc_name != "segment cache") return 3;
    int stored, best;
    std::memcpy(&best, arena + 96, 4);
    std::memcpy(&stored, arena + 124, 4);
    if (best != 7) return 4;
    if (static_cast<unsigned>(stored) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(arena + 0x1ff0))) return 5;   // low 32 bits of the host address
    std::printf("world allocate segments contract passed\n");
    return 0;
}
