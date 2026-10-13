// Observed: Load__13VIStringTablePCc at 0x0101bd78, 192 bytes. With (this, name): builds a local 8-byte byte-array object with
// __t7VIArray1Zc, then calls LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(name, local) (it does not receive the table). Only when that is not negative is
// ProcessRawFile__13VIStringTableRCt7VIArray1Zc(this, local) called, and only when that is not negative too is
// FixupTextBuffer__13VIStringTable(this) called. The result is 0 when all three returned non-negative values, else -1.
// On failure the table is unloaded in line exactly as Unload__13VIStringTable does: delete the block at +8 if set, delete the block
// at +12 if set, then zero +12, +0, +4 and +8. In every case the local array is destroyed last with _$_t7VIArray1Zc(local, 2).
// Inferred: loads a text file into a raw byte buffer, parses it into the table, then fixes up pointers; the load is
// all-or-nothing as the cleanup shows.
// Unknown: the three callees' behaviour, the local array's layout (only its two constructor/destructor calls are used here).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline void* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline void* at(unsigned address) { return reinterpret_cast<void*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void __t7VIArray1Zc(void* array);
void _$_t7VIArray1Zc(void* array, int flags);
int LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(const char* name, void* raw);
int ProcessRawFile__13VIStringTableRCt7VIArray1Zc(void* self, const void* raw);
int FixupTextBuffer__13VIStringTable(void* self);
void __builtin_vec_delete(void* pointer);

int Load__13VIStringTablePCc(void* self, const char* name) {
    unsigned raw[2];
    unsigned* words = static_cast<unsigned*>(self);
    __t7VIArray1Zc(raw);
    int result = -1;
    if (LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(name, raw) >= 0 &&
        ProcessRawFile__13VIStringTableRCt7VIArray1Zc(self, raw) >= 0 &&
        FixupTextBuffer__13VIStringTable(self) >= 0)
        result = 0;
    if (result != 0) {
        if (words[2] != 0) __builtin_vec_delete(at(words[2]));
        if (words[3] != 0) __builtin_vec_delete(at(words[3]));
        words[3] = 0;
        words[0] = 0;
        words[1] = 0;
        words[2] = 0;
    }
    _$_t7VIArray1Zc(raw, 2);
    return result;
}
}
