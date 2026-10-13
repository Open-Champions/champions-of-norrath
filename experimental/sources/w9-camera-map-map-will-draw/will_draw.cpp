// Observed willDraw__3mapP12_worldHeaderf at 0x0015e140, 84 bytes.
// With the header words at +108 and +112: if the word at +108 is zero and the
// word at +112 is zero, returns 0. Otherwise let used = (glistp - gstart) >> 3
// (arithmetic shift of the byte difference of the two words glistp 0x004844c8
// and gstart 0x004844c4) and free = GLISTSIZE (0x00484468) - used. If free is
// below 12000 returns 0. Otherwise returns 1 when the float argument truncated
// to an int is at least 2, else 0. The word at +112 is read only when the word
// at +108 is zero. Nothing is stored. What the quantities mean is inferred only
// from their names. Nothing here was executed on the EE.
extern "C" {
extern int glistp;
extern int gstart;
extern int GLISTSIZE;
}

extern "C" int willDraw__3mapP12_worldHeaderf(const unsigned char* header, float value) {
    if (*reinterpret_cast<const int*>(header + 108) == 0) {
        if (*reinterpret_cast<const int*>(header + 112) == 0) return 0;
    }
    if (GLISTSIZE - ((glistp - gstart) >> 3) < 12000) return 0;
    return static_cast<int>(value) >= 2;
}
