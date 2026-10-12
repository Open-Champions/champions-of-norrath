// Observed unnamed sdata C string at 0x485358. Callers pass this address as text.
// The view is the characters plus the terminating NUL. 3 zero bytes follow the NUL and are not part of this object.
// DoorSecret, DoorSwing, and FloorSwitch pass it to findTagString.
extern "C" const char norrath_actors_a_view_485358[];
const char norrath_actors_a_view_485358[] = "name";
static_assert(sizeof norrath_actors_a_view_485358 == 5, "the view ends at the terminating NUL");
