// Observed: PrimBuffer__8VIRasteri at 0x01113370 is 28 bytes. It reads the pointer word at offset
// 19328 of the raster object, indexes it with the int argument in 12-byte steps, and returns the
// word at the start of that element.
// Inferred: an accessor for a table of prim buffers kept by the raster object; the returned word
// is the pointer to the buffer. Unknown: the table's capacity, the use of the other eight bytes
// of each 12-byte element, and whether the index is range checked elsewhere (it is not here).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int PrimBuffer__8VIRasteri(unsigned char* self, int index) {
    unsigned table = *reinterpret_cast<unsigned*>(self + 19328);
    return *reinterpret_cast<int*>(at(table + static_cast<unsigned>(index * 12)));
}
}
