// Observed lightSetupAmbientOnly__FP6Packet at 0x0013de48, extent 164. Argument: a Packet whose
// first word is the output cursor. Returns nothing. Reads the four halfwords of charAmbient
// (0x00486850). With base = cursor + 4: *cursor = 0x6c03c000; base+48 = 0x14000010;
// base+52 = 0x6d058000; base+96 = 0x14000024; halfwords base+88..94 = charAmbient[0..3];
// zero words at base + 4k, base + 16 + 4k and base + 32 + 4k for k = 0..3; zero halfword quads
// at base + 56 + 8s (four per slot, slots 0..3); then the cursor becomes base + 100. The packet's
// second word is not touched. Same packet shape as the one lightSetupForCharacter builds, with
// no lights and the ambient colour alone.
// Unknown: what the tag words mean, and the unit of the halfword colour.
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};
typedef unsigned __attribute__((may_alias)) u32;
typedef short __attribute__((may_alias)) i16;
extern "C" {
extern short charAmbient[4];
void lightSetupAmbientOnly__FP6Packet(Packet* packet) {
    unsigned char* start = packet->cursor;
    *reinterpret_cast<u32*>(start) = 0x6c03c000u;
    unsigned char* base = start + 4;
    *reinterpret_cast<i16*>(base + 88) = charAmbient[0];
    packet->cursor = base + 100;
    *reinterpret_cast<i16*>(base + 90) = charAmbient[1];
    *reinterpret_cast<u32*>(base + 52) = 0x6d058000u;
    *reinterpret_cast<u32*>(base + 96) = 0x14000024u;
    *reinterpret_cast<i16*>(base + 92) = charAmbient[2];
    *reinterpret_cast<u32*>(base + 48) = 0x14000010u;
    *reinterpret_cast<i16*>(base + 94) = charAmbient[3];
    for (int k = 0; k < 4; ++k) {
        i16* quad = reinterpret_cast<i16*>(base + 56 + 8 * k);
        quad[0] = 0;
        quad[1] = 0;
        quad[2] = 0;
        quad[3] = 0;
        *reinterpret_cast<u32*>(base + 4 * k) = 0;
        *reinterpret_cast<u32*>(base + 16 + 4 * k) = 0;
        *reinterpret_cast<u32*>(base + 32 + 4 * k) = 0;
    }
}
}
