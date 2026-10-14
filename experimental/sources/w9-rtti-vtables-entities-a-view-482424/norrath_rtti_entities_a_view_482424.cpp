// Observed six single-precision literals in .sdata at 0x00482424, 0x00482428,
// 0x0048242c, 0x00482430, 0x00482434 and 0x00482438 (reached through gp-21964 ..
// gp-21944), as the little-endian words below: 0x3f999999 (just under 1.2),
// 0x3fb33333 (about 1.4), 0x3f999999 (again), then 0x3e4ccccc three times (just under
// 0.2). Read by initType__13FemaleDarkElfb. Each is kept as a separate view because
// the image holds them at separate addresses.
// Unknown: whether the original treats them as named variables and what they mean.
extern "C" const unsigned norrath_rtti_entities_a_view_482424 = 0x3f999999;
extern "C" const unsigned norrath_rtti_entities_a_view_482428 = 0x3fb33333;
extern "C" const unsigned norrath_rtti_entities_a_view_48242c = 0x3f999999;
extern "C" const unsigned norrath_rtti_entities_a_view_482430 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482434 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482438 = 0x3e4ccccc;
