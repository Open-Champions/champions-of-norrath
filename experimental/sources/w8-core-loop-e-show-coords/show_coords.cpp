// Observed showCoords__Fv at 0x0017fb90, 692 bytes.
// It returns when validLocalPlayer is negative or not below nmPlayers, when
// the player slot's word at byte 21872 (the entity pointer, 23984-byte slots
// from player at 0x00408600) is null, or when bit 2 of the word at pad+96 is
// set. Otherwise the 12 bytes at entity+16 are copied to a local position and
// the text is drawn with fontStart(smallFont, 7, 0x80404040, -1), three or
// four fontPrintCentered(320, y, text, 10000) lines, and fontEnd.
//   Line at y=10: if the word at world+112 is nonzero, the
//   megatile original position of the local position is requested
//   (getMegatileOriginalPosition, result returned through a hidden pointer,
//   argument passed as a pointer to a copy) and "x:%d y:%d -- Mega %d, %d"
//   prints the rounded x and y of the position and then of that result.
//   Otherwise "x:%d y:%d" prints the rounded x and y.
//   Line at y=26: "Build: %s,  %s\n" with two build strings from the image.
//   Line at y=40, only when the word at world+112 is nonzero:
//   "Megatile seed %08x:%d\n" with megatileSeed and megatileRot.
// Rounding is half away from zero: (int)(v + 0.5f) for v >= 0, else
// (int)(v - 0.5f). The build strings are copied from the image as observed.
// Unknown: the meaning of the word at world+112 and bit 2 of pad+96.
struct Point3 {
    float x, y, z;
};
extern "C" {
int sprintf(char* destination, const char* format, ...);
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontPrintCentered__FiiPci(int x, int y, char* text, int flags);
void fontEnd__Fv();
void getMegatileOriginalPosition__FG6Point3(Point3* result, const Point3* position);
extern int validLocalPlayer;
extern int nmPlayers;
extern int megatileSeed;
extern int megatileRot;
extern void* smallFont;
extern unsigned char* world;
extern unsigned char player[];
extern unsigned char pad[];
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
unsigned char* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
unsigned word_at(const unsigned char* p) {
    return p[0] | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int rounded(float v) {
    return static_cast<int>(0.0f <= v ? v + 0.5f : v - 0.5f);
}
}

extern "C" void showCoords__Fv() {
    char text[256];
    Point3 position;
    if (validLocalPlayer < 0) return;
    if (validLocalPlayer >= nmPlayers) return;
    unsigned char* const slot = player + validLocalPlayer * 23984;
    const unsigned entity_word = word_at(slot + 21872);
    if (entity_word == 0u) return;
    unsigned char* const entity = pointer_at(entity_word);
    position = *reinterpret_cast<Point3*>(entity + 16);
    if ((word_at(pad + 96) & 2u) != 0u) return;
    fontStart__FP4FontiUii(smallFont, 7, 0x80404040u, -1);
    const bool megatile = word_at(world + 112) != 0u;
    if (megatile) {
        Point3 original;
        getMegatileOriginalPosition__FG6Point3(&original, &position);
        sprintf(text, "x:%d y:%d -- Mega %d, %d", rounded(position.x), rounded(position.y),
                rounded(original.x), rounded(original.y));
    } else {
        sprintf(text, "x:%d y:%d", rounded(position.x), rounded(position.y));
    }
    fontPrintCentered__FiiPci(320, 10, text, 10000);
    sprintf(text, "Build: %s,  %s\n", "Dec 30 2003", "18:58:39");
    fontPrintCentered__FiiPci(320, 26, text, 10000);
    if (word_at(world + 112) != 0u) {
        sprintf(text, "Megatile seed %08x:%d\n", megatileSeed, megatileRot);
        fontPrintCentered__FiiPci(320, 40, text, 10000);
    }
    fontEnd__Fv();
}
