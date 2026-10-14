// Observed engineSetDisplayDisable__Fi at 0x001911f8, 8 bytes.
// The body is a return with the argument stored to engineDisplayDisable
// in the delay slot. simpleDrawLoop calls it with 2 before the frame loop
// and with 1 after a nonzero callback. Fresh compilation is not an
// original-byte match. No EE path has run.
extern "C" {
extern int engineDisplayDisable;

void engineSetDisplayDisable__Fi(int value) {
    engineDisplayDisable = value;
}
}
