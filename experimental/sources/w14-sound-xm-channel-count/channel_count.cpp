// Observed: ChannelCount__C4VIXm at 0x0113a878, 24 bytes. It reads the pointer at +36 of the object. If it is null the result is
// zero; otherwise the result is the signed halfword at +8 of the record that pointer addresses. Nothing is written.
// Inferred: the pointer at +36 addresses a per-song record whose halfword at +8 is the channel count.
// Unknown: the record type and which routine fills the pointer.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline const short* half_of(unsigned address) { return reinterpret_cast<const short*>(norrath_host_pointer(address)); }
#else
static inline const short* half_of(unsigned address) { return reinterpret_cast<const short*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int ChannelCount__C4VIXm(const void* xm) {
    unsigned record = static_cast<const unsigned*>(xm)[9];
    if (record == 0) return 0;
    return half_of(record)[4];
}
}
