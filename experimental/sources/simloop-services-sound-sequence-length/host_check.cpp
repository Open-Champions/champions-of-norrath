#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>
static uintptr_t pointer_high = 0;
static unsigned char* page = nullptr;
extern "C" void* norrath_host_pointer(unsigned address) {
    return address == 0 ? nullptr : reinterpret_cast<void*>(pointer_high | address);
}
extern "C" int convertEnd__Fi(int value) { return value == 100 ? 4 : value; }
extern "C" float soundComputeSequenceLength__FP15AnimationHeader(unsigned char*);
static void set_word(unsigned char* at, unsigned value) { std::memcpy(at, &value, 4); }
static unsigned low(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer)); }
int main() {
    page = static_cast<unsigned char*>(mmap(nullptr, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0));
    if (page == MAP_FAILED) return 90;
    pointer_high = reinterpret_cast<uintptr_t>(page) & ~uintptr_t{0xffffffffu};
    if ((reinterpret_cast<uintptr_t>(page) >> 32) != ((reinterpret_cast<uintptr_t>(page) + 0xfff) >> 32)) return 91;
    unsigned char* header = page;
    unsigned char* keys = page + 0x100;
    unsigned char* table = page + 0x200;
    unsigned char* record = page + 0x300;
    if (soundComputeSequenceLength__FP15AnimationHeader(header) != 0.0f) return 1;
    set_word(header + 56, low(keys));
    set_word(header + 52, low(table));
    set_word(keys, 0x7fffffff);
    if (soundComputeSequenceLength__FP15AnimationHeader(header) != 0.0f) return 2;
    set_word(keys, 60);
    keys[4] = 0xff; keys[5] = 0xff;
    set_word(keys + 28, 120);
    keys[28 + 4] = 1;
    set_word(keys + 56, 30);
    keys[56 + 4] = 0;
    set_word(keys + 84, 0x7fffffff);
    set_word(table + 4, 0);
    set_word(table + 12, low(record));
    set_word(record + 44, 16 * 3);
    set_word(record + 16, 100);
    const float expected = 120.0f * (1.0f / 60.0f) + 84.0f / 4.0f;
    const float got = soundComputeSequenceLength__FP15AnimationHeader(header);
    if (got < expected - 0.001f || got > expected + 0.001f) return 3;
    set_word(table + 12, 0);
    const float plain = soundComputeSequenceLength__FP15AnimationHeader(header);
    if (plain < 1.99f || plain > 2.01f) return 4;
    std::puts("sequence length contract passed");
    return 0;
}
