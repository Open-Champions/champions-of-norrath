// Observed lightSetupNonDirectionalDirectional__FP6Packet at 0x0013def0, extent 164.
// Argument: a Packet whose first word is the output cursor. Returns nothing. Reads the four halfwords of charDirect
// (0x00486858). With base = cursor + 4: *cursor = 0x6c03c000; base+48 = 0x14000010;
// base+52 = 0x6d058000; base+96 = 0x14000024; halfwords base+88..94 = charDirect[0..3];
// zero words at base + 4k, base + 16 + 4k and base + 32 + 4k for k = 0..3; zero halfword quads
// at base + 56 + 8s (four per slot, slots 0..3); then the cursor becomes base + 100. The packet's
// second word is not touched. The body is identical to lightSetupAmbientOnly except that the colour
// comes from charDirect, so the packet again carries no lights and a single colour.
// Unknown: what the tag words mean, the unit of the halfword colour, and why the name says
// non-directional while the colour is the one lightSetupForCharacter calls charDirect.
struct Packet {
    unsigned char* cursor;
    unsigned char* last;
};
typedef unsigned __attribute__((may_alias)) u32;
typedef short __attribute__((may_alias)) i16;
extern "C" {
extern short charDirect[4];
void lightSetupNonDirectionalDirectional__FP6Packet(Packet* packet) {
    unsigned char* start = packet->cursor;
    *reinterpret_cast<u32*>(start) = 0x6c03c000u;
    unsigned char* base = start + 4;
    *reinterpret_cast<i16*>(base + 88) = charDirect[0];
    packet->cursor = base + 100;
    *reinterpret_cast<i16*>(base + 90) = charDirect[1];
    *reinterpret_cast<u32*>(base + 52) = 0x6d058000u;
    *reinterpret_cast<u32*>(base + 96) = 0x14000024u;
    *reinterpret_cast<i16*>(base + 92) = charDirect[2];
    *reinterpret_cast<u32*>(base + 48) = 0x14000010u;
    *reinterpret_cast<i16*>(base + 94) = charDirect[3];
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
