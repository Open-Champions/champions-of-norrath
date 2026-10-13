// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_4852d8[6];
int main() {
    static const char expected[6] = "Color";
    if (sizeof norrath_ctors_ant_lantern_view_4852d8 != 6u) return 1;
    for (unsigned i = 0; i < 6u; ++i) {
        if (norrath_ctors_ant_lantern_view_4852d8[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_4852d8[0] = static_cast<char>(norrath_ctors_ant_lantern_view_4852d8[0] + 1);
    return norrath_ctors_ant_lantern_view_4852d8[0] == expected[0] + 1 ? 0 : 1;
}
