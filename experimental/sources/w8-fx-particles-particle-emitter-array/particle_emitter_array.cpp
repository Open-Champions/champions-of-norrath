// ParticleEmitterArray at 0x006e93e0, 2432 BSS bytes (the gap to DynamicLightArray),
// zero in the file image. worldInitWorld__FP12_worldHeader indexes it as 16 records
// of 152 bytes and compares a record with __eq__19ParticleEmitterDataRC19ParticleEmitterData
// (which reads the first 17 bytes); 16 times 152 is 2432. Record contents past
// that are not established here. The reviewed -G0 profile places it in BSS.
extern "C" {
unsigned char ParticleEmitterArray[16 * 152];
}
static_assert(sizeof(ParticleEmitterArray) == 2432u, "ParticleEmitterArray extent");
