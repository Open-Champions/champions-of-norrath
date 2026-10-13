// Observed modelSetAttachmentFlip__Fv at 0x0016cc58, extent 16. Takes nothing and returns
// nothing. It sets bit 0x80 of the word modelOption (0x0048408c) and leaves the other bits.
// Unknown: what the bit means to the attachment code that reads it, and who clears it.
extern "C" {
extern unsigned modelOption;
void modelSetAttachmentFlip__Fv() {
    modelOption |= 0x80;
}
}
