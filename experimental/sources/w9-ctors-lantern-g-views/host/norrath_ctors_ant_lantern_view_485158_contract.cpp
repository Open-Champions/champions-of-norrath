// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_485158[7];
int main() {
    static const char expected[7] = "%s.lmp";
    if (sizeof norrath_ctors_ant_lantern_view_485158 != 7u) return 1;
    for (unsigned i = 0; i < 7u; ++i) {
        if (norrath_ctors_ant_lantern_view_485158[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_485158[0] = static_cast<char>(norrath_ctors_ant_lantern_view_485158[0] + 1);
    return norrath_ctors_ant_lantern_view_485158[0] == expected[0] + 1 ? 0 : 1;
}
