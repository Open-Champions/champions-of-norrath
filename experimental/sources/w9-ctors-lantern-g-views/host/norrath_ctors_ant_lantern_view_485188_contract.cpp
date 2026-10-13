// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_485188[3];
int main() {
    static const char expected[3] = "%f";
    if (sizeof norrath_ctors_ant_lantern_view_485188 != 3u) return 1;
    for (unsigned i = 0; i < 3u; ++i) {
        if (norrath_ctors_ant_lantern_view_485188[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_485188[0] = static_cast<char>(norrath_ctors_ant_lantern_view_485188[0] + 1);
    return norrath_ctors_ant_lantern_view_485188[0] == expected[0] + 1 ? 0 : 1;
}
