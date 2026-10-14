// ghostObjectGrid at 0x0050a1c0, 57600 bytes, to stringHash.
// 120 by 120 words. BSS is zero. objectInit clears it later.
// object-infra publishes the same bytes as norrath_constructor_grid_b_view.
// This is the original name, so the link has two objects.
extern "C" {
unsigned char ghostObjectGrid[120 * 120 * 4];
}
static_assert(sizeof ghostObjectGrid == 57600u, "ghostObjectGrid extent");
