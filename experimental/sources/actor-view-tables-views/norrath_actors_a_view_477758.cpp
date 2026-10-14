// Observed unnamed rodata C string at 0x477758. Callers pass this address as text.
// The view is the characters plus the terminating NUL. 6 zero bytes follow the NUL and are not part of this object.
// FireBomb uses it as the frequency tag.
extern "C" const char norrath_actors_a_view_477758[];
const char norrath_actors_a_view_477758[] = "frequency";
static_assert(sizeof norrath_actors_a_view_477758 == 10, "the view ends at the terminating NUL");
