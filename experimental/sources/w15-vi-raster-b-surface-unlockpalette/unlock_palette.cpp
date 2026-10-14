// Observed: UnlockPalette__9VISurface at 0x0112ca80 is 68 bytes in the pinned SLUS-20565 image. It compares the word at offset 28
// (0x1c) of the surface with the word at offset 64 (0x40). When they differ it returns -1 and changes nothing. When they are equal it
// calls ReorderPalette__9VISurface(surface), stores -1 in the word at +28, and returns 0.
// Inferred: +28 holds the lock token of the palette (the value that LockPalette stored, -1 meaning unlocked) and +64 holds the token
// that must be presented to release it; the int result is a status (0 released, -1 refused).
// Unknown: what the two words really are, and ReorderPalette's behaviour (an unresolved import here). Nothing here ran on the EE.
extern "C" {
void ReorderPalette__9VISurface(unsigned char* surface);
int UnlockPalette__9VISurface(unsigned char* surface) {
    if (*reinterpret_cast<int*>(surface + 0x1c) != *reinterpret_cast<int*>(surface + 0x40)) {
        return -1;
    }
    ReorderPalette__9VISurface(surface);
    *reinterpret_cast<int*>(surface + 0x1c) = -1;
    return 0;
}
}
