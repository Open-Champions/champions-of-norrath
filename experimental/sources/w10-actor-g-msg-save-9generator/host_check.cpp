// Host contract for msg_save__9GeneratorPc; not an EE input. Includes the source.
#include "msg_save.cpp"
#include <cstdio>
#include <cstring>
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
int main() {
    alignas(16) unsigned char object[1024]; alignas(16) char buffer[16];
    std::memset(object, 0, sizeof object);
    short s = -3; std::memcpy(object + 572, &s, 2);
    unsigned w = 0x89abcdefu; std::memcpy(object + 976, &w, 4);
    std::memset(buffer, 0x55, sizeof buffer);
    int n = msg_save__9GeneratorPc(object, buffer);
    unsigned a, b; std::memcpy(&a, buffer, 4); std::memcpy(&b, buffer + 4, 4);
    expect(n == 8, "returns 8");
    expect(a == 0xfffffffdu, "halfword saved sign-extended");
    expect(b == w, "word at +976 saved second");
    expect(static_cast<unsigned char>(buffer[8]) == 0x55, "nothing past 8 bytes");
    if (!failures) std::puts("generator msg_save ok");
    return failures ? 1 : 0;
}
