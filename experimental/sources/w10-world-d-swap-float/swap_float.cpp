// Observed swap__H1Zf_RX01T0_v at 0x00316478, 20 bytes: a weak instantiation of the generic
// two-reference swap for float. It loads both values, stores each into the other slot and
// returns; with the same pointer both loads happen first, so the value is unchanged.
// Inferred: nothing beyond the exchange. routeMakeLattice__Fv calls it to carry the larger of two
// squared distances along an insertion pass.
extern "C" void swap__H1Zf_RX01T0_v(float* first, float* second) {
    const float left = *first;
    const float right = *second;
    *first = right;
    *second = left;
}
