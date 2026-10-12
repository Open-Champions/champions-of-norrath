#include "print_stream.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
int main() {
    unsigned char before[sizeof stream];
    std::memset(stream, 0x5c, sizeof stream);
    int count = 1000; std::memcpy(stream + 452 + 448, &count, 4);
    std::memcpy(before, stream, sizeof stream);
    soundPrintStream__Fi(1);
    if (std::memcmp(before, stream, sizeof stream) != 0) return 1;      // no byte changes
    count = -5; std::memcpy(stream + 448, &count, 4);
    std::memcpy(before, stream, sizeof stream);
    soundPrintStream__Fi(0);
    if (std::memcmp(before, stream, sizeof stream) != 0) return 2;
    count = 0; std::memcpy(stream + 2 * 452 + 448, &count, 4);
    soundPrintStream__Fi(2);
    std::puts("print_stream passed");
    return 0;
}
