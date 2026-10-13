// Host check only; not a build input.
#include "collide.cpp"
static unsigned char* gC; static unsigned char* gS; static int gO, calls, ret;
extern "C" int CollideObj__9VICollideR12VICollSpherei(unsigned char* c, unsigned char* s, int o) { gC = c; gS = s; gO = o; ++calls; return ret; }
int main() {
    alignas(8) unsigned char self[96], shape[8], filt[8], scene[8], coll[8];
    for (int i = 0; i < 96; ++i) self[i] = static_cast<unsigned char>(i * 3 + 1);
    const int rets[] = {0, 1, -7, 1234};
    for (int k = 0; k < 4; ++k) {
        *reinterpret_cast<int*>(self + 68) = 0x1000 + k * 77 - 5;
        calls = 0; ret = rets[k];
        if (Collide__14VISimpleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(self, shape, filt, scene, coll) != rets[k]) return 1;
        if (calls != 1 || gC != coll || gS != shape || gO != 0x1000 + k * 77 - 5) return 2;
    }
    return 0;
}
