// Observed: BeginMaterial__12VIPrimBufferi at 0x01102c48 is 80 bytes. With the material index at
// offset 64, it addresses the 16-byte record at (pointer at offset 88) + index * 16 and stores:
// the argument at record offset 0, zero at offset 4, the current counter (object offset 80) at
// offset 8, and zero at offset 12; it returns 0. It does not advance the index (EndMaterial does).
// Inferred: the record describes one material run; offset 12 is filled in later by EndMaterial
// as counter-at-end minus the counter saved here. Unknown: the argument's meaning
// and the record's word at offset 4.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int BeginMaterial__12VIPrimBufferi(unsigned char* self, int material) {
    unsigned char* records = at(*reinterpret_cast<unsigned*>(self + 88));
    int* record = reinterpret_cast<int*>(records + *reinterpret_cast<int*>(self + 64) * 16);
    record[0] = material;
    record = reinterpret_cast<int*>(records + *reinterpret_cast<int*>(self + 64) * 16);
    record[2] = *reinterpret_cast<int*>(self + 80);
    record = reinterpret_cast<int*>(records + *reinterpret_cast<int*>(self + 64) * 16);
    record[3] = 0;
    record = reinterpret_cast<int*>(records + *reinterpret_cast<int*>(self + 64) * 16);
    record[1] = 0;
    return 0;
}
}
