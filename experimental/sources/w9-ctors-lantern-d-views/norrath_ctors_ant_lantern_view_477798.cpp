// Observed .rodata C string at 0x477798. Catapult passes this text as the first argument of a lump resource lookup.
// The view is the characters plus the terminating NUL. Following zero bytes are padding and not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_477798[];
const char norrath_ctors_ant_lantern_view_477798[] = "catapult.lmp";
static_assert(sizeof norrath_ctors_ant_lantern_view_477798 == 13, "the view ends at the terminating NUL");
