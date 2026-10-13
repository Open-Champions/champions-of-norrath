// Observed: __19VIRadialFloraSystem at 0x010d5b40 is 92 bytes. It takes the object only and
// returns nothing. It stores zero in the words at offsets 0, 4, 8, 16, 20, 24 and 28 and -1 in
// the word at 12, and zeroes the nine words from offset 32 through 64 (one word each).
// Inferred: the nine words at 32 are a table of nine pointers (Render__19VIRadialFloraSystemR8VICamera
// walks nine words starting at offset 32 and skips null ones); offset 0 holds the scene stored by
// Init__19VIRadialFloraSystemR7VIScene.
// Unknown: the purpose of the words at 12 to 28.
extern "C" {
struct VIRadialFloraSystem;

void __19VIRadialFloraSystem(VIRadialFloraSystem* self) {
    int* words = reinterpret_cast<int*>(self);
    words[5] = 0;
    words[4] = 0;
    words[7] = 0;
    words[6] = 0;
    words[0] = 0;
    words[3] = -1;
    words[1] = 0;
    words[2] = 0;
    for (int i = 16; i >= 8; --i) words[i] = 0;
}
}
