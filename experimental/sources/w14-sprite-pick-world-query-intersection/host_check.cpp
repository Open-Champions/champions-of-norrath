// Host check only; not a build input.
#include "query.cpp"
#include <cstring>
static unsigned char *gW, *gVars; static int gNode, calls, ret, seenFirst, bump;
extern "C" int QueryIntersection__7VIWorldiR20VISceneIntersectVars(unsigned char* w, int n, unsigned char* v) {
    ++calls; gW = w; gVars = v; gNode = n; seenFirst = *reinterpret_cast<int*>(v);
    if (bump) *reinterpret_cast<int*>(w + 68) += 100;   // the callee may change the counter: the store must use the re-read value
    return ret;
}
int main() {
    alignas(8) unsigned char world[128], vars[64];
    const int flags[] = {0, 1, -1, 0x7fffffff};
    for (int f = 0; f < 4; ++f) for (int r = 0; r < 3; ++r) for (int b = 0; b < 2; ++b) {
        std::memset(world, 0x5a, sizeof world); std::memset(vars, 0x33, sizeof vars);
        *reinterpret_cast<int*>(world + 52) = flags[f];
        *reinterpret_cast<int*>(world + 68) = 40 + f;
        calls = 0; ret = r == 0 ? -5 : r == 1 ? 0 : 9; bump = b;
        int got = QueryIntersection__7VIWorldR20VISceneIntersectVars(world, vars);
        if (calls != 1 || gW != world || gVars != vars) return 1;
        if (gNode != (flags[f] != 0 ? 0 : 0x40000000)) return 2;
        if (seenFirst != 40 + f || *reinterpret_cast<int*>(vars) != 40 + f) return 3;
        if (ret < 0) { if (got != -1 || *reinterpret_cast<int*>(world + 68) != 40 + f + (b ? 100 : 0)) return 4; }
        else { if (got != 0 || *reinterpret_cast<int*>(world + 68) != 40 + f + (b ? 100 : 0) + 1) return 5; }
    }
    return 0;
}
