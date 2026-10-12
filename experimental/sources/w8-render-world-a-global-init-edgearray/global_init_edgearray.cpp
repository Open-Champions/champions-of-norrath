// Observed _GLOBAL_$I$edgearray at 0x00266ec8, 32 bytes. It calls the translation
// unit's file-local static initialization routine at 0x00265da0 with the
// arguments (1, 0xffff). That routine, 32 bytes, stores zero into the word at
// zeroTile (0x008d5570) when its second argument is 0xffff and its first is 1,
// and otherwise does nothing. The routine has no other effect.
// Inferred: 1 selects construction and 0xffff is the initialization priority.
// zeroTile is a 72-byte object (to slideDustParticle at 0x008d55b8) whose
// constructor clears its first word. This function is listed in __CTOR_LIST__
// (link-image-bounds-ctors).
// Unknown: why the wrapper carries the name edgearray, an object at 0x00430440.
extern "C" {
extern int zeroTile;
}

#ifdef __clang__
#define NORRATH_W8_NOIPA
#else
#define NORRATH_W8_NOIPA __attribute__((noipa))
#endif
static void NORRATH_W8_NOIPA static_initialization(int constructing, int priority) {
    if (priority == 0xffff && constructing == 1) zeroTile = 0;
}

extern "C" void _GLOBAL_$I$edgearray() {
    static_initialization(1, 0xffff);
}
