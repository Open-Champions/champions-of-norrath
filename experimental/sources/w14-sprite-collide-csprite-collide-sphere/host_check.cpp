// Host check only; not a build input.
#include "collide.cpp"
static unsigned char *gC, *gS, *gP; static int calls, ret;
extern "C" int CollideObj__9VICollideR12VICollSphereR9VICapsule(unsigned char* c, unsigned char* s, unsigned char* p) { gC = c; gS = s; gP = p; ++calls; return ret; }
int main() {
    alignas(8) static unsigned char self[4600]; unsigned char sphere[8], filt[8], scene[8], coll[8];
    const unsigned flags[] = {0, 1, 0x80000000u, 0x100};
    for (int k = 0; k < 4; ++k) for (int r = 0; r < 3; ++r) {
        *reinterpret_cast<unsigned*>(self + 4456) = flags[k]; calls = 0; ret = r * 5 - 3;
        int got = Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide(self, sphere, filt, scene, coll);
        if (flags[k] != 0) { if (got != 0 || calls != 0) return 1; }
        else { if (got != ret || calls != 1 || gC != coll || gS != sphere || gP != self + 4424) return 2; }
    }
    return 0;
}
