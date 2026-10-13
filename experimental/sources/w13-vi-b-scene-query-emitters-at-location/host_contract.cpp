#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" unsigned char* norrath_host_link_end() { return arena + 0xffffffffu; }
static void setp(unsigned char* field, const unsigned char* target) { uint32_t v = static_cast<uint32_t>(target - arena); memcpy(field, &v, 4); }
static void setw(unsigned char* field, int v) { memcpy(field, &v, 4); }
static void setlink_end(unsigned char* field) { uint32_t v = 0xffffffffu; memcpy(field, &v, 4); }

static int findResult, leafOut, subOut, findCalls;
static const void* findWorld; static const void* findPos;
extern "C" int FindLeaf__C7VIWorldRC7VIVect3RiT2(const void* world, const void* position, int* leaf, int* sub) {
    ++findCalls; findWorld = world; findPos = position; *leaf = leafOut; *sub = subOut; return findResult;
}
static int actorCalls, actors[8], actorResults[8];
static void* actorScene; static const void* actorCamera; static void* actorQuery;
extern "C" int QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(void* scene, int actor, const void* camera, void* query) {
    actorScene = scene; actorCamera = camera; actorQuery = query;
    actors[actorCalls] = actor;
    return actorResults[actorCalls++];
}
#include "query_emitters_at_location.cpp"

int main() {
    alignas(16) static unsigned char scene[2000], camera[64], query[16];
    // Arena layout: leaf table at 0x100 (592-byte records), entry table at 0x1000 (8-byte entries),
    // record table at 0x2000 (120-byte records), nodes at 0x4000+.
    struct Case { int find; int leaf; int sub; int nodes; int failAt; } cases[] = {
        {-1, 0, 0, 0, -1}, {0, 2, 3, 0, -1}, {0, 1, 2, 3, -1}, {5, 2, 1, 1, -1}, {0, 1, 2, 3, 1}, {0, 0, 0, 3, 0},
    };
    for (const Case& c : cases) {
        memset(arena, 0, sizeof arena);
        memset(scene, 0x21, sizeof scene);
        int counter = 40; memcpy(scene + 1348, &counter, 4);
        unsigned char* leafBase = arena + 0x100;
        unsigned char* leaf = leafBase + c.leaf * 592;
        setp(scene + 80, leafBase);
        unsigned char* entries = arena + 0x1000;
        setp(leaf + 80, entries);
        unsigned char* records = arena + 0x2000;
        setp(leaf + 96, records);
        int k = 3 + c.sub;
        setw(entries + c.sub * 8, k);
        unsigned char* rec = records + k * 120;
        unsigned char* nodeBase = arena + 0x4000;
        if (c.nodes == 0) setlink_end(rec + 104); else setp(rec + 104, nodeBase);
        for (int i = 0; i < c.nodes; ++i) {
            unsigned char* n = nodeBase + i * 32;
            setw(n + 4, 100 + i);
            if (i + 1 < c.nodes) setp(n + 16, nodeBase + (i + 1) * 32); else setlink_end(n + 16);
        }
        for (int i = 0; i < 8; ++i) actorResults[i] = (i == c.failAt) ? -2 : i;
        findResult = c.find; leafOut = c.leaf; subOut = c.sub; findCalls = 0; actorCalls = 0;
        int ret = QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery(scene, camera, query);
        if (findCalls != 1 || findWorld != scene + 4 || findPos != camera + 28) return 1;
        int counterNow; memcpy(&counterNow, scene + 1348, 4);
        if (c.find < 0) {
            if (ret != -1 || actorCalls != 0 || counterNow != 40) return 2;
        } else if (c.failAt >= 0) {
            if (ret != -1 || actorCalls != c.failAt + 1 || counterNow != 40) return 3;
        } else {
            if (ret != 0 || actorCalls != c.nodes || counterNow != 41) return 4;
        }
        for (int i = 0; i < actorCalls; ++i) if (actors[i] != 100 + i) return 5;
        if (actorCalls && (actorScene != scene || actorCamera != camera || actorQuery != query)) return 6;
    }
    return 0;
}
