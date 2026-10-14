// Host check for the lumps table extent. Not an EE plan input.
#include <cstdio>
#include "lumps.cpp"
int main() {
    int failures = 0;
    if (sizeof(LumpEntry) != 16 + sizeof(void*) + 20 && sizeof(void*) == 4) { std::fprintf(stderr, "fail: entry size\n"); ++failures; }
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(lumps);
    for (unsigned i = 0; i < sizeof lumps; ++i) if (bytes[i]) { std::fprintf(stderr, "fail: not zero\n"); ++failures; break; }
    if (failures) return 1;
    std::printf("lumps storage check passed\n");
    return 0;
}
