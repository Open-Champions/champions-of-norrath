// Host check only; not a build input.
#include "query.cpp"
static int calls, lastFlags, ret; static unsigned char *lastZone, *lastVars;
extern "C" int QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars(unsigned char* z, int f, unsigned char* v) { ++calls; lastFlags = f; lastZone = z; lastVars = v; return ret; }
int main() {
    alignas(8) unsigned char zone[128] = {0};
    unsigned char vars[8];
    const int results[] = {-5, -1, 0, 1, 9};
    const int trees[] = {0, 1, -1, 0x1000};
    for (int tree : trees) {
        *reinterpret_cast<int*>(zone + 68) = tree;
        for (int r : results) {
            ret = r; calls = 0;
            int got = QueryEmitters__6VIZoneR23VISceneEmitterQueryVars(zone, vars);
            if (calls != 1 || lastZone != zone || lastVars != vars) return 1;
            if (lastFlags != (tree != 0 ? 0 : 0x40000000)) return 2;
            if (got != (r < 0 ? -1 : 0)) return 3;
        }
    }
    return 0;
}
