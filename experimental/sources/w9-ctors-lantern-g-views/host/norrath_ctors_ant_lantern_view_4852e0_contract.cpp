// Host comparison only. Not an EE build input.
extern "C" char norrath_ctors_ant_lantern_view_4852e0[5];
int main() {
    static const char expected[5] = "Type";
    if (sizeof norrath_ctors_ant_lantern_view_4852e0 != 5u) return 1;
    for (unsigned i = 0; i < 5u; ++i) {
        if (norrath_ctors_ant_lantern_view_4852e0[i] != expected[i]) return 1;
    }
    norrath_ctors_ant_lantern_view_4852e0[0] = static_cast<char>(norrath_ctors_ant_lantern_view_4852e0[0] + 1);
    return norrath_ctors_ant_lantern_view_4852e0[0] == expected[0] + 1 ? 0 : 1;
}
