// trail at 0x00780b00, 66560 bytes, to cutCameraPos.
// 64 records of 1040. BSS is zero. initTrails clears the first halfword later.
// Two other local ELF symbols named trail, at 0x008d7040 and 0x008d7090,
// are not this object.
extern "C" {
unsigned char trail[64 * 1040];
}
static_assert(sizeof trail == 66560u, "trail extent");
