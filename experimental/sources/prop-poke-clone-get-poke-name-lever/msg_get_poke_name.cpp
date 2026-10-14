#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_getPokeName__5LeverPUsi at 0x215cd8, 332 bytes.
// The integer selects a 23984-byte player record; the entity word at byte
// 21872 of it is read without a null test, and its point at +16 (three
// singles) is taken. icos and isin of (halfword at object+56 minus 16384)
// give the pair (c, s), with a third component of 0.0. The vector from this
// object's point at +16 to the entity's point is dotted with (c, s, 0.0) in
// the order x*c, then + y*s, then + z*0.0. The sum is scaled by the signed
// byte at object+80 converted to single. A result greater than zero selects
// the text key "PullLever", anything else (including NaN) "PushLever". The
// text goes through textGetString and ustrcpy into the buffer and the
// function returns 1. Unknown: what the byte at +80 is, and what a negative
// scale means for which action the lever offers.
extern "C" unsigned char player[];
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
extern "C" int msg_getPokeName__5LeverPUsi(void* self, unsigned short* buffer, int index) {
    using namespace vslots_b;
    const unsigned slot = static_cast<unsigned>(index) * 23984u + 21872u;
    const Address entity = address(pointer(read_word(address(player) + slot)));
    const float px = read_single(entity + 16u);
    const float py = read_single(entity + 20u);
    const float pz = read_single(entity + 24u);
    const int angle = read_i16(address(self) + 56u) - 16384;
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    const float zero = 0.0f;
    const float dx = px - read_single(address(self) + 16u);
    const float dy = py - read_single(address(self) + 20u);
    const float dz = pz - read_single(address(self) + 24u);
    const float scale = static_cast<float>(static_cast<signed char>(read_u8(address(self) + 80u)));
    const float along = (dx * c + dy * s + dz * zero) * scale;
    static char pull[] = {'P', 'u', 'l', 'l', 'L', 'e', 'v', 'e', 'r', '\0'};
    static char push[] = {'P', 'u', 's', 'h', 'L', 'e', 'v', 'e', 'r', '\0'};
    ustrcpy__FPUsT0(buffer, textGetString__FPc(0.0f < along ? pull : push));
    return 1;
}
