// scene at 0x004f4060, 56 bytes, to savedScene. BSS is zero.
// lightInit writes the direction and the later parameter bytes.
// The reviewed -G0 profile keeps this zero object in BSS.
extern "C" {
unsigned char scene[56];
}
static_assert(sizeof scene == 56u, "scene extent");
