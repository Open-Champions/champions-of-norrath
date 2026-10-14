// Numeric lighting flag comparison adapter with static storage duration.
// The original owner is local to its translation unit; this named external
// definition supports the isolated source dependency comparison.
typedef char unsigned_width[sizeof(unsigned) == 4 ? 1 : -1];
extern "C" {
unsigned norrath_disable_volumetric_lighting = 0;
}
