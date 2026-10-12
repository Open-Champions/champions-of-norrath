// nmParticleEmitterRecs at 0x004869c0, four .sbss bytes, zero in the file image
// (symbol table: local BSS object; extent is the gap to closeGoalR2). worldInitWorld
// resets it to zero and counts emitter records into ParticleEmitterArray, stopping
// at 16, so it is a signed count. The reviewed -G0 profile places it in BSS.
extern "C" {
int nmParticleEmitterRecs;
}
static_assert(sizeof(nmParticleEmitterRecs) == 4u, "nmParticleEmitterRecs extent");
