// engineSetBlur at 0x001910a0, 28 bytes, in SLUS_205.65.
// Observed: the signed difference 128 - amount is compared with the word compositeAlpha
// (0x00484428); compositeAlpha is replaced only when the difference is smaller. Nothing is
// returned. Unknown: what compositeAlpha is consumed by, and what range callers pass.
extern "C" int compositeAlpha;

extern "C" void engineSetBlur__Fi(int amount) {
    int limit = 128 - amount;
    if (limit < compositeAlpha) compositeAlpha = limit;
}
