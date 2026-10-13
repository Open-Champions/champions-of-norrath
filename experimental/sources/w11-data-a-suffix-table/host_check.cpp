// Host contract for _4item$suffixTable. Not an EE plan input.
struct Row {
    const char* p0;
    unsigned w1;
    const char* p2;
    const char* p3;
    const char* p4;
};
extern "C" Row _4item$suffixTable[91];
int main() {
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 91u; ++i) {
        const unsigned* w = reinterpret_cast<const unsigned*>(&_4item$suffixTable[i]);
        (void)w;
        if (_4item$suffixTable[i].p0) { for (const char* q = _4item$suffixTable[i].p0; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        hash = (hash ^ _4item$suffixTable[i].w1) * 0x01000193u;
        if (_4item$suffixTable[i].p2) { for (const char* q = _4item$suffixTable[i].p2; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        if (_4item$suffixTable[i].p3) { for (const char* q = _4item$suffixTable[i].p3; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
        if (_4item$suffixTable[i].p4) { for (const char* q = _4item$suffixTable[i].p4; *q; ++q) hash = (hash ^ static_cast<unsigned char>(*q)) * 0x01000193u; hash = (hash ^ 0xffu) * 0x01000193u; } else hash = (hash ^ 0u) * 0x01000193u;
    }
    if (_4item$suffixTable[90].p0 != nullptr) return 1;
    return hash == 1234074125u ? 0 : 2;
}
