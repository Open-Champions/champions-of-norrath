// Observed unnamed sdata C string at 0x485408. Callers pass this address as text.
// The view is the characters plus the terminating NUL. The next literal begins on the following byte.
// Blocker passes it to the second tag query.
extern "C" const char norrath_actors_a_view_485408[];
const char norrath_actors_a_view_485408[] = "radius2";
static_assert(sizeof norrath_actors_a_view_485408 == 8, "the view ends at the terminating NUL");
