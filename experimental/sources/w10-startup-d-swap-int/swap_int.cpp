// Observed swap__H1Zi_RX01T0_v at 0x003131a0, 20 bytes (weak): the instantiation of a
// two-reference swap for int. Both words are loaded first, then each is stored into the
// other slot. With the same address for both arguments the word is unchanged.
// The name is the image's own symbol; the megatile sources use a different import name
// (norrath_swap_int) for the same behavior.
// Nothing here was executed on the EE.
extern "C" void swap__H1Zi_RX01T0_v(int& first, int& second) {
    const int left = first;
    const int right = second;
    first = right;
    second = left;
}
