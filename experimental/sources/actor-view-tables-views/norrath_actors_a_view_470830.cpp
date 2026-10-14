// Observed unnamed rodata C string at 0x470830. Callers pass this address as text.
// The view is the characters plus the terminating NUL. 6 zero bytes follow the NUL and are not part of this object.
// Blocker passes it to the third integer-tag query.
extern "C" const char norrath_actors_a_view_470830[];
const char norrath_actors_a_view_470830[] = "standable";
static_assert(sizeof norrath_actors_a_view_470830 == 10, "the view ends at the terminating NUL");
