// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_485180[7];
int main() {
    static const char expected[7] = "%s.anm";
    if (sizeof norrath_ctors_ant_lantern_view_485180 != 7u) return 1;
    for (unsigned i = 0; i < 7u; ++i) {
        if (norrath_ctors_ant_lantern_view_485180[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_485180[0] = static_cast<char>(norrath_ctors_ant_lantern_view_485180[0] + 1);
    return norrath_ctors_ant_lantern_view_485180[0] == expected[0] + 1 ? 0 : 1;
}
