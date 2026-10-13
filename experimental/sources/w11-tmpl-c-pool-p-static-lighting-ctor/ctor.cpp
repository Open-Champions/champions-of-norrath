// Observed: __t6VIPool1ZP16VIStaticLighting at 0x01224600 is 32 bytes and returns nothing. It stores
// words at byte offsets 0=0, 4=0, 8=0, 12=-1, 16=-1, 20=-1 of its object and touches nothing else.
// Inferred: this is the default constructor of the VIPool instance named by the symbol; the
// leading words are the empty-container state (the -1 words read as empty indices).
// Unknown: which offset is which named member, and the element type's size.
extern "C" {
void __t6VIPool1ZP16VIStaticLighting(void* object) {
    int* w = static_cast<int*>(object);
    w[0] = 0;
    w[1] = 0;
    w[2] = 0;
    w[3] = -1;
    w[4] = -1;
    w[5] = -1;
}
}
