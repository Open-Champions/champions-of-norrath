/* Observed texturePackDown__Fv at 0x00172690, 56 bytes. GP is 0x004879f0.
 * The byte at gp-14670 is textureUncompBufferLocked (0x004840a2). A signed
 * value of 0, or a signed value of 10 or more, is left unchanged. Every
 * other signed value increments the unsigned byte by one, wrapping through
 * 255. cacheClear__Fv then runs. Fresh compilation is not an original-byte
 * match.
 */
extern "C" unsigned char textureUncompBufferLocked;
extern "C" void cacheClear__Fv();

extern "C" void texturePackDown__Fv() {
    const signed char signed_byte = static_cast<signed char>(textureUncompBufferLocked);
    const unsigned char unsigned_byte = textureUncompBufferLocked;
    if (signed_byte != 0 && signed_byte < 10)
        textureUncompBufferLocked = static_cast<unsigned char>(unsigned_byte + 1u);
    cacheClear__Fv();
}
