// Observed bytes at 0x485470 in .sdata, through the first NUL.
// hasLightOrParticle compares 4 bytes and walks this storage until NUL,
// folding case into the same bytes. The following ".lmp" literal is separate.
// The bytes are published writable because that walk stores.
extern "C" {
unsigned char norrath_factory_halo_tag_bytes[] = {
    0x48, 0x61, 0x6c, 0x6f, 0x00
};
}
