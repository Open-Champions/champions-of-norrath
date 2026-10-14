// Host check for npcLumpName storage. Not an EE plan input.
#include <cstdio>
#include "npc_lump_name.cpp"
int main() {
    int failures = 0;
    if (sizeof(npcLumpName) != sizeof(char*) || npcLumpName != nullptr) { std::fprintf(stderr, "fail: storage\n"); ++failures; }
    if (failures) return 1;
    std::printf("npcLumpName storage check passed\n");
    return 0;
}
