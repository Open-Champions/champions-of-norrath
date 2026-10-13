#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
static void setp(unsigned char* field, const unsigned char* target) { uint32_t v = static_cast<uint32_t>(target - arena); memcpy(field, &v, 4); }
static void setw(unsigned char* field, int v) { memcpy(field, &v, 4); }
static void setf(unsigned char* field, float v) { memcpy(field, &v, 4); }

static int zoneCalls, zoneIds[8], zoneResults[8];
static unsigned char* zonePtrs[8]; static void* zoneVars[8];
extern "C" int RenderByVolume__6VIZoneR15VISceneRendVarsi(void* zone, void* vars, int index) {
    zonePtrs[zoneCalls] = static_cast<unsigned char*>(zone); zoneVars[zoneCalls] = vars; zoneIds[zoneCalls] = index;
    return zoneResults[zoneCalls++];
}
#include "render_by_volume.cpp"

static void plane(unsigned char* n, float a, float b, float c, float d, int child16, int child20) {
    setf(n, a); setf(n + 4, b); setf(n + 8, c); setf(n + 12, d); setw(n + 16, child16); setw(n + 20, child20);
}

int main() {
    alignas(16) static unsigned char world[128], vars[64], camera[64];
    unsigned char* nodes = arena + 0x100;          // 24-byte records
    unsigned char* intArray = arena + 0x400;
    unsigned char* zones = arena + 0x1000;         // 592-byte records
    unsigned char* points = arena + 0x600;
    const int Z = 0x40000000;
    struct Case { int node; int pointCount; float px[3]; float eyeX; int r[4]; int expectRet; int expectCalls; int expectIds[4]; } cases[] = {
        // behind, only front points: zone of child 16 (array value 11), then done
        {0, 1, {10, 0, 0}, 0, {5, 5, 5, 5}, 0, 1, {11, 0, 0, 0}},
        {0, 1, {10, 0, 0}, 0, {-3, 5, 5, 5}, -1, 1, {11, 0, 0, 0}},
        // behind, front and back: child 16 then child 20
        {0, 2, {10, 1, 0}, 0, {1, 2, 5, 5}, 0, 2, {11, 22, 0, 0}},
        {0, 2, {10, 1, 0}, 0, {-1, 2, 5, 5}, -1, 1, {11, 0, 0, 0}},
        {0, 2, {10, 1, 0}, 0, {1, -2, 5, 5}, -1, 2, {11, 22, 0, 0}},
        // not behind: child 20 first, then child 16
        {0, 2, {10, 1, 0}, 9, {1, 2, 5, 5}, 0, 2, {22, 11, 0, 0}},
        {0, 2, {10, 1, 0}, 9, {-1, 2, 5, 5}, -1, 1, {22, 0, 0, 0}},
        {0, 2, {10, 1, 0}, 9, {1, -4, 5, 5}, -1, 2, {22, 11, 0, 0}},
        {0, 1, {1, 0, 0}, 9, {7, 5, 5, 5}, 0, 1, {22, 0, 0, 0}},
        // empty point list: neither child
        {0, 0, {0, 0, 0}, 0, {5, 5, 5, 5}, 0, 0, {0, 0, 0, 0}},
        // high bits without the zone bit are masked off
        {static_cast<int>(0x80000000u), 1, {10, 0, 0}, 0, {5, 5, 5, 5}, 0, 1, {11, 0, 0, 0}},
        // zone node: tail call with its result, and no node access
        {Z | 3, 1, {10, 0, 0}, 0, {42, 5, 5, 5}, 42, 1, {44, 0, 0, 0}},
        // recursion: node 1 has regular children 2 and 3
        {1, 1, {10, 0, 0}, 0, {1, 5, 5, 5}, 0, 1, {66, 0, 0, 0}},
        {1, 2, {10, 1, 0}, 0, {1, 2, 3, 4}, 0, 4, {66, 66, 66, 66}},
        {1, 2, {10, 1, 0}, 9, {1, -2, 3, 4}, -1, 2, {66, 66, 0, 0}},
    };
    for (const Case& c : cases) {
        memset(arena, 0, sizeof arena); memset(world, 0x22, sizeof world); memset(vars, 0x33, sizeof vars); memset(camera, 0, sizeof camera);
        setp(world + 56, nodes); setp(world + 64, intArray); setp(world + 76, zones);
        plane(nodes, 1, 0, 0, -5, Z | 1, Z | 2);            // front when x > 5
        plane(nodes + 24, 1, 0, 0, -5, 2, 3);               // node 1: regular children (nodes 2 and 3)
        plane(nodes + 48, 1, 0, 0, -5, Z | 5, Z | 5);       // node 2
        plane(nodes + 72, 1, 0, 0, -5, Z | 5, Z | 5);       // node 3
        setw(intArray + 4 * 1, 11); setw(intArray + 4 * 2, 22); setw(intArray + 4 * 3, 44); setw(intArray + 4 * 5, 66);
        setw(vars + 24, c.pointCount); setp(vars + 28, points); setp(vars + 12, camera);
        if (c.pointCount >= 1) { setf(points, c.px[0]); setf(points + 4, 0); setf(points + 8, 0); }
        if (c.pointCount >= 2) { setf(points + 12, c.px[1]); setf(points + 16, 0); setf(points + 20, 0); }
        setf(camera + 28, c.eyeX);
        for (int i = 0; i < 4; ++i) zoneResults[i] = c.r[i];
        zoneCalls = 0;
        int ret = RenderByVolume__7VIWorldiR15VISceneRendVars(world, c.node, vars);
        if (ret != c.expectRet) return 1;
        if (zoneCalls != c.expectCalls) return 2;
        for (int i = 0; i < zoneCalls; ++i) {
            if (zoneIds[i] != c.expectIds[i] || zonePtrs[i] != zones + 592 * c.expectIds[i] || zoneVars[i] != vars) return 3;
        }
    }
    return 0;
}
