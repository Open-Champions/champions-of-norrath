// Host comparison only. Not an EE build input.
extern "C" unsigned norrath_ctors_ant_lantern_view_483f88;
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_483f88 != 4u) return 1;
    if (norrath_ctors_ant_lantern_view_483f88 != 1u) return 1;
    norrath_ctors_ant_lantern_view_483f88 = norrath_ctors_ant_lantern_view_483f88 * 0x19660du + 0x3c6ef35fu;
    return norrath_ctors_ant_lantern_view_483f88 == (1u * 0x19660du + 0x3c6ef35fu) ? 0 : 1;
}
