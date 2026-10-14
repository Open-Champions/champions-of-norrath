// Observed: FirstPlayback__13VISoundDevice at 0x011269e0, 16 bytes. Loads the word at device + 3284 (a node pointer, N) and then
// the first word of N. If that first word is nonzero the result is N, otherwise 0. No other state is touched.
// Inferred: the playback records form a chain whose first word links to the next record and whose last record has a zero link, so
// that last record is a terminator and never returned. This matches NextPlayback__13VISoundDeviceP10VIPlayback, which applies the
// same test to the node after its argument.
// Unknown: what the terminator record is, and the member name of +3284.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
unsigned FirstPlayback__13VISoundDevice(unsigned char* device) {
    unsigned node = word_at(device, 3284);
    return word_at(bytes_at(node), 0) != 0 ? node : 0;
}
}
