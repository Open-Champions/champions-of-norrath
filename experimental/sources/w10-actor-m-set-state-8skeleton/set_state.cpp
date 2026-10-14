// Observed setState__8Skeletoni at 0x2453f0, 1352 bytes. This is the table entry the 8Skeleton constructors and vtable view use. The body is authored
// in w9-rtti-vtables-entities-c as norrath_skeleton_set_state (the name the Skeleton constructor imports);
// this definition only gives it the symbol name the actor tables use and passes both arguments through.
// Nothing is returned.
extern "C" void norrath_skeleton_set_state(void* self, int requested);
extern "C" void setState__8Skeletoni(void* self, int state) {
    norrath_skeleton_set_state(self, state);
}
