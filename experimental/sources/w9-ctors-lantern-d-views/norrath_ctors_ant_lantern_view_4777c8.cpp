// Observed .rodata C string at 0x4777c8. Catapult passes this text as a tag name to a tag query.
// The view is the characters plus the terminating NUL. Following zero bytes are padding and not part of this object.
extern "C" const char norrath_ctors_ant_lantern_view_4777c8[];
const char norrath_ctors_ant_lantern_view_4777c8[] = "launchvel";
static_assert(sizeof norrath_ctors_ant_lantern_view_4777c8 == 10, "the view ends at the terminating NUL");
