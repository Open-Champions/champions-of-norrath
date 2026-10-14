// Host contract for the map storage. Not an EE plan input.
#include "storage.cpp"

#include <cstdio>

static_assert(sizeof(mapButtonWasDown) == 16, "four words");
static_assert(sizeof(mapScale) == 4 && sizeof(mapX) == 4 && sizeof(mapY) == 4, "three floats");

int main() {
    int failures = 0;
    for (int v : mapButtonWasDown) failures += v != 0;
    failures += mapObjList != nullptr;
    failures += mapScale != 0.0f || mapX != 0.0f || mapY != 0.0f;
    if (failures) std::fprintf(stderr, "fail: storage starts zeroed\n");
    return failures ? 1 : 0;
}
