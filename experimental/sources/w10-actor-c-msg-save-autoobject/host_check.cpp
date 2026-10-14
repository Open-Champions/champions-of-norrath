#include <cstdio>
#include <cstring>
extern "C" int msg_save__10AutoObjectPc(void*, char*);
int main() {
    unsigned char object[64];
    char buffer[16];
    std::memset(object, 0x5a, sizeof object);
    std::memset(buffer, 0x33, sizeof buffer);
    int failures = 0;
    if (msg_save__10AutoObjectPc(object, buffer) != 0) { std::fprintf(stderr, "fail: return\n"); ++failures; }
    for (char c : buffer) if (c != 0x33) { std::fprintf(stderr, "fail: buffer written\n"); ++failures; break; }
    for (unsigned char c : object) if (c != 0x5a) { std::fprintf(stderr, "fail: object written\n"); ++failures; break; }
    return failures ? 1 : 0;
}
