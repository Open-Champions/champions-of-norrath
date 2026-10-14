// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "end_char.cpp"
#include <cstdio>
int main() {
    int& n = *reinterpret_cast<int*>(arena + 36);
    n = 0;
    bool ok = EndChar__7VIRFont(arena) == 0 && n == 1;
    n = 41; ok = ok && EndChar__7VIRFont(arena) == 0 && n == 42;
    n = -1; ok = ok && EndChar__7VIRFont(arena) == 0 && n == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
