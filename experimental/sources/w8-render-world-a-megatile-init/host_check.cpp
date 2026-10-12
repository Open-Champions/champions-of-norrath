#define NORRATH_W8_HOST 1
#include <cstdio>
#include <cstring>
#include <vector>
alignas(16) static unsigned char arena[0x10000];
extern "C" unsigned norrath_w8_pointer_to_word(const void* pointer) { return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - arena); }
extern "C" void* norrath_w8_word_to_pointer(unsigned word) { return arena + word; }
unsigned norrath_w8_delay_steps;
extern "C" void* mgrg;
void* mgrg;
struct Call { int kind; unsigned record; unsigned base; };
static std::vector<Call> calls;
static int init_calls;
static void* switch_root_after_first_bank;
extern "C" void megatileBankInit__FP16MegatileInstBankPv(void* bank, void* base) {
    calls.push_back({0, norrath_w8_pointer_to_word(bank), norrath_w8_pointer_to_word(base)});
    if (switch_root_after_first_bank) { mgrg = switch_root_after_first_bank; switch_root_after_first_bank = nullptr; }
}
extern "C" void megatileItemInit__FP8MegatilePv(void* item, void* base) {
    calls.push_back({1, norrath_w8_pointer_to_word(item), norrath_w8_pointer_to_word(base)});
}
extern "C" void mtcInitialize__Fv() { ++init_calls; }
#include "megatile_init.cpp"
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static unsigned& word(unsigned address) { return *reinterpret_cast<unsigned*>(arena + address); }
static void set_half(unsigned address, short v) { std::memcpy(arena + address, &v, 2); }
int main() {
    const unsigned root = 0x1000, base = 0x4000;
    std::memset(arena, 0, sizeof arena);
    word(root + 28) = 2;  word(root + 32) = 0x100;       // two banks
    word(root + 36) = 3;  word(root + 40) = 0x400;       // three items
    word(root + 52) = 0x2000;                            // block, halfword mode 2
    set_half(root + 48, 2);
    word(base + 0x2000 + 12) = 7; word(base + 0x2000 + 12 + 8 * 32) = 9;
    word(base + 0x2000 + 276) = 11; word(base + 0x2000 + 276 + 8 * 32) = 13;
    word(root + 56) = 2; word(root + 84) = 3;
    word(root + 88) = 1; word(root + 92) = 2; word(root + 96) = 3; word(root + 100) = 50;
    word(root + 760) = 1; word(root + 764) = 2; word(root + 768) = 100; word(root + 772) = 200; word(root + 776) = 300;
    // Entries from +768 in 16-byte steps; the byte at entry + 16 is 1 for the first and 2 for the second.
    arena[root + 768 + 16] = 1; word(root + 768 + 20) = 1000; word(root + 768 + 24) = 2000; word(root + 768 + 28) = 3000;
    arena[root + 768 + 32] = 2; word(root + 768 + 36) = 4000; word(root + 768 + 40) = 5000; word(root + 768 + 44) = 6000;
    arena[root + 768 + 48] = 0;   // third entry ends the list
    word(root + 768 + 56) = 9999; word(root + 768 + 60) = 9999;
    init_calls = 0;
    megatileInit__FP12MegatileRootPv(arena + root, arena + base);

    expect(mgrg == arena + root, "mgrg is the root");
    expect(word(root + 32) == 0x4100 && word(root + 40) == 0x4400, "bank and item pointers relocated");
    expect(calls.size() == 5 && calls[0].kind == 0 && calls[0].record == 0x4100 && calls[1].record == 0x4100 + 12 &&
           calls[2].kind == 1 && calls[2].record == 0x4400 && calls[3].record == 0x4400 + 204 && calls[4].record == 0x4400 + 408,
           "bank stride 12, item stride 204");
    expect(calls[0].base == base && calls[4].base == base, "base is passed through");
    expect(word(root + 52) == 0x2000 + base, "block pointer relocated");
    expect(word(base + 0x2000 + 12) == 7 + base && word(base + 0x2000 + 12 + 8 * 32) == 9 + base, "block words at +12 + 8i, 33 of them");
    expect(word(base + 0x2000 + 276) == 11 + base && word(base + 0x2000 + 276 + 8 * 32) == 13 + base, "block words at +276 + 8i");
    expect(word(base + 0x2000 + 276 + 8 * 33) == 0, "the 34th word of the second run is not touched");
    expect(word(root + 84) == 3 && word(root + 88) == 1 + base && word(root + 96) == 3 + base && word(root + 100) == 50, "kind 2 relocates [+84] words from +88");
    expect(norrath_w8_delay_steps == 9, "empty 3 by 3 loop");
    expect(word(root + 768) == 100 + base && word(root + 772) == 200 + base && word(root + 776) == 300, "flag 1 relocates [+764] words");
    expect(word(root + 768 + 20) == 1000 + base, "entry with byte 1 relocates +20");
    expect(word(root + 768 + 24) == 2000 + base && word(root + 768 + 28) == 3000 + base, "entry words +24 and +28");
    expect(word(root + 768 + 36) == 4000 && word(root + 768 + 40) == 5000 + base && word(root + 768 + 44) == 6000 + base,
           "entry with byte 2 leaves +20 alone but relocates +24 and +28");
    expect(word(root + 768 + 56) == 9999, "a zero byte ends the entries");
    expect(init_calls == 1, "mtcInitialize once");

    // Kind 1 adds to [+84]; halfword 4 and a zero block pointer relocate nothing.
    std::memset(arena, 0, sizeof arena);
    calls.clear(); init_calls = 0;
    word(root + 56) = 1; word(root + 84) = 5; set_half(root + 48, 4); word(root + 52) = 0;
    megatileInit__FP12MegatileRootPv(arena + root, arena + base);
    expect(word(root + 84) == 5 + base && word(root + 52) == 0 && calls.empty() && init_calls == 1, "kind 1 and no block");
    expect(word(root + 32) == base && word(root + 40) == base, "pointers relocate even with zero counts");

    // The tail uses mgrg as read after the calls: a callee that switches mgrg redirects it.
    std::memset(arena, 0, sizeof arena);
    calls.clear();
    word(root + 28) = 1; word(root + 32) = 0x100;
    const unsigned other = 0x3000;
    arena[other + 784] = 1; word(other + 788) = 1; word(other + 792) = 2; word(other + 796) = 3;
    arena[root + 784] = 1; word(root + 788) = 1; word(root + 792) = 2; word(root + 796) = 3;
    switch_root_after_first_bank = arena + other;
    megatileInit__FP12MegatileRootPv(arena + root, arena + base);
    expect(word(other + 792) == 2 + base && word(root + 792) == 2, "the entries of the switched root are relocated");
    return failures;
}
