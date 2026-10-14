// Observed: _$_10VIMatrix33 at 0x0108ae10, 44 bytes, returns nothing. It does nothing to
// the object itself; if bit 0 of the second argument is set it tail-calls
// __builtin_delete(this).
// Inferred: the destructor of a plain matrix type with no members needing teardown.
// Unknown: whether the type has a base class with its own destructor.
extern "C" {
void __builtin_delete(void* pointer);

void _$_10VIMatrix33(void* object, int flags) {
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
