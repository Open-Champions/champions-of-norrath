// Observed in the pinned SLUS_205.65 image: two floor-sprite objects imported by name by the sprite
// add and floor-save routines.
//   spriteCount  .sdata  0x004844d8    4 bytes, initial 0 (fadePacket follows at +4)
//   sprGrid      .bss    0x007ac1f0 3600 bytes (nodes follows at +3600; floorClear clears 3600 bytes)
// Observed uses: the sprite add routine appends to a 30 by 30 grid of per-cell block pointers and
// adds one to spriteCount; the floor save-info routine walks the same grid.
// Inferred: 3600 = 30 * 30 * 4, one 4-byte pointer per cell.
// The floor-storage unit publishes the same two addresses under view names. This unit publishes the
// ELF names the callers import; one of the two pairs has to be dropped when the plans are merged.
extern "C" {
int spriteCount;
unsigned char* sprGrid[30][30];
}
