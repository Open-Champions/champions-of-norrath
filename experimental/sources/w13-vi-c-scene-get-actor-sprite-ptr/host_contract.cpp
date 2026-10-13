// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
// Pointer words are native width on the host; the entry stride (12) is the image's.
#include "scene-get-actor-sprite-ptr.cpp"
#include <cstring>

int main() {
    alignas(8) unsigned char scene[1100] = {0};
    alignas(8) unsigned char actor[128] = {0};
    alignas(8) unsigned char table[12 * 6] = {0};
    int sprite_a = 1, sprite_b = 2, sprite_c = 3;
    void* ptrs[3] = {&sprite_a, &sprite_b, &sprite_c};
    unsigned char* t = table;
    std::memcpy(scene + 1004, &t, sizeof t);
    std::memcpy(table + 0, &ptrs[0], sizeof(void*));
    std::memcpy(table + 12 * 2, &ptrs[1], sizeof(void*));
    std::memcpy(table + 12 * 5, &ptrs[2], sizeof(void*));
    int index = 0;
    std::memcpy(actor + 96, &index, 4);
    if (GetActorSpritePtr__C7VIScenei(scene, actor) != &sprite_a) return 1;
    index = 2; std::memcpy(actor + 96, &index, 4);
    if (GetActorSpritePtr__C7VIScenei(scene, actor) != &sprite_b) return 2;
    index = 5; std::memcpy(actor + 96, &index, 4);
    if (GetActorSpritePtr__C7VIScenei(scene, actor) != &sprite_c) return 3;
    return 0;
}
