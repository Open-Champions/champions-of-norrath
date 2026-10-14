// Observed: netLocalPad at 0x00332fd0 is 352 bytes, 352 bytes of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: the pad-processing code strides it by 88 bytes per local pad (four records).
// Unknown: the layout inside a record.

extern "C" {
unsigned char netLocalPad[352];
}
