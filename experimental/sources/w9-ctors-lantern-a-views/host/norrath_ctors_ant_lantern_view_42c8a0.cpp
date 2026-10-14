// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" {
ResourceSlot norrath_ctors_ant_lantern_view_42c760[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c788[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c7b0[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c7d8[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c800[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c828[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c850[1];
ResourceSlot norrath_ctors_ant_lantern_view_42c878[1];
extern ResourceSlot* norrath_ctors_ant_lantern_view_42c8a0[8];
}
int main() {
    if (sizeof norrath_ctors_ant_lantern_view_42c8a0 != 8u * sizeof(ResourceSlot*)) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[0] != norrath_ctors_ant_lantern_view_42c760) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[1] != norrath_ctors_ant_lantern_view_42c788) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[2] != norrath_ctors_ant_lantern_view_42c7b0) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[3] != norrath_ctors_ant_lantern_view_42c7d8) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[4] != norrath_ctors_ant_lantern_view_42c800) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[5] != norrath_ctors_ant_lantern_view_42c828) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[6] != norrath_ctors_ant_lantern_view_42c850) return 1;
    if (norrath_ctors_ant_lantern_view_42c8a0[7] != norrath_ctors_ant_lantern_view_42c878) return 1;
    return 0;
}
