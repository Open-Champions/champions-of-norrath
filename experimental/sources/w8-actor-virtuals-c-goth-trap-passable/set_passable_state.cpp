// Observed setPassableState__8GothTrapb at 0x225cc0, 300 bytes. The body is
// authored in the vehicle-actors package as norrath_goth_trap_set_passable_state
// (door collision, door move and door height from the word at object+376); this
// definition gives it the symbol name the actor tables use and passes both
// arguments through unchanged. No value is returned: the image does not leave
// that body's result in V0 on exit (the exit V0 is a leftover compare constant).
extern "C" unsigned norrath_goth_trap_set_passable_state(void* object, int enabled);
extern "C" void setPassableState__8GothTrapb(void* self, int passable) {
    (void)norrath_goth_trap_set_passable_state(self, passable);
}
