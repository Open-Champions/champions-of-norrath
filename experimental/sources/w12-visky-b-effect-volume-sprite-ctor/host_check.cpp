#include "ctor.cpp"

#include <cstring>

unsigned char _vt$20VIEffectVolumeSprite[16];
static int order[4];
static void* seen[4];
static int n;
static unsigned char snapshot[128];
extern "C" void __8VISprite(void* p) { order[n] = 1; seen[n++] = p; }
extern "C" void __14VIEffectVolume(void* p) {
    order[n] = 2; seen[n++] = p;
    std::memcpy(snapshot, static_cast<unsigned char*>(p) - 88, sizeof snapshot);
}

int main() {
    alignas(8) static unsigned char object[128];
    std::memset(object, 0xee, sizeof object);
    __20VIEffectVolumeSprite(object);
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (seen[0] != object || seen[1] != object + 88) return 2;
    unsigned low = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$20VIEffectVolumeSprite));
    unsigned got;
    std::memcpy(&got, snapshot + 60, 4);
    if (got != low) return 3;
    for (int i = 64; i < 88; i += 4) {
        unsigned w;
        std::memcpy(&w, snapshot + i, 4);
        if (w != 0) return 4;
    }
    // The type word is written after the member constructor ran.
    unsigned type_at_call;
    std::memcpy(&type_at_call, snapshot + 52, 4);
    if (type_at_call == 14) return 5;
    int type;
    std::memcpy(&type, object + 52, 4);
    if (type != 14) return 6;
    for (int i = 0; i < 128; ++i)
        if (!(i >= 52 && i < 56) && !(i >= 60 && i < 88) && object[i] != 0xee) return 7;
    return 0;
}
