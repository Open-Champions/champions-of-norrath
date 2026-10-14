// Host check (not a plan input). Build:
//   c++ -O1 -Dstrupr=w16_strupr -o /tmp/w16_strupr host_check.cpp strupr.cpp && /tmp/w16_strupr
// The table stand-in marks 'a'..'z' with bit 1 and also marks one high-bit byte
// (0xe9) so that a sign-extension error in the index would change the result.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_factory_ctype_minimum_view[384];
extern "C" char* w16_strupr(char* text);
unsigned char norrath_factory_ctype_minimum_view[384];

int main() {
    for (int c = 'a'; c <= 'z'; ++c) norrath_factory_ctype_minimum_view[128 + c] = 2;
    for (int c = 'A'; c <= 'Z'; ++c) norrath_factory_ctype_minimum_view[128 + c] = 1;
    norrath_factory_ctype_minimum_view[128 + static_cast<signed char>(0xe9)] = 2;
    norrath_factory_ctype_minimum_view[128 + '0'] = 4;
    unsigned cases = 0;
    char buffer[64];
    const char* inputs[] = {"", "abc", "Hello, World 0123", "zzzz{`@[", "\xe9x\xe9", "already UPPER", "a"};
    for (const char* input : inputs) {
        std::strcpy(buffer, input);
        char* result = w16_strupr(buffer);
        if (result != buffer) return 1;
        char expected[64];
        std::strcpy(expected, input);
        for (char* p = expected; *p; ++p) {
            const int v = *p;
            if ((v >= 'a' && v <= 'z') || static_cast<unsigned char>(v) == 0xe9) *p = static_cast<char>(v - 32);
        }
        if (std::strcmp(buffer, expected) != 0) { std::printf("mismatch on %s\n", input); return 2; }
        ++cases;
    }
    // Bytes after the terminator are untouched.
    std::memcpy(buffer, "ab\0cd", 6);
    w16_strupr(buffer);
    if (std::memcmp(buffer, "AB\0cd", 6) != 0) return 3;
    ++cases;
    std::printf("%u strupr cases passed\n", cases);
    return 0;
}
