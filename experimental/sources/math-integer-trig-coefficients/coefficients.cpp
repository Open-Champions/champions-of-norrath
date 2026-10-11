// Observed: icos__Fi at 0x15a070 and isin__Fi at 0x15a100 each load four
// consecutive .lit4 floats through gp 0x4879f0. The eight words begin at
// 0x47fefc. The two polynomials use the same four numeric values and remain
// separate objects. No ELF symbol names this pool.
// Inferred: the loads are norrath_icos_scale/cubic/quintic/septic and the
// matching norrath_isin_* imports, in that address order.
// Unknown: why the pool stores two identical copies.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(float) == 4, "float word");

extern "C" {
float norrath_icos_scale = __builtin_bit_cast(float, 0x38490fdau);
float norrath_icos_cubic = __builtin_bit_cast(float, 0x3e2aaa57u);
float norrath_icos_quintic = __builtin_bit_cast(float, 0x3c086b24u);
float norrath_icos_septic = __builtin_bit_cast(float, 0x39458017u);
float norrath_isin_scale = __builtin_bit_cast(float, 0x38490fdau);
float norrath_isin_cubic = __builtin_bit_cast(float, 0x3e2aaa57u);
float norrath_isin_quintic = __builtin_bit_cast(float, 0x3c086b24u);
float norrath_isin_septic = __builtin_bit_cast(float, 0x39458017u);
}
