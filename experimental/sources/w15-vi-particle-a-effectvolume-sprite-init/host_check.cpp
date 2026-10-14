// Host check only; not a build input.
#include "init.cpp"
#include <cstring>
#include <string>
static std::string tr; static unsigned char* ob; static const void* bx;
extern "C" void Init__8VISphereRC6VIBBox(void* s, const void* b) {
    tr += "S" + std::to_string(static_cast<unsigned char*>(s) - ob) + "," + std::to_string(static_cast<const unsigned char*>(b) - ob) + ";";
}
extern "C" int Init__14VIEffectVolumeRC6VIBBox(void* v, const void* b) {
    tr += "V" + std::to_string(static_cast<unsigned char*>(v) - ob) + (b == bx ? "=box;" : "=other;"); return 5;
}
int main() {
    alignas(4) unsigned char o[256], e[256];
    std::memset(o, 0x5a, sizeof o); std::memset(e, 0x5a, sizeof e);
    ob = o;
    float box[6] = {1, 2, 3, 4, 5, 6}; bx = box;
    if (Init__20VIEffectVolumeSpriteRC6VIBBox(o, box) != 0) return 1;
    if (tr != "S32,8;V88=box;") return 2;
    std::memcpy(e + 8, box, 24); std::memcpy(e + 64, box, 12); std::memcpy(e + 76, box + 3, 12);
    return std::memcmp(o, e, sizeof o) != 0;
}
