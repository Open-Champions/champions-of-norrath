// Observed: the local symbol padFrame.756 is the 4-byte word at 0x00484064,
// zero in the image. padProcess increments it on every call and acts when the
// previous value is a multiple of 180. The spelled name carries a compiler
// numeric suffix; this unit defines exactly that spelling through an asm label.
// Inferred: it counts pad frames.
// Unknown: nothing beyond the extent.
extern "C" unsigned pad_frame asm("padFrame.756") = 0;
