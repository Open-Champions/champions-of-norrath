// Observed originalPos at 0x0042e2d0 (48 bytes, file-backed, all zero in the image) and
// originalYaw at 0x00485bf0 (8 bytes, file-backed in the small-data area, all zero). The
// authored un-modify/modify RAM-slot code reads them per player as 12 bytes of position
// at 12 * n and two bytes of yaw at 2 * n for n in 0..3; those uses are the only layout
// evidence. Their element types are not established, so they stay byte arrays.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char originalPos[48] NORRATH_LOADED(".data") = {};
unsigned char originalYaw[8] NORRATH_LOADED(".sdata") = {};
}
