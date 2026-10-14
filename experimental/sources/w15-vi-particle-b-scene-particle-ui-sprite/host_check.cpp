// Host check only; not a build input.
#include "scene_particle_ui_sprite.cpp"
#include <cstdio>
#include <cstring>
int main() {
    alignas(16) unsigned char object[2048];
    std::memset(object, 0x5a, sizeof object);
    for (int k = 0; k < 3; ++k) {
        int v = {};
        v = (int)(0x1234567 + k * 77);
        std::memcpy(object + 1136, &v, 4);
        if (GetParticleUISprite__7VIScene(object) != v) { std::puts("FAIL"); return 1; }
    }
    std::puts("ok");
    return 0;
}
