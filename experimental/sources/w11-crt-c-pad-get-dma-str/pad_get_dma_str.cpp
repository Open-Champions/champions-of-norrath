/* Observed scePadGetDmaStr at 0x002d39b0, 96 bytes (a file-local routine of
 * the pad library). PadInfo (0x008dd8d0) holds 28 bytes per slot and 112 per
 * port; word +0 is a pointer to a 256-byte area. The routine writes that area
 * back with SyncDCache(base, base + 255) and returns one of its two 128-byte
 * halves: the second when the signed word at +88 is less than the signed word
 * at +216, otherwise the first.
 * Inferred: the two halves are alternating receive blocks and the words are
 * their sequence counters; neither is established by this routine.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];

void SyncDCache(void* begin, void* end);
#ifdef NORRATH_HOST_CONTRACT
unsigned char* norrath_host_area(unsigned word);
#endif

unsigned char* scePadGetDmaStr(int port, int slot) {
    unsigned word = *reinterpret_cast<const unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
#ifdef NORRATH_HOST_CONTRACT
    unsigned char* base = norrath_host_area(word);
#else
    unsigned char* base = reinterpret_cast<unsigned char*>(word);
#endif
    SyncDCache(base, base + 255);
    int first = *reinterpret_cast<const int*>(base + 88);
    int second = *reinterpret_cast<const int*>(base + 216);
    return base + ((first < second) << 7);
}
}
