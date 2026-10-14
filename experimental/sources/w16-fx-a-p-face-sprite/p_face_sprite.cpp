// Observed: P_FaceSprite__FG6Point3ffiiiiP8_texturesi at 0x001815c8, 148 bytes, no calls. The arguments are a Point3 (three floats, passed as a
// pointer to the caller's copy), two floats (F1, F2 in the first two float argument registers), four ints (A, B, C, D), a texture pointer T,
// a short S and an int U. When the word nmSingle already equals 1024 it returns without writing anything. Otherwise it takes
// the index n = nmSingle, stores n + 1 in nmSingle, and fills the 32-byte record at singleBuffer + 32 * n: the three position words at +0, +4
// and +8; F2 at +12; F1 at +16; the low bytes of A, B, C, D at +20, +21, +22, +23; T at +24; S at +28 as a halfword; the low 16 bits of U at +30.
// Inferred: a fixed pool of 1024 camera-facing sprite records for the frame, drawn later from singleBuffer; A to D are four colour bytes
// and F1 and F2 sizes (the callers pass a size first and a second size or scale next), S an angle-like halfword and U a flag or mode value.
// Unknown: which of A to D is which colour channel, what U means (it is truncated to 16 bits), and whether the record is cleared at frame start.
struct Point3 { float x, y, z; };

extern "C" {
extern unsigned char singleBuffer[];
extern unsigned nmSingle;

void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const Point3* position, float first, float second, int a, int b, int c, int d, void* texture,
                                               short spin, int mode) {
    if (nmSingle == 1024) return;
    unsigned char* record = singleBuffer + nmSingle * 32;
    ++nmSingle;
    *reinterpret_cast<Point3*>(record) = *position;
    *reinterpret_cast<short*>(record + 28) = spin;
    *reinterpret_cast<float*>(record + 16) = first;
    *reinterpret_cast<float*>(record + 12) = second;
    record[20] = static_cast<unsigned char>(a);
    record[21] = static_cast<unsigned char>(b);
    record[22] = static_cast<unsigned char>(c);
    record[23] = static_cast<unsigned char>(d);
    *reinterpret_cast<unsigned*>(record + 24) = static_cast<unsigned>(reinterpret_cast<unsigned long>(texture));
    *reinterpret_cast<short*>(record + 30) = static_cast<short>(mode);
}
}
