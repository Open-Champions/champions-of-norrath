// Host check only; not a build input.
#include "init.cpp"
#include <cstring>
static void* gs; static const void* gb; static float seen[6];
extern "C" void Init__8VISphereRC6VIBBox(void* s, const void* b) { gs = s; gb = b; std::memcpy(seen, static_cast<const unsigned char*>(b) + 0, 12); std::memcpy(seen + 3, static_cast<const unsigned char*>(b) + 12, 12); }
int main() {
    alignas(4) unsigned char o[128], e[128];
    std::memset(o, 0x5a, sizeof o); std::memset(e, 0x5a, sizeof e);
    float box[6] = {1, 2, 3, 4, 5, 6};
    if (Init__19VIStreamAudioSpriteRC6VIBBox(o, box) != 0) return 1;
    if (gs != o + 32 || gb != o + 8) return 2;
    std::memcpy(e + 8, box, 12); std::memcpy(e + 20, box + 3, 12);
    if (std::memcmp(o, e, sizeof o)) return 3;
    return std::memcmp(seen, box, sizeof box) != 0;
}
