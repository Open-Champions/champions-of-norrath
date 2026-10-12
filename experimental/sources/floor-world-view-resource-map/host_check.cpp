#include <cstdio>
#include <cstdint>

extern "C" void __11ResourceMapPcT1(void* map, char* group, char* name);

int main() {
    alignas(4) unsigned words[4] = {1u, 2u, 3u, 0x11111111u};
    char group[] = "engine.lmp";
    char name[] = "floorsprite.tex";
    __11ResourceMapPcT1(words, group, name);
    const auto group_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(group));
    const auto name_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(name));
    if (words[0] != 0xffffffffu) return 1;
    if (words[1] != group_bits) return 2;
    if (words[2] != name_bits) return 3;
    if (words[3] != 0x11111111u) return 4;
    if (group_bits == name_bits) return 5;
    return 0;
}
