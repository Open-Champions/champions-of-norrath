// Host comparison only. Not an EE build input.
#include "norrath_ctors_ant_lantern_view_42c788.cpp"
#include <cstring>
int main() {
    static const char* const expected[] = {"chest2.lmp", "chest2.vif", "chest2.tex", "chest2.anm", };
    if (sizeof norrath_ctors_ant_lantern_view_42c788 != 5u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 4; ++i)
        if (norrath_ctors_ant_lantern_view_42c788[i].unused != 0u || std::strcmp(norrath_ctors_ant_lantern_view_42c788[i].name, expected[i]) != 0) return 1;
    if (norrath_ctors_ant_lantern_view_42c788[4].name != nullptr || norrath_ctors_ant_lantern_view_42c788[4].unused != 0u) return 1;
    return 0;
}
