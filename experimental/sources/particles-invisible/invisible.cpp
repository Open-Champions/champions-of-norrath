// invisible at 0x006f5a80, 80 bytes, to PS. BSS is zero.
// P_Init passes this object to allocDescriptor later.
extern "C" {
unsigned char invisible[80];
}
static_assert(sizeof invisible == 80u, "invisible extent");
