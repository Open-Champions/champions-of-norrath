// Observed the text "fx.lmp" (six characters and a terminator) at 0x00483b70. The bytes
// after the terminator belong to other data and are not part of this view. Read by the
// PhProp constructor as the first argument of a lump-resource lookup.
// Inferred: a lump file name, from its use. Unknown: whether the original treats it as one
// named variable.
extern "C" const char norrath_w9_startup_crt_b_view_483b70[7] = "fx.lmp";
