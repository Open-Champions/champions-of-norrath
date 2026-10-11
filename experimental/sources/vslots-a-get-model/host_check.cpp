#include <cstdio>
#include <cstdint>
#include <cstring>

#define NORRATH_VSLOTS_A_HOST
#include "get_model.cpp"

unsigned long long norrath_vslots_a_host_base;
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

int main() {
    alignas(16) unsigned char memory[64];
    std::memset(memory, 0xab, sizeof memory);
    norrath_vslots_a_host_base = reinterpret_cast<unsigned long long>(memory);
    write_word(memory + 36, 16);
    write_word(memory + 16 + 12, 0x89abcdefu);
    expect(getModel__8Creature(memory) == 0x89abcdefu, "word at model+12");
    memory[36] = 0;
    write_word(memory + 36, 4);
    write_word(memory + 4 + 12, 0x01020304u);
    expect(getModel__8Creature(memory) == 0x01020304u, "second model word");
    return failures ? 1 : 0;
}
