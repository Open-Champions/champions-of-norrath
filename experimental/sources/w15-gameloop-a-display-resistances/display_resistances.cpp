#include "../w9-animation-object-c-common/mem.hpp"
// Observed displayResistances__Fi at 0x001bef50, 600 bytes, 240-byte frame. `slot` selects one of the two 116-byte
// records of statsScreen (0x004281a8, 232 bytes): rec = statsScreen + 116 * slot. Its word at +52 indexes the
// 23984-byte player records of `player`, and its words at +0 and +4 are a screen offset (ox, oy).
//  1. Two tables are copied to the stack: six text keys ("Resistances", "FireResistance", "ColdResistance",
//     "PoisonResistance", "DiseaseResistance", "MagicResistance") and six (x, y) pairs (273,156) (39,7) (39,56)
//     (39,107) (39,158) (39,204).
//  2. fontStart(smallFont, 6, 0x80323c3c, -1) and fontColor(0x80323c3c, -1, 0).
//  3. For i = 0 to 5 with the pair (x, y): i = 0 draws the heading, fontPrintCenteredu(x + 320 * slot, y,
//     textGetString(key 0), 1000000). For i = 1 to 5 the float at (pointer in player + 21872) + 84 + 4 * i is multiplied
//     by 100, 0.5 is added and the result truncated to an int; it is printed with sprintf("%d%%") into a local text and
//     drawn with fontPrintCentered(ox + x + 237, oy + y + 188, text, 10000).
//  4. fontEnd().
// Nothing is returned. The keys for i = 1 to 5 are copied but never used by this function.
// Inferred: the five floats are fractions of resistance (printed as percentages) for fire, cold, poison, disease and
// magic, in the order of the key table; the player word at +21872 points at the record holding them.
// Unknown: the layout of the pointed-to record before +84, the meaning of the 6, -1 and 0x80323c3c font arguments, and
// of statsScreen beyond +0, +4 and +52.
typedef unsigned short UString;

#if defined(__mips__)
#define LIBRARY_SYMBOL(name) __asm__(#name)
#else
#define LIBRARY_SYMBOL(name)
#endif

extern "C" {
extern unsigned char statsScreen[];
extern unsigned char player[];
extern unsigned smallFont;

void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b);
void fontColor__FUiii(unsigned color, int a, int b);
void fontEnd__Fv();
UString* textGetString__FPc(const char* key);
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width);
void fontPrintCentered__FiiPci(int x, int y, char* text, int width);
int formatText(char* buffer, const char* format, ...) LIBRARY_SYMBOL(sprintf);

void displayResistances__Fi(int slot) {
    using namespace norrath_aoc;
    const unsigned char* const record = statsScreen + 116 * slot;
    const unsigned char* const entry = player + 23984 * static_cast<int>(load_word(record + 52));
    const char* keys[6] = {"Resistances", "FireResistance", "ColdResistance", "PoisonResistance", "DiseaseResistance",
                           "MagicResistance"};
    int positions[12] = {273, 156, 39, 7, 39, 56, 39, 107, 39, 158, 39, 204};
    char text[40];
    fontStart__FP4FontiUii(reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(smallFont)), 6, 0x80323c3cu, -1);
    fontColor__FUiii(0x80323c3cu, -1, 0);
    for (int i = 0; i < 6; ++i) {
        const int x = static_cast<int>(load_word(record)) + positions[2 * i];
        const int y = static_cast<int>(load_word(record + 4)) + positions[2 * i + 1];
        if (i == 0) {
            fontPrintCenteredu__FiiPUsi(positions[0] + 320 * slot, positions[1], textGetString__FPc(keys[0]), 1000000);
        } else {
            const unsigned char* resistances = load_pointer(entry + 21872);
            formatText(text, "%d%%", static_cast<int>(load_float(resistances + 76 + 4 * i + 8) * 100.0f + 0.5f));
            fontPrintCentered__FiiPci(x + 237, y + 188, text, 10000);
        }
    }
    fontEnd__Fv();
}
}
