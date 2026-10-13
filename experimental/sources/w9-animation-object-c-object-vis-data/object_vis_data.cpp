// Storage for the three words gameSetObjectVis__Fv writes (see w8-core-loop-d-set-object-vis).
//   objectVisPoints        0x00333d28, 48 bytes (distance to the next symbol): four 12-byte
//                          points as three floats each; initial file bytes are zero.
//   objectVisCount         0x00483afe, one signed byte; initial value 0.
//   objectVisUseCamCenter  0x00483ac9, one byte; initial value 1 in the file image.
// All three are in the initialised data section of the original (symbol class D). The reviewed
// profile places zero-valued objects in BSS, so points and count are zero BSS objects here and
// only the flag byte (value 1) is in .data; the values are the same, the section is not.
// Unknown: the declared type of the points and who else reads them.
struct ObjectVisPoint { float x, y, z; };

extern "C" {
ObjectVisPoint objectVisPoints[4] = {};
signed char objectVisCount = 0;
unsigned char objectVisUseCamCenter = 1;
}

#if defined(__mips__)
static_assert(sizeof(objectVisPoints) == 48, "extent of objectVisPoints");
#endif
