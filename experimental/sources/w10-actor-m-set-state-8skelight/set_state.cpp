// Observed setState__8Skelighti at 0x246cf0, 544 bytes. This is the table entry the 8Skelight constructors and vtable view use. The body is authored
// in w9-rtti-vtables-entities-c as norrath_skelight_set_state (the name the Skelight constructor imports);
// this definition only gives it the symbol name the actor tables use and passes both arguments through.
// Nothing is returned.
extern "C" void norrath_skelight_set_state(void* self, int requested);
extern "C" void setState__8Skelighti(void* self, int state) {
    norrath_skelight_set_state(self, state);
}
