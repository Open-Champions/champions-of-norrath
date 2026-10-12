#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void dtor_14AnimationState(void* object, int flags) asm("_$_14AnimationState");

// The destructor handles 32-bit pointer words; a host base maps them to host memory.
unsigned long long norrath_w8av_host_base;

static int failures = 0;
static int deletes = 0;
static void* deleted = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" void __builtin_delete(void* pointer) {
    ++deletes;
    deleted = pointer;
}

static unsigned char* low_block(std::size_t size) {
    static unsigned char arena[1 << 16] __attribute__((aligned(16)));
    static std::size_t used = 0;
    unsigned char* p = arena + used;
    used += size;
    return p;
}
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p) - norrath_w8av_host_base); }
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void put_word(unsigned char* p, unsigned v) {
    p[0] = static_cast<unsigned char>(v); p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16); p[3] = static_cast<unsigned char>(v >> 24);
}

int main() {
    norrath_w8av_host_base = reinterpret_cast<std::uintptr_t>(low_block(0)) - 0x1000;
    constexpr std::size_t kSize = 12 + 32 * 8;
    unsigned char* object = low_block(kSize);
    unsigned char* owner = low_block(1600);
    unsigned char* kids[8];
    for (auto& k : kids) k = low_block(64);

    // Nothing linked, count zero: nothing is written.
    std::memset(object, 0xA5, kSize);
    put_word(object, 0);
    object[5] = 0;
    unsigned char before[kSize];
    std::memcpy(before, object, kSize);
    dtor_14AnimationState(object, 0);
    expect(std::memcmp(object, before, kSize) == 0 && deletes == 0, "null owner and zero count change nothing");

    // Owner link and three children.
    std::memset(owner, 0xEE, 1600);
    for (auto k : kids) std::memset(k, 0xEE, 64);
    put_word(object, word_of(owner));
    object[5] = 3;
    for (int i = 0; i < 8; ++i) put_word(object + 12 + 32 * i, word_of(kids[i]));
    dtor_14AnimationState(object, 2);
    expect(read_word(object) == 0, "owner pointer cleared");
    expect(read_word(owner + 1552) == 0 && owner[1551] == 0xEE && owner[1556] == 0xEE, "word at owner+1552 cleared, neighbours kept");
    for (int i = 0; i < 3; ++i) {
        expect(read_word(object + 12 + 32 * i) == 0, "child slot cleared");
        expect(read_word(kids[i] + 20) == 0 && kids[i][19] == 0xEE && kids[i][24] == 0xEE, "child word at +20 cleared");
    }
    expect(read_word(object + 12 + 96) == word_of(kids[3]) && read_word(kids[3] + 20) == 0xEEEEEEEE, "slots at and above the count are untouched");
    expect(object[4] == 0xA5, "byte +4 is not written");
    expect(deletes == 0, "flag 2 does not delete");

    // Null slots are skipped, negative counts do nothing.
    put_word(object + 12, 0);
    for (auto k : kids) std::memset(k, 0xEE, 64);
    put_word(object + 12 + 32, word_of(kids[1]));
    object[5] = 0xFF;
    dtor_14AnimationState(object, 0);
    expect(read_word(object + 12 + 32) == word_of(kids[1]) && read_word(kids[1] + 20) == 0xEEEEEEEE, "a count of -1 clears no slot");
    object[5] = 2;
    dtor_14AnimationState(object, 1);
    expect(read_word(object + 12 + 32) == 0 && read_word(kids[1] + 20) == 0, "non-null slot cleared after a null one");
    expect(deletes == 1 && deleted == object, "flag 1 deletes the object");
    dtor_14AnimationState(object, 3);
    expect(deletes == 2, "odd flag deletes");
    return failures == 0 ? 0 : 1;
}
