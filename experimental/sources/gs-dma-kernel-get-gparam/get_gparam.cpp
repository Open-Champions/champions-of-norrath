/* Observed sceGsGetGParam at 0x002d1878 is twelve bytes and returns
 * 0x00444880. That address is gp.15, 16 file bytes: halfwords 1, 2, 1, 3
 * and two zero words. This source returns the gp.15 symbol. It is not an
 * original-byte match. The halfword names are offsets, not original fields.
 */
extern "C" unsigned char gp_15[16] asm("gp.15") = {
    1, 0, 2, 0, 1, 0, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

extern "C" unsigned char* sceGsGetGParam() {
    return gp_15;
}
