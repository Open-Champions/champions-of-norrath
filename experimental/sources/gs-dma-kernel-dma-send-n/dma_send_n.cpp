/* Observed sceDmaSendN at 0x002d3040, 116 bytes. It passes the packet through
 * CheckAddress, waits for the channel, and stores that address at channel
 * offset 16 unless the word already there is 0xffffffff. Offset 32 receives
 * the quadword count. The first word keeps every bit except bits 2 and 3,
 * then bits 0 and 8 are set.
 */
extern "C" unsigned CheckAddress(unsigned address);
extern "C" void WaitDma(unsigned* channel);

extern "C" void sceDmaSendN(unsigned* channel, void* packet, int qwc) {
    const unsigned address = CheckAddress(static_cast<unsigned>(
        reinterpret_cast<unsigned long>(packet)));
    WaitDma(channel);
    if (channel[4] != 0xffffffffu)
        channel[4] = address;
    channel[8] = static_cast<unsigned>(qwc);
    channel[0] = (channel[0] & 0xfffffff3u) | 0x101u;
}
