// Observed bytes at 0x470cc8 in .rodata, through the first NUL.
// hasLightOrParticle compares 12 bytes and walks this storage until NUL,
// folding case into the same bytes. The following literal is not part of
// this object. The bytes are published writable because that walk stores.
extern "C" {
unsigned char norrath_factory_dynamic_light_tag_bytes[] = {
    0x44, 0x79, 0x6e, 0x61, 0x6d, 0x69, 0x63, 0x4c, 0x69, 0x67, 0x68, 0x74, 0x00
};
}
