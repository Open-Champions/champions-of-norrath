// Observed __7WebTrapG6Point3T1 at 0x0020e758, 396 bytes (two positions: the object's position and an offset).
// This is a different overload from the WebTrap constructor that takes a short and an argument list.
// Arguments: the object, a position P and a position Q (both by value). Returns nothing. Steps, in order:
//  1. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument.
//  2. The word at +76 is cleared, _vt$7WebTrap goes to the word at +72, and Q is stored at +84 (three floats).
//  3. Bytes +40 and +41 become 0xff and the word at +156 is cleared; byte +132 is cleared and then set to 50;
//     the word at +152 becomes 1; the word at +80 becomes frameNm + 600.
//  4. With O the position the GameObject constructor left at +16 and Q read back from +84: S = O + Q per
//     component, and S * 0.5 per component (H) is stored at +108 and then copied to +120. The words at +136 and
//     +148 are cleared and bit 0 is set in the word at +12.
//  5. objectAddToRunList runs on the object, then lumpFindResource__FPcT0 is called with the two texts
//     "fx.lmp" and "spiderweb.tex" (resource names); its result goes to the word at +140 and the word at +144 is cleared.
// Inferred: H is the midpoint between O and O + Q. Unknown: what the fields at +40, +80, +132 and +152 mean.
// Nothing here ran on the EE.
#include "../w11-ctor-a-common/ctor.hpp"

extern "C" {
extern unsigned char vt_WebTrap[] __asm__("_vt$7WebTrap");
void __7WebTrapG6Point3T1(void* self, Point3 position, Point3 offset);
}

extern "C" void __7WebTrapG6Point3T1(void* self, Point3 position, Point3 offset) {
    using namespace w11;
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 76) = 0;
    u32(self, 72) = word(vt_WebTrap);
    *reinterpret_cast<Point3*>(&u32(self, 84)) = offset;
    u8(self, 132) = 0;
    u8(self, 41) = 0xff;
    u8(self, 40) = 0xff;
    u32(self, 156) = 0;
    u8(self, 132) = 50;
    u32(self, 152) = 1;
    u32(self, 80) = static_cast<unsigned>(frameNm + 600);
    const Point3 origin = *reinterpret_cast<Point3*>(&u32(self, 16));
    const Point3 extra = *reinterpret_cast<Point3*>(&u32(self, 84));
    Point3 sum;
    sum.x = origin.x + extra.x;
    sum.y = origin.y + extra.y;
    sum.z = origin.z + extra.z;
    u32(self, 12) |= 1u;
    Point3 half;
    half.x = sum.x * 0.5f;
    half.y = sum.y * 0.5f;
    half.z = sum.z * 0.5f;
    u32(self, 136) = 0;
    *reinterpret_cast<Point3*>(&u32(self, 108)) = half;
    u32(self, 148) = 0;
    *reinterpret_cast<Point3*>(&u32(self, 120)) = *reinterpret_cast<Point3*>(&u32(self, 108));
    objectAddToRunList__FP10GameObject(self);
    void* const resource = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spiderweb.tex"));
    u32(self, 144) = 0;
    u32(self, 140) = word(resource);
}
