// Observed bytes at 0x470cb8 in .rodata: the text ParticleEmitter and its
// terminating NUL, 16 bytes up to the next literal (DynamicLight at 0x470cc8).
// hasLightOrParticle compares 15 bytes, lowercases the query in place, and
// reads the value from the tag text, so the object is writable.
extern "C" {
unsigned char norrath_factory_particle_emitter_tag_bytes[] = {
    0x50, 0x61, 0x72, 0x74, 0x69, 0x63, 0x6c, 0x65, 0x45, 0x6d, 0x69, 0x74, 0x74, 0x65, 0x72, 0x00
};
}
