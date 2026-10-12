/* Observed sceDmaSend at 0x002d2fd8, 104 bytes. The packet goes through
 * CheckAddress, then WaitDma. Channel offset 48 receives that address unless
 * the word already there is 0xffffffff. Offset 32 is cleared. The first word
 * clears bit 3 and sets bits 0, 2, and 8. Fresh compilation is not an
 * original-byte match.
 */
extern "C" unsigned CheckAddress(unsigned address);
extern "C" void WaitDma(unsigned* channel);

extern "C" void sceDmaSend(unsigned* channel, void* packet) {
    const unsigned address = CheckAddress(static_cast<unsigned>(
        reinterpret_cast<unsigned long>(packet)));
    WaitDma(channel);
    if (channel[12] != 0xffffffffu)
        channel[12] = address;
    channel[8] = 0u;
    channel[0] = (channel[0] & 0xfffffff3u) | 0x105u;
}
