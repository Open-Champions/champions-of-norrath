// Observed shopMouseOnHotSpot__FP12screenStruct at 0x001bc400, 132 bytes. It reads three values of the
// screen record: the integer at offset 0 (converted to float), and the floats at offsets 68 and 72.
// With x = (int)(float@68 - (float)int@0) and y = (int)float@72, and left = 91 when the word
// shopRunning is nonzero, else 16:
//   y >= 98                        -> returns -1.
//   otherwise i = (x - left) / 47 (signed division, truncating), set to 0 when negative, then
//     shopRunning nonzero: i is capped at 2;
//     shopRunning zero:    i is capped at 5.
// Returns i. Nothing is written.
// Inferred: the pointer sits on a tab bar that is 47 units per tab and starts at 91 or 16, with the
// bar's top band above 98; the result is a tab number or -1. Unknown: the screenStruct layout beyond
// these three fields, the pixel unit, and which of float@68 and float@72 is the horizontal position
// (only the one that pairs with the subtraction is guessed from the arithmetic).
extern "C" {
extern int shopRunning;

int shopMouseOnHotSpot__FP12screenStruct(void* screen) {
    const unsigned char* bytes = static_cast<const unsigned char*>(screen);
    const float base = static_cast<float>(*reinterpret_cast<const int*>(bytes));
    const float vertical = *reinterpret_cast<const float*>(bytes + 72);
    const float horizontal = *reinterpret_cast<const float*>(bytes + 68);
    const int left = shopRunning ? 91 : 16;
    if (static_cast<int>(vertical) >= 98) return -1;
    int index = (static_cast<int>(horizontal - base) - left) / 47;
    if (index < 0) index = 0;
    if (shopRunning) return index < 3 ? index : 2;
    return index < 6 ? index : 5;
}
}
