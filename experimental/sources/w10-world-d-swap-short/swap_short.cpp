// Observed swap__H1Zs_RX01T0_v at 0x00316460, 20 bytes: a weak instantiation of the generic
// two-reference swap for short. It loads both halfwords, stores each into the other slot and
// returns; with the same pointer both loads happen first, so the value is unchanged.
// Inferred: nothing beyond the exchange. routeMakeLattice__Fv calls it to carry a node index
// along with its squared distance.
extern "C" void swap__H1Zs_RX01T0_v(short* first, short* second) {
    const short left = *first;
    const short right = *second;
    *first = right;
    *second = left;
}
