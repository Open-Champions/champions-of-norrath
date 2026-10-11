#include <cstdint>
#include <cstdio>
extern "C" void norrath_archive_engine_push_border(unsigned color);
static unsigned char sentinel[64];
static unsigned fingerprint() {
    unsigned sum = 0;
    for (unsigned char byte : sentinel) sum = sum * 33u + byte;
    return sum;
}
int main() {
    for (int i = 0; i < 64; ++i) sentinel[i] = static_cast<unsigned char>(i * 3 + 1);
    unsigned before = fingerprint();
    norrath_archive_engine_push_border(0x00ffffffu);
    norrath_archive_engine_push_border(0u);
    if (fingerprint() != before) return 1;
    std::puts("push border contract passed");
    return 0;
}
