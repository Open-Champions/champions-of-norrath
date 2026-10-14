#include <cstdio>
extern "C" void norrath_archive_engine_pop_border();
static unsigned char sentinel[64];
static unsigned fingerprint() {
    unsigned sum = 0;
    for (unsigned char byte : sentinel) sum = sum * 33u + byte;
    return sum;
}
int main() {
    for (int i = 0; i < 64; ++i) sentinel[i] = static_cast<unsigned char>(255 - i);
    unsigned before = fingerprint();
    norrath_archive_engine_pop_border();
    norrath_archive_engine_pop_border();
    if (fingerprint() != before) return 1;
    std::puts("pop border contract passed");
    return 0;
}
