// Observed .lit4 word at 0x48001c. The collision-list code loads one float from
// it. The bits are 0x3bcccccc, written here as little-endian bytes.
// Unknown: whether other literals share this pool entry.
extern "C" {
extern const unsigned char norrath_collision_grid_scale_bytes[4];
const unsigned char norrath_collision_grid_scale_bytes[4] = {0xcc, 0xcc, 0xcc, 0x3b};
}
