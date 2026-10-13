#define NORRATH_HOST_CONTRACT 1
#include "get_env_flags.cpp"

#include <cstring>
#include <cstdint>

// Guest addresses are offsets into this arena.
alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }

static int find_result, find_a, find_b;
static void* find_world;
static const VIVect3* find_position;

extern "C" int FindLeaf__C7VIWorldRC7VIVect3RiT2(void* world, const VIVect3* position, int* a, int* b) {
    find_world = world;
    find_position = position;
    if (find_result >= 0) { *a = find_a; *b = find_b; }
    return find_result;
}

static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }

int main() {
    // scene at 0x100; table at 0x1000; leaf 3's element array at 0x3000.
    unsigned scene_at = 0x100, table = 0x1000, elements = 0x3000;
    std::memset(arena, 0, sizeof arena);
    put(scene_at + 80, table);
    put(table + 592 * 3 + 80, elements);
    put(elements + 8 * 5 + 4, 0xabcd1234u);
    put(elements + 8 * 4 + 4, 0x11111111u);
    put(elements + 8 * 5 + 0, 0x22222222u);
    const VIVect3* position = reinterpret_cast<const VIVect3*>(arena + 0x400);
    VIScene* scene = reinterpret_cast<VIScene*>(arena + scene_at);
    find_result = 0; find_a = 3; find_b = 5;
    if (static_cast<unsigned>(GetEnvFlags__7VISceneRC7VIVect3(scene, position)) != 0xabcd1234u) return 1;
    if (find_world != arena + scene_at + 4 || find_position != position) return 2;
    find_result = 7;
    if (static_cast<unsigned>(GetEnvFlags__7VISceneRC7VIVect3(scene, position)) != 0xabcd1234u) return 3;
    find_result = -1;
    if (GetEnvFlags__7VISceneRC7VIVect3(scene, position) != -1) return 4;
    find_result = 0; find_b = 4;
    if (GetEnvFlags__7VISceneRC7VIVect3(scene, position) != 0x11111111) return 5;
    return 0;
}
