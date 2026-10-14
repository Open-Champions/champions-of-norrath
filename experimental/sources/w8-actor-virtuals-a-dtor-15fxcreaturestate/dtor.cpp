// Observed _$_15FxCreatureState at 0x001e5db0, 44 bytes. It returns nothing.
// Bit 0 of the incoming flag selects __builtin_delete on the object. The
// object bytes are not touched. Other flag bits do nothing. Unknown: what
// the class holds; callers embed it at offset 408 of the Hate* props and
// pass flag 2, so no delete happens there.

extern "C" {
void __builtin_delete(void* pointer);
}

extern "C" void dtor_15FxCreatureState(void* object, int flags) asm("_$_15FxCreatureState");
extern "C" void dtor_15FxCreatureState(void* object, int flags) {
    if ((flags & 1) != 0) __builtin_delete(object);
}
