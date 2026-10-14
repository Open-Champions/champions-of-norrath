// Observed start__3WebG6Point3T1i at 0x0020faf0, 148 bytes. Arguments: the Web object W, two positions P and Q
// (both by value, passed by hidden pointer) and an int T. Nothing is called. Stores, from the image:
//  - P (three floats) to W+16 and Q to W+32;
//  - the word at +8 becomes 40 and the word at +0 becomes 1;
//  - the word at +44 becomes the word at +48 plus T (32-bit integer addition);
//  - the words at +56 and +12 are cleared.
// Inferred: nothing beyond the layout above. Unknown: what any of these words mean, including whether
// the two positions are end points and whether +48 / +44 are times.
// Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" void start__3WebG6Point3T1i(void* self, Point3 from, Point3 to, int ticks) {
    unsigned char* const w = static_cast<unsigned char*>(self);
    *reinterpret_cast<Point3*>(w + 16) = from;
    *reinterpret_cast<Point3*>(w + 32) = to;
    *reinterpret_cast<int*>(w + 8) = 40;
    const int base = *reinterpret_cast<int*>(w + 48);
    *reinterpret_cast<int*>(w + 0) = 1;
    *reinterpret_cast<int*>(w + 56) = 0;
    *reinterpret_cast<int*>(w + 44) = base + ticks;
    *reinterpret_cast<int*>(w + 12) = 0;
}
