// Observed in the pinned image: _12VINameSprite$NumNameDefers at 0x01190c40 (4 bytes) and
// _12VINameSprite$NameDefers at 0x01190c50 (0xd80 = 3456 bytes), both all zero in the file and
// inside the file-backed data of the VI module segment. The RasterDeferred unit sorts and draws
// 144-byte records from the array, so 3456 bytes is 24 records (see
// w11-vi-render-a-namesprite-raster-deferred for the observed record fields).
// Inferred: the count is the number of live records; zero at start. Unknown: which code appends
// records (the writers are not part of this unit) and what bounds the count.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
int norrath_name_sprite_num_defers __asm__("_12VINameSprite$NumNameDefers") NORRATH_LOADED(".data") = 0;
unsigned char norrath_name_sprite_defers[24 * 144] __asm__("_12VINameSprite$NameDefers") NORRATH_LOADED(".data") = {0};
}
