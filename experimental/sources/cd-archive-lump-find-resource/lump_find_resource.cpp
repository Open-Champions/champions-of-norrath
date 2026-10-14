// Observed lumpFindResource__FPcT0 at 0x00165dc0, 396 bytes. The cache is the
// same 40-byte lumps table. Bit 0 of the flags returns the resident pointer
// plus 256 when that pointer's text matches the resource. A clear bit keeps
// the pointer as the lump and stops the scan. A missing lump calls
// lumpLoad__FPc. The search is over 12-byte records at lump+4. The header
// halfword, minus one, is the initial upper bound. A hit returns the word at
// record+4, not the record.
extern "C" {
extern unsigned char norrath_constructor_lump_cache_view[];
extern unsigned char norrath_constructor_lump_count_view[4];
int strcmp(const char* left, const char* right);
void* lumpLoad__FPc(char* name);
static unsigned load_half(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8);
}
static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static int signed_word(unsigned value) {
    return static_cast<int>(value);
}
static unsigned arithmetic_shift(unsigned value, unsigned bits) {
    const unsigned shifted = value >> bits;
    if ((value & 0x80000000u) == 0u) return shifted;
    return shifted | (0xffffffffu << (32u - bits));
}
#if defined(NORRATH_HOST_CONTRACT)
void* norrath_host_pointer(unsigned address);
#endif
static void* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_host_pointer(address);
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
char* lumpFindResource__FPcT0(char* lump_name, char* resource_name) {
    unsigned char* cache = norrath_constructor_lump_cache_view;
    unsigned char* resident = nullptr;
    int count = signed_word(load_word(norrath_constructor_lump_count_view));
    if (count > 0) {
        unsigned index = 0u;
        unsigned offset = 0u;
        for (;;) {
            if (strcmp(reinterpret_cast<char*>(cache + offset), lump_name) == 0) {
                const unsigned flags = load_word(cache + offset + 20u);
                const unsigned data = load_word(cache + offset + 16u);
                if ((flags & 1u) != 0u) {
                    if (strcmp(reinterpret_cast<char*>(pointer_at(data)), resource_name) == 0)
                        return static_cast<char*>(pointer_at(data + 256u));
                } else {
                    resident = static_cast<unsigned char*>(pointer_at(data));
                    break;
                }
            }
            count = signed_word(load_word(norrath_constructor_lump_count_view));
            index += 1u;
            offset += 40u;
            if (!(signed_word(index) < count)) break;
        }
    }
    if (resident == nullptr) {
        resident = static_cast<unsigned char*>(lumpLoad__FPc(lump_name));
        if (resident == nullptr) return nullptr;
    }
    const int maximum = static_cast<int>(load_half(resident)) - 1;
    int lower = 0;
    int upper = maximum;
    unsigned char* const records = resident + 4;
    for (;;) {
        const unsigned sum = static_cast<unsigned>(lower) + static_cast<unsigned>(upper);
        const int middle = signed_word(arithmetic_shift(sum, 1u));
        int relation;
        if (middle < 0) relation = 1;
        else if (maximum < middle) relation = -1;
        else {
            const unsigned char* record = records + static_cast<unsigned>(middle) * 12u;
            const unsigned name = load_word(record);
            relation = strcmp(resource_name, static_cast<char*>(pointer_at(name)));
        }
        if (relation == 0) {
            const unsigned char* record = records + static_cast<unsigned>(middle) * 12u;
            return static_cast<char*>(pointer_at(load_word(record + 4)));
        }
        if (!(lower < upper)) return nullptr;
        if (relation <= 0) upper = middle - 1;
        else lower = middle + 1;
    }
}
}
