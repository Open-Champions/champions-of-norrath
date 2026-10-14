// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "next.cpp"
static const void* seen;
static int calls;
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(const void* pool) { seen = pool; ++calls; return 777; }
int main() {
    unsigned pool[6] = {0, 0, arena_base + 0x10, 0, 0, 0};
    int values[4] = {3, 0, -2, -1};
    for (int i = 0; i < 4; ++i) *reinterpret_cast<int*>(arena + 0x10 + i * 16 + 8) = values[i];
    if (Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, 0) != 3 || Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, 1) != 0 ||
        Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, 2) != -2 || calls) return 1;
    if (Next__Ct6VIPool1Z19VIHSpriteAttachmenti(pool, 3) != 777 || calls != 1 || seen != pool) return 2;
    return 0;
}
