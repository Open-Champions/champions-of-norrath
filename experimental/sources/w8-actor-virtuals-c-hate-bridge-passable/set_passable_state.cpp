// Observed setPassableState__10HateBridgeb at 0x225df0, 252 bytes. The body is
// authored in the vehicle-actors package as norrath_hate_bridge_set_passable_state;
// this definition gives it the symbol name the actor tables use and passes both
// arguments through unchanged. No value is returned: the image does not leave
// that body's result in V0 on exit (the exit V0 is a leftover compare constant).
extern "C" unsigned norrath_hate_bridge_set_passable_state(void* object, int enabled);
extern "C" void setPassableState__10HateBridgeb(void* self, int passable) {
    (void)norrath_hate_bridge_set_passable_state(self, passable);
}
