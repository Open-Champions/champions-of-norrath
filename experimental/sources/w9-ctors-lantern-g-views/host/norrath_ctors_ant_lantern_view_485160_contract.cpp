// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_485160[8];
int main() {
    static const char expected[8] = "wakname";
    if (sizeof norrath_ctors_ant_lantern_view_485160 != 8u) return 1;
    for (unsigned i = 0; i < 8u; ++i) {
        if (norrath_ctors_ant_lantern_view_485160[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_485160[0] = static_cast<char>(norrath_ctors_ant_lantern_view_485160[0] + 1);
    return norrath_ctors_ant_lantern_view_485160[0] == expected[0] + 1 ? 0 : 1;
}
