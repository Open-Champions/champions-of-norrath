// Observed: GetWorld__C9VICollide at 0x010970d8, 8 bytes, in the pinned SLUS-20565 image. It returns the collide-record address plus 16 and touches no memory.
// Callers pass that address on as the world matrix (SetWorld__9VICollideRC10VIMatrix44 and the sprite collide routines), so
// the record carries a matrix starting at byte offset 16.
// Inferred: the 4x4 world matrix sits at offset 16 of the collide record. Unknown: the record's other fields and size.
extern "C" {
unsigned char* GetWorld__C9VICollide(unsigned char* self) {
    return self + 16;
}
}
