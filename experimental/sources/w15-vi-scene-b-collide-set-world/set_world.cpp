// Observed: SetWorld__9VICollideRC10VIMatrix44 at 0x01097090, 68 bytes, in the pinned SLUS-20565 image. Arguments: the collide record (C) and a 4x4 matrix (M).
// It calls sceVu0CopyMatrix(C+16, M), then InvertAffine__10VIMatrix44RC10VIMatrix44(C+80, C+16), and returns 0.
// Inferred: the record holds the world matrix at +16 (this is what GetWorld__C9VICollide returns) and the inverse of that matrix at +80, recomputed on every call.
// Unknown: the library copy routine's exact argument roles beyond (destination, source) as the call site shows them, and what the inverse is used for.
extern "C" {
void sceVu0CopyMatrix(unsigned char* destination, const unsigned char* source);
void InvertAffine__10VIMatrix44RC10VIMatrix44(unsigned char* self, const unsigned char* source);

int SetWorld__9VICollideRC10VIMatrix44(unsigned char* self, const unsigned char* matrix) {
    unsigned char* world = self + 16;
    sceVu0CopyMatrix(world, matrix);
    InvertAffine__10VIMatrix44RC10VIMatrix44(self + 80, world);
    return 0;
}
}
