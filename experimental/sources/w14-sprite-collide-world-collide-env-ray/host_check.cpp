// Host check only; not a build input.
#include "collide.cpp"
static unsigned char *gw, *gr, *gf, *gt; static int gs, calls, ret;
extern "C" int Collide__7VIWorldiR8VIEnvRayR7VIVect3T3(unsigned char* w, int s, unsigned char* r, unsigned char* f, unsigned char* t) { gw = w; gs = s; gr = r; gf = f; gt = t; ++calls; return ret; }
int main() {
    alignas(8) unsigned char world[96] = {}, ray[32] = {};
    const unsigned flags[] = {0, 1, 0x80000000u, 52};
    const int rets[] = {0, 1, -1, 77, 0x10000};
    for (unsigned f : flags) for (int r : rets) {
        *reinterpret_cast<unsigned*>(world + 52) = f; ret = r; calls = 0;
        int got = Collide__7VIWorldR8VIEnvRay(world, ray);
        if (got != (r != 0 ? 1 : 0)) return 1;
        if (calls != 1 || gw != world || gr != ray || gf != ray || gt != ray + 12) return 2;
        if (gs != (f != 0 ? 0 : 0x40000000)) return 3;
    }
    return 0;
}
