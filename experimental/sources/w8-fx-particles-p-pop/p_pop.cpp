// P_Pop__Fv at 0x00180580, 8 bytes, read from the pinned SLUS-20565 image.
// Observed: the body is a bare return. It reads no argument, writes nothing
// and calls nothing. Fresh compilation is not an original-byte match.
extern "C" void P_Pop__Fv() {}
