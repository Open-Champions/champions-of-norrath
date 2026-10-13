// Observed: __t6VIList1Z7VIActor at 0x01224368 is 16 bytes and returns nothing. It stores
// words at byte offsets 0=0, 4=0, 8=0 of its object and touches nothing else.
// Inferred: this is the default constructor of the VIList instance named by the symbol; the
// leading words are the empty-container state.
// Unknown: which offset is which named member, and the element type's size.
extern "C" {
void __t6VIList1Z7VIActor(void* object) {
    int* w = static_cast<int*>(object);
    w[0] = 0;
    w[1] = 0;
    w[2] = 0;
}
}
