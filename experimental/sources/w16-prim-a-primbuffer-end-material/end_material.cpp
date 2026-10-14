// Observed: EndMaterial__12VIPrimBuffer at 0x01102c98 is 52 bytes. With the material index at
// offset 64, it addresses the 16-byte record at (pointer at offset 88) + index * 16, stores
// (word at offset 80) minus (record word at offset 8) into record offset 12, then increments the
// index word and returns 0.
// Inferred: offset 80 is a running vertex or element count; record offset 8 is its value when the
// material began (see BeginMaterial), so offset 12 receives the material's length.
// Unknown: the units of the counter and the first two record words.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int EndMaterial__12VIPrimBuffer(unsigned char* self) {
    int index = *reinterpret_cast<int*>(self + 64);
    int* record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 88)) + index * 16);
    record[3] = *reinterpret_cast<int*>(self + 80) - record[2];
    *reinterpret_cast<int*>(self + 64) = *reinterpret_cast<int*>(self + 64) + 1;
    return 0;
}
}
