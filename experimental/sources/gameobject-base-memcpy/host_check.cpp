#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" void* norrath_memcpy_under_test(void* destination, const void* source, unsigned count);

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static void fill(unsigned char* p, unsigned n, unsigned char value) {
    for (unsigned i = 0; i < n; ++i) p[i] = value;
}

static void check_copy(unsigned count, unsigned dest_align, unsigned src_align, const char* label) {
    alignas(16) unsigned char raw[512];
    fill(raw, sizeof raw, 0xee);
    unsigned char* dest = raw + 64 + dest_align;
    unsigned char* src = raw + 256 + src_align;
    for (unsigned i = 0; i < count + 8u; ++i) src[i] = static_cast<unsigned char>(0x30 + (i * 17u));
    std::vector<unsigned char> before(src, src + count + 8u);
    void* returned = norrath_memcpy_under_test(dest, src, count);
    expect(returned == dest, label);
    for (unsigned i = 0; i < count; ++i) {
        if (dest[i] != before[i]) {
            std::fprintf(stderr, "fail: %s byte %u\n", label, i);
            ++failures;
            break;
        }
    }
    expect(dest[count] == 0xee, label);
    bool source_intact = true;
    for (unsigned i = 0; i < count + 8u; ++i)
        if (src[i] != before[i]) source_intact = false;
    expect(source_intact, label);
}

int main() {
    check_copy(0, 0, 0, "empty");
    check_copy(1, 0, 0, "one");
    check_copy(7, 1, 3, "seven unaligned");
    check_copy(8, 0, 0, "eight");
    check_copy(15, 2, 1, "fifteen");
    check_copy(16, 0, 0, "sixteen");
    check_copy(31, 0, 0, "thirty-one aligned");
    check_copy(31, 1, 1, "thirty-one unaligned");
    check_copy(32, 0, 0, "thirty-two aligned");
    check_copy(32, 1, 0, "thirty-two dest unaligned");
    check_copy(32, 0, 2, "thirty-two source unaligned");
    check_copy(33, 0, 0, "thirty-three");
    check_copy(40, 0, 0, "forty");
    check_copy(63, 4, 4, "sixty-three");
    check_copy(64, 0, 0, "sixty-four");

    alignas(16) unsigned char overlap[64];
    for (unsigned i = 0; i < 64u; ++i) overlap[i] = static_cast<unsigned char>(i + 1u);
    void* returned = norrath_memcpy_under_test(overlap + 16, overlap, 32);
    expect(returned == overlap + 16, "overlap return");
    bool propagated = true;
    for (unsigned i = 0; i < 16u; ++i)
        if (overlap[i] != static_cast<unsigned char>(i + 1u)) propagated = false;
    for (unsigned i = 16; i < 48u; ++i)
        if (overlap[i] != static_cast<unsigned char>((i & 15u) + 1u)) propagated = false;
    expect(propagated, "aligned overlap stores the first 16 before reading the next 16");

    unsigned char bytes[16];
    for (unsigned i = 0; i < 16u; ++i) bytes[i] = static_cast<unsigned char>(0x41 + i);
    returned = norrath_memcpy_under_test(bytes + 1, bytes, 4);
    expect(returned == bytes + 1, "byte overlap return");
    expect(bytes[0] == 'A' && bytes[1] == 'A' && bytes[2] == 'A' && bytes[3] == 'A' && bytes[4] == 'A',
           "unaligned overlap copies forward");
    expect(bytes[5] == 'F', "byte overlap stops at the count");
    return failures == 0 ? 0 : 1;
}
