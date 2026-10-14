// Observed: ReorderPalette__9VISurface at 0x0112c650 is 252 bytes. If the word at +16
// is not 1 it returns 0 untouched. Otherwise it switches on the word at +52:
//   2: for i = 0, 32, 64, ... while i < the word at +40 (re-read each pass), take the
//      address ([+312] + [+44] + 4 * i) and swap the eight 32-bit words at +32 with
//      the eight at +64 of that address;
//   5: if the word at +40 is positive, take address [+312] + [+44] once and for
//      i = 0, 32, ... while i < the count read once at the start, swap the eight
//      16-bit words at +16 with those at +32 of the address, then advance it by 64;
//   any other value: return -1 without touching memory.
// Both swap paths return 0.
// Inferred: a colour-palette block reorder (swap of the middle two groups of eight
// entries per 32 entries, for 32-bit and 16-bit entries); the 32-bit address words
// are an offset ([+44]) from a base ([+312]). Unknown: names of the format values
// and the object's other fields.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int ReorderPalette__9VISurface(unsigned char* self) {
    if (*reinterpret_cast<int*>(self + 16) != 1)
        return 0;
    int format = *reinterpret_cast<int*>(self + 52);
    if (format == 2) {
        int count = *reinterpret_cast<int*>(self + 40);
        if (count > 0) {
            for (int i = 0; i < count; ) {
                unsigned address = *reinterpret_cast<unsigned*>(self + 312) + (*reinterpret_cast<unsigned*>(self + 44) + i * 4);
                unsigned* a = reinterpret_cast<unsigned*>(at(address + 32));
                unsigned* b = reinterpret_cast<unsigned*>(at(address + 64));
                for (int j = 0; j < 8; ++j) {
                    unsigned t = a[j];
                    a[j] = b[j];
                    b[j] = t;
                }
                i += 32;
                count = *reinterpret_cast<int*>(self + 40);
            }
        }
        return 0;
    }
    if (format != 5)
        return -1;
    int count = *reinterpret_cast<int*>(self + 40);
    if (count > 0) {
        unsigned address = *reinterpret_cast<unsigned*>(self + 44) + *reinterpret_cast<unsigned*>(self + 312);
        for (int i = 0; i < count; i += 32) {
            unsigned short* a = reinterpret_cast<unsigned short*>(at(address + 16));
            unsigned short* b = reinterpret_cast<unsigned short*>(at(address + 32));
            for (int j = 0; j < 8; ++j) {
                unsigned short t = a[j];
                a[j] = b[j];
                b[j] = t;
            }
            address += 64;
        }
    }
    return 0;
}
}
