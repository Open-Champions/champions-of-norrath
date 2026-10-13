// Observed: __t7VIArray1Z13VIWorldRegion at 0x0120f920, 12 bytes; returns nothing. Stores 0 at +4 and 0 at +0 of its object and touches nothing else.
// Inferred: the default constructor of this VIArray instance; both words are the empty state.
// Unknown: which word is which named member beyond their use by Clear and Init.
extern "C" {
void __t7VIArray1Z13VIWorldRegion(void* object) {
    int* words = static_cast<int*>(object);
    words[1] = 0;
    words[0] = 0;
}
}
