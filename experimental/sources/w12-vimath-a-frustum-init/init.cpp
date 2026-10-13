// Observed: Init__9VIFrustum at 0x010174b0 is 8 bytes. It does not read or write
// through its argument (a0) and returns 0.
// Inferred: a placeholder initializer for the frustum member at camera offset 304;
// the frustum planes are filled later by CalcFrustum. Unknown: whether the original
// source had a body that the compiler removed.
extern "C" {
int Init__9VIFrustum(void* frustum) {
    (void)frustum;
    return 0;
}
}
