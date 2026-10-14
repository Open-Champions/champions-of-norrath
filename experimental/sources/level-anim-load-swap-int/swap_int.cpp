// Observed swap__H1Zi_RX01T0_v at 0x003131a0, 20 bytes: a weak instantiation of the
// generic two-reference swap for int. It loads both words, stores each into the other
// slot, and returns. specialparse calls it to exchange the width and height words.
// Inferred: nothing beyond the word exchange; this is the import name used by the
// megatile sources, not an original symbol.
// Unknown: whether the two pointers may alias; with the same pointer both loads happen
// first, so the word is unchanged, which this body reproduces.
extern "C" void norrath_swap_int(int* first, int* second) {
    const int left = *first;
    const int right = *second;
    *first = right;
    *second = left;
}
