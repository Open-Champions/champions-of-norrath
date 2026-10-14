// Observed: __t5VISet1ZP5VIWnd at 0x0120ba18 is 28 bytes (weak). With the set object as this it
// stores zero at +0, +4, +8 and +12 and the value -1 at +16, then returns nothing. It calls nothing.
// Inferred: this is the constructor of the VISet template instance for window pointers (the name
// says so); +8 is the pool pointer the destructor frees, and +16 is a sentinel index.
// Unknown: the role of the words at +0, +4, +12 and +16 beyond their initial values.
extern "C" void __t5VISet1ZP5VIWnd(void* self) {
    int* words = static_cast<int*>(self);
    words[0] = 0;
    words[4] = -1;
    words[1] = 0;
    words[2] = 0;
    words[3] = 0;
}
