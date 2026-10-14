// Observed: __22VIParticleDefinitionEx at 0x010b4bc8 is 64 bytes. With this it calls __20VIParticleAttributes(this), then
// __t6VIPool1ZP15VIParticleMotif(this+724), then stores zero at this+748 and this+752, and one at this+760 and this+756.
// Nothing is returned.
// Inferred: this is the constructor: base attributes, a motif pool at +724 and defaults (blend mode 0, z-write off, z-test on,
// texture configuration 1).
// Unknown: the pool's capacity argument, if any (none is passed here), and whether +756 and +760 are really the offsets of the z-test flag
// and configuration (the matching setters at 0x010b8980 and 0x010b8a28 store to them).
extern "C" {
void __20VIParticleAttributes(void* self);
void __t6VIPool1ZP15VIParticleMotif(void* pool);

void __22VIParticleDefinitionEx(char* self) {
    __20VIParticleAttributes(self);
    __t6VIPool1ZP15VIParticleMotif(self + 724);
    *(int*)(self + 748) = 0;
    *(int*)(self + 752) = 0;
    *(int*)(self + 760) = 1;
    *(int*)(self + 756) = 1;
}
}
