// Host check only; not a build input.
#include "get.cpp"
int main() {
    alignas(8) unsigned char scene[2048];
    for (int i = 0; i < 2048; ++i) scene[i] = static_cast<unsigned char>(i * 7 + 3);
    unsigned char before[2048];
    for (int i = 0; i < 2048; ++i) before[i] = scene[i];
    const int values[] = {0, 1, -1, 0x12345678, -0x7fffffff};
    for (int v : values) {
        *reinterpret_cast<int*>(scene + 1144) = v;
        before[1144] = scene[1144]; before[1144+1] = scene[1144+1]; before[1144+2] = scene[1144+2]; before[1144+3] = scene[1144+3];
        if (GetStreamAudioUISprite__7VIScene(scene) != v) return 1;
    }
    for (int i = 0; i < 2048; ++i) if (scene[i] != before[i]) return 2;
    return 0;
}
