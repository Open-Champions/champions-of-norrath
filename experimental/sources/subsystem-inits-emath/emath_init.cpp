// emathInit__Fv at 0x0015a8e0, 28 bytes, read from the pinned SLUS-20565 image.
// The body saves the return address and calls initNoiseTable__Fv. Fresh
// compilation is not an original-byte match. The noise table has not been
// built on the EE.
extern "C" {
void initNoiseTable__Fv();

void emathInit__Fv() {
    initNoiseTable__Fv();
}
}
