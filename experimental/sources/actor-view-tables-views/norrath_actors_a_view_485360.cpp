// Observed unnamed sdata C string at 0x485360. Callers pass this address as text.
// The view is the characters plus the terminating NUL. The next literal begins on the following byte.
// DoorSwing passes it to findTagInt.
extern "C" const char norrath_actors_a_view_485360[];
const char norrath_actors_a_view_485360[] = "disable";
static_assert(sizeof norrath_actors_a_view_485360 == 8, "the view ends at the terminating NUL");
