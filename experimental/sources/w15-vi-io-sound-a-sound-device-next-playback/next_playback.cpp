// Observed: NextPlayback__13VISoundDeviceP10VIPlayback at 0x011269f0, 16 bytes. Arguments: the device (unused) and a playback
// record. Loads the first word of the record (N) and then the first word of N. If that is nonzero the result is N, otherwise 0.
// Inferred: see FirstPlayback__13VISoundDevice; the first word is the link to the following record and a record whose own link is
// zero is the chain terminator, not a playback.
// Unknown: the member name of the link.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(unsigned char*, unsigned playback) {
    unsigned node = word_at(bytes_at(playback), 0);
    return word_at(bytes_at(node), 0) != 0 ? node : 0;
}
}
