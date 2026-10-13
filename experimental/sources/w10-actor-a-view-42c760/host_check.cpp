// Host comparison only. Not an EE build input.
#include "norrath_ctors_ant_lantern_view_42c760.cpp"
#include <cstring>
int main() {
    static const char* const expected[] = {"chest.lmp", "chest_large.vif", "chest_large.tex", "chest_large_open.anm", };
    if (sizeof norrath_ctors_ant_lantern_view_42c760 != 5u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 4; ++i)
        if (norrath_ctors_ant_lantern_view_42c760[i].unused != 0u || std::strcmp(norrath_ctors_ant_lantern_view_42c760[i].name, expected[i]) != 0) return 1;
    if (norrath_ctors_ant_lantern_view_42c760[4].name != nullptr || norrath_ctors_ant_lantern_view_42c760[4].unused != 0u) return 1;
    return 0;
}
