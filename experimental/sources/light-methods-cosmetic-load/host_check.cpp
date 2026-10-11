#include <cstdint>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

extern "C" int norrath_vt_msg_load_12CosmeticProp(void* object, const unsigned char* source);

static int failures = 0;
static std::vector<int> pool_args;
static std::vector<std::pair<unsigned, unsigned>> finds;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void write_half(unsigned char* p, int value) {
    unsigned bits = static_cast<unsigned>(value);
    p[0] = static_cast<unsigned char>(bits);
    p[1] = static_cast<unsigned char>(bits >> 8);
}
static float read_float(const unsigned char* p) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = read_word(p);
    return converted.number;
}

extern "C" char* objectSaveGetFromStringPool__Fi(int index) {
    pool_args.push_back(index);
    return reinterpret_cast<char*>(static_cast<std::uintptr_t>(0x01000000u + static_cast<unsigned>(pool_args.size())));
}
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    finds.push_back({static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(lump)),
                     static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(name))});
    return reinterpret_cast<char*>(static_cast<std::uintptr_t>(0x02000000u + static_cast<unsigned>(finds.size())));
}

int main() {
    unsigned char object[96];
    unsigned char source[20];
    std::memset(object, 0xa5, sizeof object);
    write_word(source, 11u);
    write_word(source + 4, 22u);
    write_word(source + 8, 33u);
    write_word(source + 12, 0x89abcdefu);
    write_half(source + 16, 1);
    write_half(source + 18, -2);
    int count = norrath_vt_msg_load_12CosmeticProp(object, source);
    expect(count == 20, "returns 20");
    expect(pool_args.size() == 3u && pool_args[0] == 11 && pool_args[1] == 22 && pool_args[2] == 33,
           "resolves the first three words in order");
    expect(read_word(object + 76) == 0x01000001u && read_word(object + 80) == 0x01000002u &&
               read_word(object + 84) == 0x01000003u,
           "stores the three pool pointers");
    expect(read_word(object + 12) == 0x89abcdefu, "stores the fourth word at object+12");
    expect(read_float(object + 32) == 1.0f && object[41] == 1, "the first half restores float 32 and byte 41");
    expect(read_float(object + 28) == -2.0f && object[40] == 0xfe, "the second half restores float 28 and byte 40");
    expect(finds.size() == 2u && finds[0].first == 0x01000001u && finds[0].second == 0x01000002u,
           "the first resource lookup uses the first two strings");
    expect(finds[1].first == 0x01000001u && finds[1].second == 0x01000003u,
           "the second resource lookup uses the first and third strings");
    expect(read_word(object + 88) == 0x02000001u && read_word(object + 92) == 0x02000002u,
           "stores the two resource pointers");
    expect(object[42] == 0xa5 && object[16] == 0xa5 && object[56] == 0xa5 && object[72] == 0xa5,
           "leaves unrelated object bytes unchanged");
    return failures == 0 ? 0 : 1;
}
