#include "query_intersection.cpp"
#include <initializer_list>
#include <string.h>

static const void* storeBox; static float* storeOut; static const void* initBox; static float* initOut;
static VISceneIntersectVars snapshot; static void* seenWorld; static int worldResult, order, orderOf[3];
extern "C" void Store__C6VIBBoxP7VIVect3(const void* box, float* corners) {
    storeBox = box; storeOut = corners; orderOf[0] = ++order;
    for (int i = 0; i < 24; ++i) corners[i] = 10.0f + i;
}
extern "C" void Init__8VISphereRC6VIBBox(float* sphere, const void* box) {
    initBox = box; initOut = sphere; orderOf[1] = ++order;
    for (int i = 0; i < 4; ++i) sphere[i] = 500.0f + i;
}
extern "C" int QueryIntersection__7VIWorldR20VISceneIntersectVars(void* world, VISceneIntersectVars* vars) {
    seenWorld = world; snapshot = *vars; orderOf[2] = ++order;
    return worldResult;
}

int main() {
    alignas(16) static unsigned char scene[20000];
    float box[8] = {1, 2, 3, 4, 5, 6, 77, 88};
    unsigned char filter[8], list[8];
    for (int result : {0, 5, -1, -9}) {
        memset(scene, 0x31, sizeof scene);
        int word = 0x600d; memcpy(scene + 19364, &word, 4);
        int counter = 12; memcpy(scene + 1348, &counter, 4);
        worldResult = result; order = 0;
        int ret = QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(scene, box, filter, list);
        if (ret != (result < 0 ? -1 : 0)) return 1;
        if (seenWorld != scene + 4 || storeBox != box || initBox != box) return 2;
        if (orderOf[0] != 1 || orderOf[1] != 2 || orderOf[2] != 3) return 3;
        if (snapshot.zero != 0 || snapshot.scene != scene || snapshot.filter != filter || snapshot.triList != list || snapshot.sceneWord != 0x600d) return 4;
        for (int i = 0; i < 3; ++i) if (snapshot.minimum[i] != box[i] || snapshot.maximum[i] != box[3 + i]) return 5;
        for (int i = 0; i < 24; ++i) if (snapshot.corners[i / 3][i % 3] != 10.0f + i) return 6;
        for (int i = 0; i < 4; ++i) if (snapshot.sphere[i] != 500.0f + i) return 7;
        int now; memcpy(&now, scene + 1348, 4);
        if (now != (result < 0 ? 12 : 13)) return 9;
    }
    return 0;
}
