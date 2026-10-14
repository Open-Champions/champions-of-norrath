// Host check only; not a build input.
#include "process_hierarchy.cpp"
static int procRet, procCalls, dtorCalls, dtorFlags, order;
static int identityAt, procAt, dtorAt;
static float seen[16];
static void* matrixSeen;
static int nodeSeen;
extern "C" void Identity__10VIMatrix44(void* m) {
    identityAt = ++order;
    const float* f = static_cast<const float*>(m);
    for (int i = 0; i < 16; ++i) if (f[i] != 0.0f) identityAt = -100;
    for (int i = 0; i < 16; ++i) static_cast<float*>(m)[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}
extern "C" int ProcessHierarchy__9VIHSpriteiR10VIMatrix44(void*, int node, void* m) {
    procAt = ++order; ++procCalls; nodeSeen = node; matrixSeen = m;
    for (int i = 0; i < 16; ++i) seen[i] = static_cast<float*>(m)[i];
    return procRet;
}
extern "C" void _$_10VIMatrix44(void* m, int flags) { dtorAt = ++order; ++dtorCalls; dtorFlags = flags; if (m != matrixSeen) dtorAt = -100; }
int main() {
    int dummy;
    procRet = 5;
    if (ProcessHierarchy__9VIHSprite(&dummy) != 0) return 1;
    if (identityAt != 1 || procAt != 2 || dtorAt != 3 || nodeSeen != 0 || dtorFlags != 2) return 2;
    for (int i = 0; i < 16; ++i) if (seen[i] != ((i % 5 == 0) ? 1.0f : 0.0f)) return 3;
    procRet = -3;
    if (ProcessHierarchy__9VIHSprite(&dummy) != -1 || dtorCalls != 2) return 4;
    return 0;
}
