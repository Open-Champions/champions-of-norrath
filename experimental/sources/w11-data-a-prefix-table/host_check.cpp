// Host contract for _4item$prefixTable. Not an EE plan input.
struct Row {
    const char* p0;
    unsigned w1;
    unsigned w2;
    const char* p3;
    const char* p4;
    unsigned w5;
    unsigned w6;
    unsigned w7;
    unsigned w8;
    unsigned w9;
};
extern "C" Row _4item$prefixTable[58];
int main() {
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 58u; ++i) {
        const unsigned* w = reinterpret_cast<const unsigned*>(&_4item$prefixTable[i]);
        (void)w;
        if (_4item$prefixTable[i].p0) { for (const char* q = _4item$prefixTable[i].p0; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w1) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w2) * 0x01000193u;
        if (_4item$prefixTable[i].p3) { for (const char* q = _4item$prefixTable[i].p3; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        if (_4item$prefixTable[i].p4) { for (const char* q = _4item$prefixTable[i].p4; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w5) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w6) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w7) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w8) * 0x01000193u;
        hash = (hash ^ _4item$prefixTable[i].w9) * 0x01000193u;
    }
    if (_4item$prefixTable[57].p0 != nullptr) return 1;
    return hash == 1329058979u ? 0 : 2;
}
