// Observed _GLOBAL_$I$modelClippingOn at 0x0016ce68, 32 bytes. It calls the
// translation unit's file-local static initialization routine at 0x0016c548 with
// the arguments (1, 0xffff). That routine is 8 bytes: it returns at once and does
// nothing for any arguments, so this unit constructs nothing.
// Inferred: the 0xffff argument is the initialization priority and 1 selects
// construction rather than destruction (the destruction wrapper of a different
// unit passes 0); the translation unit has no object that needs a constructor.
// This function is listed in __CTOR_LIST__ (link-image-bounds-ctors).
// Unknown: which globals of the original unit the name refers to besides
// modelClippingOn.
#ifdef __clang__
#define NORRATH_W8_NOIPA
#else
#define NORRATH_W8_NOIPA __attribute__((noipa))
#endif
static void NORRATH_W8_NOIPA static_initialization(int constructing, int priority) {
    (void)constructing;
    (void)priority;
    __asm__ volatile("");
}

extern "C" void _GLOBAL_$I$modelClippingOn() {
    static_initialization(1, 0xffff);
}
