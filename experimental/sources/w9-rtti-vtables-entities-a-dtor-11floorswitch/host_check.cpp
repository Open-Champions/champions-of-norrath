// Host check only. Not an EE build input.
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void dtor_11FloorSwitch(void* object, int flags) asm("_$_11FloorSwitch");
extern "C" unsigned char norrath_actors_a_view_449790[8] = {};
extern "C" void worldpart_destructor(void* object, int flags) asm("_$_9WorldPart");

static int failures = 0, steps = 0, free_calls = 0, order[8];
static unsigned char* seen_object[8];
static int seen_flag[8];
static unsigned char* freed;
static unsigned vtable_at_first_call;
static unsigned char* current;

static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void note(int index, void* object, int flags) {
    if (steps == 0) vtable_at_first_call = read_word(current + 72);
    order[steps] = index;
    seen_object[steps] = static_cast<unsigned char*>(object);
    seen_flag[steps] = flags;
    ++steps;
}
extern "C" void worldpart_destructor(void* object, int flags) { note(0, object, flags); }
extern "C" void norrath_block_free(void* object) {
    expect(steps == 1, "free comes after every destructor call");
    ++free_calls;
    freed = static_cast<unsigned char*>(object);
}
static void reset() { steps = 0; free_calls = 0; freed = nullptr; vtable_at_first_call = 0; }

int main() {
    unsigned char object[256];
    unsigned char before[256];
    std::memset(object, 0x5a, sizeof object);
    std::memcpy(before, object, sizeof object);
    current = object;
    const unsigned want = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actors_a_view_449790));
    const int flags_to_try[] = {0, 1, 2, 3, 5, -1, -2};
    for (int flags : flags_to_try) {
        reset();
        std::memcpy(object, before, sizeof object);
        dtor_11FloorSwitch(object, flags);
        expect(steps == 1, "number of destructor calls");
    expect(order[0] == 0 && seen_object[0] == object + 0 && seen_flag[0] == 0, "step 0 arguments");
        expect(vtable_at_first_call == want, "vtable stored before the first call");
        expect(free_calls == ((flags & 1) ? 1 : 0), "free only when bit 0 is set");
        if (flags & 1) expect(freed == object, "free receives the object");
        expect(std::memcmp(object, before, 72) == 0 && std::memcmp(object + 76, before + 76, 180) == 0, "only the word at +72 is written");
        expect(read_word(object + 72) == want, "vtable word persists");
    }
    if (failures == 0) std::printf("11FloorSwitch destructor cases passed\n");
    return failures == 0 ? 0 : 1;
}
