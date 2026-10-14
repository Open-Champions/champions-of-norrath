// Observed single-precision literals in .sdata at 0x00482430, 0x00482434 and 0x00482438, each the
// little-endian word 0x3e4ccccc (just under 0.2). initType__13FemaleDarkElfb reads all three through gp.
// They are kept as three views because the image holds them at three addresses. The adjacent
// literals at 0x00482424..0x0048242c belong to the unselected w9 unit
// w9-rtti-vtables-entities-a-view-482424, which also defines these three; select only one of
// the two. Unknown: what the values mean.
extern "C" const unsigned norrath_rtti_entities_a_view_482430 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482434 = 0x3e4ccccc;
extern "C" const unsigned norrath_rtti_entities_a_view_482438 = 0x3e4ccccc;
