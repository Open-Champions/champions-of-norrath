// Observed: dpadd at 0x002fb5c0 (88 bytes), dpmul at 0x002fb680 (644 bytes), dpcmp at
// 0x002fba48 (76 bytes) and dptofp at 0x002fbc20 (84 bytes) are the four soft-double entry
// points that the lava-wraith constructors, the water-spout constructor and the auto-map
// drawing import under the address-suffixed names defined here. The original dpadd unpacks
// both operands with __unpack_d, runs _fpadd_parts and packs with __pack_d; dpcmp unpacks both
// and returns the result of __fpcmp_parts_d; dptofp unpacks one operand and builds the float
// with __make_fp; dpmul unpacks both, handles the exceptional classes, forms the 128-bit
// product of the two fractions, normalises it and packs.
// The same four original functions already have independently authored bodies in the
// game-player-softdouble-build and game-player-float-tags-build sources, under different
// names: norrath_player_double_add, norrath_player_double_multiply, norrath_factory_double_compare
// and norrath_player_double_to_float. This unit does not duplicate those bodies. Each function
// here passes its arguments through unchanged to the matching one and returns its result
// unchanged, so that the consumers' imports resolve without a symbol alias.
// Unknown: whether the original programs ever pass operands in other registers; the four
// observed signatures take and return full 64-bit values (float for dptofp).
extern "C" {
unsigned long long norrath_player_double_add(unsigned long long left, unsigned long long right);
unsigned long long norrath_player_double_multiply(unsigned long long left, unsigned long long right);
int norrath_factory_double_compare(unsigned long long left, unsigned long long right);
float norrath_player_double_to_float(unsigned long long value);

unsigned long long norrath_softdouble_add_2fb5c0(unsigned long long left, unsigned long long right) {
    return norrath_player_double_add(left, right);
}
unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long left, unsigned long long right) {
    return norrath_player_double_multiply(left, right);
}
int norrath_softdouble_compare_2fba48(unsigned long long left, unsigned long long right) {
    return norrath_factory_double_compare(left, right);
}
float norrath_softdouble_to_float_2fbc20(unsigned long long value) {
    return norrath_player_double_to_float(value);
}
}
