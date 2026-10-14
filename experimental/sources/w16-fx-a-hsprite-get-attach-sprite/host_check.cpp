// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "get_attach_sprite.cpp"
int main() {
    alignas(8) static unsigned char object[128];
    *reinterpret_cast<unsigned*>(object + 104) = arena_base + 0x200;
    for (int i = 0; i < 4; ++i) *reinterpret_cast<int*>(arena + 0x200 + 16 * i) = 100 + i;
    *reinterpret_cast<int*>(arena + 0x200 + 4) = 999;
    if (GetAttachSprite__C9VIHSpritei(object, 0) != 100) return 1;
    if (GetAttachSprite__C9VIHSpritei(object, 3) != 103) return 2;
    *reinterpret_cast<int*>(arena + 0x200 - 16) = -5;
    if (GetAttachSprite__C9VIHSpritei(object, -1) != -5) return 3;
    return 0;
}
