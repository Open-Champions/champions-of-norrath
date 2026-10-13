#include "collide_coll_ray.cpp"
#include <string.h>

static int calls;
static void* seenWorld;
static VISceneCollRayVars snapshot;
extern "C" int Collide__7VIWorldR18VISceneCollRayVars(void* world, VISceneCollRayVars* vars) {
    ++calls; seenWorld = world; snapshot = *vars;
    return 91;
}

int main() {
    alignas(16) static unsigned char scene[20000];
    float ray[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned char filter[16];
    memset(scene, 0x21, sizeof scene);
    int word = 0x1357; memcpy(scene + 19364, &word, 4);
    int counter = 20; memcpy(scene + 1348, &counter, 4);
    if (Collide__7VISceneR9VICollRayR13VISceneFilter(scene, ray, filter) != 91 || calls != 1) return 1;
    if (seenWorld != scene + 4) return 2;
    if (snapshot.zero != 0 || snapshot.scene != scene || snapshot.filter != filter || snapshot.ray != ray || snapshot.sceneWord != 0x1357) return 3;
    for (int i = 0; i < 3; ++i)
        if (snapshot.first[i] != ray[i] || snapshot.second[i] != ray[3 + i]) return 4;
    memcpy(&counter, scene + 1348, 4);
    if (counter != 21) return 5;
    return 0;
}
