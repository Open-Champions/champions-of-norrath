// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_485190[5];
int main() {
    static const char expected[5] = "path";
    if (sizeof norrath_ctors_ant_lantern_view_485190 != 5u) return 1;
    for (unsigned i = 0; i < 5u; ++i) {
        if (norrath_ctors_ant_lantern_view_485190[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_485190[0] = static_cast<char>(norrath_ctors_ant_lantern_view_485190[0] + 1);
    return norrath_ctors_ant_lantern_view_485190[0] == expected[0] + 1 ? 0 : 1;
}
