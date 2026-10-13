// Observed _$_5Light at 0x0013eac8, 92 bytes: the Light destructor, taking the object and a flag word.
// The object begins with two link words, called here first and second because their roles are inferred
// only: the object is removed from a doubly linked chain. If the second link is nonzero, the word at
// offset 0 of the object it points to is set to this object's first link. If the first link is nonzero,
// the word at offset 4 of the object it points to is set to this object's second link. Both of this
// object's links are then cleared. When bit 0 of the flag word is set the object is released with
// __builtin_delete (0x002fedf0, a tail call). Nothing is returned. Unknown: which direction is forward.
struct LightLinks {
    LightLinks* first;
    LightLinks* second;
};

extern "C" void builtin_delete(void* object) __asm__("__builtin_delete");

extern "C" void light_destructor(LightLinks* self, int flags) __asm__("_$_5Light");
extern "C" void light_destructor(LightLinks* self, int flags) {
    if (self->second != 0) self->second->first = self->first;
    if (self->first != 0) self->first->second = self->second;
    self->first = 0;
    self->second = 0;
    if (flags & 1) builtin_delete(self);
}
