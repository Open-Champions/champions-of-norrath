// Host contract for chestResourceList. Not an EE plan input. Build with the table source and this
// file; the eight view maps are defined here.
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" {
extern ResourceSlot* chestResourceList[8];
ResourceSlot norrath_ctors_ant_lantern_view_42c760[1], norrath_ctors_ant_lantern_view_42c788[1],
    norrath_ctors_ant_lantern_view_42c7b0[1], norrath_ctors_ant_lantern_view_42c7d8[1],
    norrath_ctors_ant_lantern_view_42c800[1], norrath_ctors_ant_lantern_view_42c828[1],
    norrath_ctors_ant_lantern_view_42c850[1], norrath_ctors_ant_lantern_view_42c878[1];
}
int main() {
    ResourceSlot* expected[8] = {norrath_ctors_ant_lantern_view_42c760, norrath_ctors_ant_lantern_view_42c788,
                                 norrath_ctors_ant_lantern_view_42c7b0, norrath_ctors_ant_lantern_view_42c7d8,
                                 norrath_ctors_ant_lantern_view_42c800, norrath_ctors_ant_lantern_view_42c828,
                                 norrath_ctors_ant_lantern_view_42c850, norrath_ctors_ant_lantern_view_42c878};
    if (sizeof(chestResourceList) != 8u * sizeof(void*)) return 1;
    for (int i = 0; i < 8; ++i)
        if (chestResourceList[i] != expected[i]) return 2;
    return 0;
}
