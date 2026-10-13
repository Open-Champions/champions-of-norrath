// Observed .rodata C string at 0x4777d8. Catapult passes this text as the second argument of a lump resource lookup.
// The view is the characters plus the terminating NUL. Following zero bytes are padding and not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_4777d8[];
const char norrath_ctors_ant_lantern_view_4777d8[] = "catapult.anm";
static_assert(sizeof norrath_ctors_ant_lantern_view_4777d8 == 13, "the view ends at the terminating NUL");
