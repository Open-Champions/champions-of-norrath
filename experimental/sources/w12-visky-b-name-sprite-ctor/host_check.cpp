#include "ctor.cpp"

#include <cstring>

unsigned char _vt$12VINameSprite[16];
static int order[4];
static void* seen[4];
static int n;
static unsigned char snapshot[128];
extern "C" void __8VISprite(void* p) { order[n] = 1; seen[n++] = p; }
extern "C" int Clear__12VINameSprite(void* p) {
    order[n] = 2; seen[n++] = p;
    std::memcpy(snapshot, p, sizeof snapshot);
    return 0;
}

int main() {
    alignas(8) static unsigned char object[128];
    std::memset(object, 0xee, sizeof object);
    __12VINameSprite(object);
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (seen[0] != object || seen[1] != object) return 2;
    // Pointers are 8 bytes on the host, so only the low half of the vtable word is comparable.
    unsigned low = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$12VINameSprite));
    unsigned got;
    std::memcpy(&got, snapshot + 60, 4);
    if (got != low) return 3;
    for (int i = 100; i < 116; i += 4) {
        float f;
        std::memcpy(&f, snapshot + i, 4);
        if (f != 0.0f) return 4;
        unsigned w;
        std::memcpy(&w, snapshot + i, 4);
        if (w != 0) return 5;
    }
    for (int i = 0; i < 128; ++i)
        if (!(i >= 60 && i < 64) && !(i >= 100 && i < 116) && object[i] != 0xee) return 6;
    return 0;
}
