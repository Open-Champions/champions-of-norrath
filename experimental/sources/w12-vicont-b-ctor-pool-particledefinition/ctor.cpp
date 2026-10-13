// Observed: __t6VIPool1ZP20VIParticleDefinition at 0x01208f08, 32 bytes; returns nothing. Stores 0 at +0, -1 at +20, 0 at +4, 0 at +8, -1 at +12 and -1 at +16 of its
// object and touches nothing else.
// Inferred: the default constructor of this VIPool instance; the zero words are the empty state and the -1 words read as empty indices.
// Unknown: which offset is which named member beyond their use by Clear and Init.
extern "C" {
void __t6VIPool1ZP20VIParticleDefinition(void* object) {
    int* words = static_cast<int*>(object);
    words[0] = 0;
    words[5] = -1;
    words[1] = 0;
    words[2] = 0;
    words[3] = -1;
    words[4] = -1;
}
}
