/* Observed sceDmaGetChan at 0x002d2ca8, 40 bytes. A channel below 10 returns
 * the word at dch plus channel*4. Channel 10 and above return 0. dch at
 * 0x004448c0 is ten channel-base words. The original loads that absolute
 * address. This source loads the dch symbol. It is not an original-byte match.
 */
extern "C" unsigned dch[10] = {
    0x10008000u, 0x10009000u, 0x1000a000u, 0x1000b000u, 0x1000b400u,
    0x1000c000u, 0x1000c400u, 0x1000c800u, 0x1000d000u, 0x1000d400u
};

extern "C" unsigned* sceDmaGetChan(int channel) {
    if (static_cast<unsigned>(channel) >= 10u)
        return 0;
    return reinterpret_cast<unsigned*>(static_cast<unsigned long>(dch[channel]));
}
