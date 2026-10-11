// Observed memcpy at 0x301ffc, 176 bytes.
// Count below 32, or either address not 16-byte aligned, is a forward byte copy.
// Otherwise each step captures 16 bytes, stores them, then captures the next 16.
// That store happens before the second capture. Eight-byte steps follow, then bytes.
// The function returns the original destination. Count zero writes nothing.
extern "C" void* memcpy(void* destination, const void* source, unsigned count) {
    unsigned char* dest = static_cast<unsigned char*>(destination);
    const unsigned char* src = static_cast<const unsigned char*>(source);
    unsigned char* const result = dest;
    const unsigned long long combined = reinterpret_cast<unsigned long long>(destination) |
                                        reinterpret_cast<unsigned long long>(source);
    if (count >= 32u && (combined & 15ull) == 0ull) {
        do {
            unsigned char first[16];
            for (unsigned i = 0; i < 16u; ++i)
                first[i] = *reinterpret_cast<const volatile unsigned char*>(src + i);
            for (unsigned i = 0; i < 16u; ++i)
                *reinterpret_cast<volatile unsigned char*>(dest + i) = first[i];
            src += 16;
            dest += 16;
            unsigned char second[16];
            for (unsigned i = 0; i < 16u; ++i)
                second[i] = *reinterpret_cast<const volatile unsigned char*>(src + i);
            for (unsigned i = 0; i < 16u; ++i)
                *reinterpret_cast<volatile unsigned char*>(dest + i) = second[i];
            src += 16;
            dest += 16;
            count -= 32u;
        } while (count >= 32u);
        while (count >= 8u) {
            unsigned char block[8];
            for (unsigned i = 0; i < 8u; ++i)
                block[i] = *reinterpret_cast<const volatile unsigned char*>(src + i);
            for (unsigned i = 0; i < 8u; ++i)
                *reinterpret_cast<volatile unsigned char*>(dest + i) = block[i];
            src += 8;
            dest += 8;
            count -= 8u;
        }
    }
    while (count != 0u) {
        const unsigned char value = *reinterpret_cast<const volatile unsigned char*>(src);
        *reinterpret_cast<volatile unsigned char*>(dest) = value;
        src += 1;
        dest += 1;
        count -= 1u;
    }
    return result;
}
