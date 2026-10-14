// Host check only; not a build input.
#include "collide.cpp"
static unsigned char *gw, *gv; static int gs, calls, ret, bump; static unsigned seenStamp;
extern "C" int Collide__7VIWorldiR18VISceneCollRayVars(unsigned char* w, int s, unsigned char* v) {
    gw = w; gs = s; gv = v; ++calls; seenStamp = *reinterpret_cast<unsigned*>(v);
    *reinterpret_cast<unsigned*>(w + 68) += bump;
    return ret;
}
int main() {
    alignas(8) unsigned char world[96] = {}, vars[32] = {};
    const unsigned flags[] = {0, 1, 0x80000000u};
    const int rets[] = {0, 1, -1, 77};
    const unsigned stamps[] = {0, 5, 0xffffffffu};
    for (unsigned f : flags) for (int r : rets) for (unsigned st : stamps) for (int b = 0; b <= 3; b += 3) {
        *reinterpret_cast<unsigned*>(world + 52) = f;
        *reinterpret_cast<unsigned*>(world + 68) = st;
        *reinterpret_cast<unsigned*>(vars) = 0x12345678; ret = r; bump = b; calls = 0;
        int got = Collide__7VIWorldR18VISceneCollRayVars(world, vars);
        if (got != (r != 0 ? 1 : 0)) return 1;
        if (calls != 1 || gw != world || gv != vars) return 2;
        if (gs != (f != 0 ? 0 : 0x40000000)) return 3;
        if (seenStamp != st || *reinterpret_cast<unsigned*>(vars) != st) return 4;
        if (*reinterpret_cast<unsigned*>(world + 68) != st + b + 1u) return 5;
    }
    return 0;
}
