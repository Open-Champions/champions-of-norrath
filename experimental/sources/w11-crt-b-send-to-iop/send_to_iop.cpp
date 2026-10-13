// Observed _send_to_iop at 0x002d3398, 280 bytes (a local routine of the pad library). Arguments:
// port and slot. PadInfo (0x008dd8d0) holds 28 bytes per slot and 112 per port. In the slot's
// entry the word +4 is the address of the request object, the word +8 the address of two 32-byte
// halves on the receiving side, and the word +12 the id of the last queued transfer (0 for none).
//   1. A nonzero id is checked with sceSifDmaStat(id); a result that is not negative (transfer
//      still going) skips the send.
//   2. Otherwise the request object's first word is incremented, SyncDCache(object, object + 31)
//      runs, and one 32-byte transfer from the request object to half (new count & 1) of the
//      receiving buffer is queued with sceSifSetDma(packet, 1). The packet is four words: source,
//      destination, size 32, mode 0. A nonzero returned id is stored in the entry's word +12 and
//      1 is returned.
//   3. A skipped or failed send prints "libpad: sceSifSetDma faild\n" through scePrintf, only while
//      the isWarning word at 0x004448fc is nonzero, and returns 0.
// Unknown: the word at entry +0 and the role of the request object's other words. Nothing was
// executed on the EE.
extern "C" {
extern unsigned char norrath_libpad_is_warning_word[];
extern unsigned char norrath_libpad_port_info_view[];

int scePrintf(const char* format, ...);
int sceSifDmaStat(unsigned id);
unsigned sceSifSetDma(void* packet, int count);
void SyncDCache(void* begin, void* end);
#if defined(NORRATH_HOST_CONTRACT)
unsigned char* norrath_host_request_object(unsigned word);  // host check: maps the 32-bit word to a host object
unsigned norrath_host_object_word(const unsigned char* object);  // host check: maps the object back to its word
#endif

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

int _send_to_iop(int port, int slot) {
    unsigned char* entry = norrath_libpad_port_info_view + slot * 28 + port * 112;
#if defined(NORRATH_HOST_CONTRACT)
    unsigned char* request = norrath_host_request_object(load_word(entry + 4));
#else
    unsigned char* request = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(load_word(entry + 4)));
#endif
    const unsigned pending = load_word(entry + 12);
    if (pending == 0 || sceSifDmaStat(pending) < 0) {
        const unsigned count = load_word(request) + 1;
        const unsigned destination = load_word(entry + 8) + ((count & 1u) << 5);
        store_word(request, count);
        SyncDCache(request, request + 31);
        unsigned packet[4];
#if defined(NORRATH_HOST_CONTRACT)
        packet[0] = norrath_host_object_word(request);
#else
        packet[0] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(request));
#endif
        packet[1] = destination;
        packet[2] = 32;
        packet[3] = 0;
        const unsigned id = sceSifSetDma(packet, 1);
        if (id != 0) {
            store_word(entry + 12, id);
            return 1;
        }
    }
    if (load_word(norrath_libpad_is_warning_word) != 0)
        scePrintf("libpad: sceSifSetDma faild\n");
    return 0;
}
}
