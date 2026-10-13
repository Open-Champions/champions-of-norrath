// Host contract for _4item$namedItemTable. Not an EE plan input.
struct ItemObserved {
    const char* name;
    unsigned observed[15];
};
extern "C" ItemObserved _4item$namedItemTable[733];
int main() {
    unsigned n = 0;
    while (_4item$namedItemTable[n].name) ++n;
    if (n != 732u) return 2;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 733u; ++i) {
        for (const char* p = _4item$namedItemTable[i].name ? _4item$namedItemTable[i].name : ""; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        for (unsigned j = 0; j < 15u; ++j) hash = (hash ^ _4item$namedItemTable[i].observed[j]) * 0x01000193u;
    }
    return hash == 1835837510u ? 0 : 3;
}
