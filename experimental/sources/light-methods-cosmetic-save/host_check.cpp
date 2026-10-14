#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" int norrath_vt_msg_save_12CosmeticProp(void* object, unsigned char* destination);

static int failures = 0;
static std::vector<unsigned> seen;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static int read_half(const unsigned char* p) {
    unsigned value = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_float(unsigned char* p, float value) {
    union {
        float number;
        unsigned bits;
    } converted;
    converted.number = value;
    write_word(p, converted.bits);
}

extern "C" unsigned objectSaveAddToStringPool__FPCc(const char* text) {
    seen.push_back(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(text)));
    return 100u + static_cast<unsigned>(seen.size());
}

int main() {
    unsigned char object[96];
    unsigned char destination[24];
    std::memset(object, 0x3c, sizeof object);
    std::memset(destination, 0xa5, sizeof destination);
    write_word(object + 76, 0x11110001u);
    write_word(object + 80, 0x22220002u);
    write_word(object + 84, 0x33330003u);
    write_word(object + 12, 0x44556677u);
    write_float(object + 32, 1.5f);
    write_float(object + 28, -1.5f);
    int count = norrath_vt_msg_save_12CosmeticProp(object, destination);
    expect(count == 20, "returns 20");
    expect(destination[20] == 0xa5 && destination[21] == 0xa5, "does not write past 20 bytes");
    expect(seen.size() == 3u && seen[0] == 0x11110001u && seen[1] == 0x22220002u && seen[2] == 0x33330003u,
           "pools the three pointers in order");
    expect(read_word(destination) == 101u && read_word(destination + 4) == 102u && read_word(destination + 8) == 103u,
           "stores the pool results");
    expect(read_word(destination + 12) == 0x44556677u, "stores the word at object+12");
    expect(read_half(destination + 16) == 2, "1.5 rounds to even 2");
    expect(read_half(destination + 18) == -2, "-1.5 rounds to even -2");

    seen.clear();
    write_float(object + 32, 0.5f);
    write_float(object + 28, 65536.0f);
    count = norrath_vt_msg_save_12CosmeticProp(object, destination);
    expect(count == 20 && read_half(destination + 16) == 0, "0.5 ties to even 0");
    expect(read_half(destination + 18) == 0, "65536 keeps the low 16 bits");
    return failures == 0 ? 0 : 1;
}
