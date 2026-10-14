// Host comparison only. Not an EE build input.
extern "C" unsigned norrath_ctors_ant_lantern_view_483e5c;
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_483e5c != 4u) return 1;
    if (norrath_ctors_ant_lantern_view_483e5c != 0u) return 1;
    norrath_ctors_ant_lantern_view_483e5c = norrath_ctors_ant_lantern_view_483e5c * 0x19660du + 0x3c6ef35fu;
    return norrath_ctors_ant_lantern_view_483e5c == (0u * 0x19660du + 0x3c6ef35fu) ? 0 : 1;
}
