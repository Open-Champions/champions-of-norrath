// Observed: undoBrowser copies the 64 bytes at undoQuickLoadStamp to
// quickLoadStamp, in eight-byte pieces, and returns. It reads no other
// object and calls nothing.
// Inferred: none. The names suggest it restores a copy kept elsewhere, but
// nothing here shows who saves undoQuickLoadStamp.
// Unknown: the contents of the 64 bytes and their writers.

struct QuickLoadStampBytes {
    unsigned char bytes[64];
};

extern "C" QuickLoadStampBytes undoQuickLoadStamp;
extern "C" QuickLoadStampBytes quickLoadStamp;

extern "C" void undoBrowser__Fv() {
    quickLoadStamp = undoQuickLoadStamp;
}
