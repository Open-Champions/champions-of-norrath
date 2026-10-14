// Observed: FilterActor__13VIActorFilterR7VIScenei at 0x0105ec28, 16 bytes. Arguments: the filter (F), the scene (unused) and an actor index (A). It returns 1 when the 32-bit
// word at F+0 equals A and 0 otherwise.
// Inferred: the filter stores one actor index and accepts exactly that actor; VIActorFilter has the same body as its sibling filter class.
// Unknown: whether a zero or negative word in F+0 has a special meaning to callers (none here), and the filter's other fields.
extern "C" {
int FilterActor__13VIActorFilterR7VIScenei(unsigned char* self, unsigned char*, int actor) {
    return *reinterpret_cast<int*>(self) == actor;
}
}
