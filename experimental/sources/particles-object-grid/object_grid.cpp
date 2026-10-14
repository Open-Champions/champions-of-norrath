// objectGrid at 0x004fc0c0, 57600 bytes, to ghostObjectGrid.
// 120 by 120 words. BSS is zero. objectInit clears it later.
// object-infra publishes the same bytes as norrath_constructor_grid_a_view.
// This is the original name, so the link has two objects.
extern "C" {
unsigned char objectGrid[120 * 120 * 4];
}
static_assert(sizeof objectGrid == 57600u, "objectGrid extent");
