// Observed: __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry at 0x0120a420 is 28 bytes and returns nothing. It stores
// words at byte offsets 0=0, 4=0, 8=0, 12=0, 16=-1 of its object and touches nothing else.
// Inferred: this is the default constructor of the VIMap instance named by the symbol; the
// leading words are the empty-container state (the -1 words read as empty indices).
// Unknown: which offset is which named member, and the element type's size.
extern "C" {
void __t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* object) {
    int* w = static_cast<int*>(object);
    w[0] = 0;
    w[1] = 0;
    w[2] = 0;
    w[3] = 0;
    w[4] = -1;
}
}
