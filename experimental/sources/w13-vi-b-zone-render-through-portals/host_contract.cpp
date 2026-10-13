#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
static void setp(unsigned char* field, const unsigned char* target) { uint32_t v = static_cast<uint32_t>(target - arena); memcpy(field, &v, 4); }
static void setw(unsigned char* field, int v) { memcpy(field, &v, 4); }
static int getw(const unsigned char* field) { int v; memcpy(&v, field, 4); return v; }
static void setf(unsigned char* field, float v) { memcpy(field, &v, 4); }

enum { CLIP = 1, PUSH, GETTOP, ROOM, POP, FRUSTUM };
static int events[64], eventCount, eventArg[64];
static int clipResult, pushResult, roomResult, popResult, frustumResult;
static int roomWasFlagged[8];
static unsigned char* portalsForHook;
extern "C" int ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(void*, int count, void*, int* outCount, void** outVertices) {
    events[eventCount] = CLIP; eventArg[eventCount++] = count; *outCount = 7; *outVertices = arena + 0x7000; return clipResult;
}
extern "C" int PushPortal__11VIClipStackR8VICameraiP7VIVect3(void*, void*, int count, void* vertices) {
    events[eventCount] = PUSH; eventArg[eventCount++] = count * 1000 + (vertices == arena + 0x7000); return pushResult;
}
extern "C" int GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(void*, int*, void**, int*, void**) { events[eventCount] = GETTOP; eventArg[eventCount++] = 0; return 0; }
extern "C" int RenderRoom__6VIZoneiR15VISceneRendVars(void*, int room, void*) {
    events[eventCount] = ROOM; eventArg[eventCount++] = room;
    roomWasFlagged[room & 7] = getw(portalsForHook + (room - 2) * 32 + 28) & 1;
    return roomResult;
}
extern "C" int Pop__11VIClipStack(void*) { events[eventCount] = POP; eventArg[eventCount++] = 0; return popResult; }
extern "C" int RenderByFrustum__7VIWorldR15VISceneRendVars(void*, void*) { events[eventCount] = FRUSTUM; eventArg[eventCount++] = 0; return frustumResult; }
#include "render_through_portals.cpp"

int main() {
    alignas(16) static unsigned char zone[128], vars[64], camera[64];
    unsigned char* rooms = arena + 0x100;          // 120-byte records
    unsigned char* portals = arena + 0x1000;       // 32-byte records for room 1
    unsigned char* verts = arena + 0x2000;
    // Scenario table by hand: three portals with distances {far, behind, near}.
    // Portal 0: dist 1.0 (clip, push, gettop, room, pop, gettop). Portal 1: behind (skipped).
    // Portal 2: dist 0.0 (near; no clip/push/pop). Target rooms 2, 3, 4.
    for (int variant = 0; variant < 6; ++variant) {
        memset(arena, 0, sizeof arena); memset(zone, 0, sizeof zone); memset(vars, 0, sizeof vars); memset(camera, 0, sizeof camera);
        setp(zone + 96, rooms);
        unsigned char* room1 = rooms + 120;
        setw(room1 + 76, 3); setp(room1 + 92, portals); setp(room1 + 96, verts);
        for (int p = 0; p < 3; ++p) {
            unsigned char* pl = portals + 32 * p;
            setf(pl, 0); setf(pl + 4, 0); setf(pl + 8, 1);       // normal (0,0,1)
            setf(pl + 12, p == 0 ? 1.0f : (p == 1 ? -5.0f : 0.0f));
            setw(pl + 20, 3 + p); setw(pl + 24, 2 + p); setw(pl + 28, 0);
        }
        setf(camera + 28, 0); setf(camera + 32, 0); setf(camera + 36, 0);
        setp(vars + 12, camera); setp(vars + 20, arena + 0x3000); setp(vars + 8, arena + 0x3800);
        setw(vars + 40, variant == 4 ? 1 : 0);
        for (int r = 2; r <= 4; ++r) setw(rooms + 120 * r, (variant >= 3) ? 1 : 0);
        clipResult = variant == 1 ? 0 : 1; pushResult = 0; roomResult = 0; popResult = 0; frustumResult = variant == 5 ? 1 : 0;
        eventCount = 0; portalsForHook = portals;
        int ret = RenderThroughPortals__6VIZoneiR15VISceneRendVars(zone, 1, vars);
                // Variant 0: all portals open.
        if (variant == 0) {
            if (ret != 0) return 1;
            int expect[] = {CLIP, PUSH, GETTOP, ROOM, POP, GETTOP, ROOM};
            // Portal 0: clip(3), push(7*1000+1), gettop, room(2), pop, gettop; portal 2 is near: room(4).
            if (eventCount != 7) return 2;
            for (int i = 0; i < 7; ++i) if (events[i] != expect[i]) return 3;
            if (eventArg[0] != 3 || eventArg[1] != 7001 || eventArg[3] != 2 || eventArg[6] != 4) return 4;
            for (int p = 0; p < 3; ++p) if (getw(portals + 32 * p + 28) != 0) return 5;
            if (!roomWasFlagged[2] || !roomWasFlagged[4]) return 10;
        }
        if (variant == 1) {   // clip rejects the far portal: only the near one recurses
            if (ret != 0 || eventCount != 2 || events[0] != CLIP || events[1] != ROOM || eventArg[1] != 4) return 6;
        }
        if (variant == 3) {   // targets have bit 0 set: frustum for each non-skipped portal
            if (ret != 0 || eventCount != 3 || events[0] != CLIP || events[1] != FRUSTUM || events[2] != FRUSTUM) return 7;
        }
        if (variant == 4) {   // same with V+40 set: no frustum calls
            if (ret != 0 || eventCount != 1 || events[0] != CLIP) return 8;
        }
        if (variant == 5) {   // frustum reports nonzero: return -1 after the first
            if (ret != -1 || eventCount != 2) return 9;
        }
    }
    return 0;
}
