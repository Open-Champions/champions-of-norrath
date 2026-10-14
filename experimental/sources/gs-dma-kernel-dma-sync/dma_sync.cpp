/* Observed sceDmaSync at 0x002d3278, 56 bytes. Mode 1 returns bit 8 of the
 * first channel word and does not call WaitDma. Every other mode calls
 * WaitDma and returns 0. The timeout argument is not read.
 */
extern "C" void WaitDma(unsigned* channel);

extern "C" int sceDmaSync(unsigned* channel, int mode, int timeout) {
    (void)timeout;
    if (mode == 1)
        return static_cast<int>((channel[0] >> 8) & 1u);
    WaitDma(channel);
    return 0;
}
