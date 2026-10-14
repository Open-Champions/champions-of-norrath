// Observed unnamed sdata C string at 0x485400. Callers pass this address as text.
// The view is the characters plus the terminating NUL. One zero byte follows the NUL and is not part of this object.
// Blocker passes it to the first tag query.
extern "C" const char norrath_actors_a_view_485400[];
const char norrath_actors_a_view_485400[] = "radius";
static_assert(sizeof norrath_actors_a_view_485400 == 7, "the view ends at the terminating NUL");
