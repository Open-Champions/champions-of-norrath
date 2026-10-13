// Observed: __t7VIArray1Zi at 0x0120f9f0 is 12 bytes and returns nothing. It stores
// words at byte offsets 0=0, 4=0 of its object and touches nothing else.
// Inferred: this is the default constructor of the VIArray instance named by the symbol; the
// leading words are the empty-container state.
// Unknown: which offset is which named member, and the element type's size.
extern "C" {
void __t7VIArray1Zi(void* object) {
    int* w = static_cast<int*>(object);
    w[0] = 0;
    w[1] = 0;
}
}
