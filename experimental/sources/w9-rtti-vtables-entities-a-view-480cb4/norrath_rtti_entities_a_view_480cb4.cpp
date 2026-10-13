// Observed four single-precision literals in .sdata at 0x00480cb4, 0x00480cb8,
// 0x00480cbc and 0x00480cc0 (reached through gp-27964 .. gp-27952), as the
// little-endian words below: 0x3db851eb (0.09), 0x3f199999 (just under 0.6),
// 0x3f68ba2f (about 0.909091, near 10/11) and 0x45081000 (2177.0). Read by
// addTurnAnimation__3Cat. Each is kept as a separate view because the image holds
// them at separate addresses.
// Unknown: whether the original treats them as named variables and what they mean.
extern "C" const unsigned norrath_rtti_entities_a_view_480cb4 = 0x3db851eb;
extern "C" const unsigned norrath_rtti_entities_a_view_480cb8 = 0x3f199999;
extern "C" const unsigned norrath_rtti_entities_a_view_480cbc = 0x3f68ba2f;
extern "C" const unsigned norrath_rtti_entities_a_view_480cc0 = 0x45081000;
