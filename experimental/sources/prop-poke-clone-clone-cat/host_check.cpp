#include "clone.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" const unsigned char norrath_actor_view_4488f0[8] = {};
extern "C" const unsigned char norrath_actor_view_44c170[8] = {};
extern "C" const unsigned char norrath_game_object_vtable_view[8] = {};

// Steps in image order. Kind 0 copies bytes at the offset, 1 is a list-head
// copy constructor (offset, source offset), 2 is a state copy call (not
// defined here, stubbed to write nothing), 3 stores table number c at +72.
struct Step { int kind; unsigned a; unsigned b; };
static const Step steps[] = {
    {3, 72, 0},
    {1, 0, 0},
    {1, 4, 4},
    {1, 8, 8},
    {0, 12, 4},
    {0, 16, 8},
    {0, 24, 4},
    {0, 28, 4},
    {0, 36, 4},
    {0, 32, 4},
    {0, 40, 1},
    {3, 72, 1},
    {0, 41, 1},
    {0, 48, 4},
    {0, 60, 4},
    {0, 68, 4},
    {0, 52, 4},
    {0, 44, 4},
    {0, 56, 2},
    {0, 64, 4},
    {0, 76, 1},
    {0, 80, 4},
    {0, 84, 1},
    {0, 85, 1},
    {0, 88, 20},
    {0, 108, 16},
    {0, 124, 16},
    {0, 140, 8},
    {0, 148, 8},
    {0, 156, 1},
    {0, 168, 4},
    {0, 160, 8},
    {0, 172, 1},
    {0, 173, 1},
    {0, 176, 4},
    {0, 180, 8},
    {0, 188, 2},
    {0, 192, 4},
    {0, 200, 2},
    {0, 212, 4},
    {0, 202, 2},
    {0, 196, 4},
    {0, 204, 2},
    {0, 208, 4},
    {0, 216, 4},
    {0, 244, 4},
    {0, 220, 4},
    {0, 248, 4},
    {0, 236, 4},
    {0, 252, 4},
    {0, 224, 4},
    {0, 232, 2},
    {0, 228, 4},
    {0, 240, 4},
    {0, 256, 4},
    {0, 260, 8},
    {0, 268, 4},
    {0, 272, 8},
    {0, 280, 4},
    {0, 284, 2},
    {2, 288, 288},  // __15AnimationState8RC15AnimationState8
    {0, 552, 8},
    {0, 560, 4},
    {0, 564, 4},
    {0, 568, 1},
    {0, 569, 1},
    {0, 570, 1},
    {0, 572, 2},
    {1, 576, 576},
    {0, 580, 128},
    {0, 708, 4},
    {0, 712, 4},
    {0, 716, 4},
    {0, 720, 8},
    {0, 728, 4},
    {0, 732, 4},
    {0, 748, 2},
    {0, 740, 4},
    {0, 736, 4},
    {0, 744, 4},
    {1, 752, 752},
    {0, 756, 4},
    {0, 760, 8},
    {0, 768, 4},
    {0, 772, 4},
    {0, 776, 8},
    {0, 784, 4},
    {0, 792, 2},
    {0, 788, 4},
    {0, 794, 2},
    {0, 796, 2},
    {0, 798, 2},
    {0, 800, 1},
    {0, 804, 4},
    {0, 808, 4},
    {0, 812, 4},
    {0, 816, 8},
    {0, 824, 8},
    {0, 832, 8},
    {0, 840, 8},
    {0, 848, 8},
    {0, 856, 8},
    {0, 864, 8},
    {0, 872, 8},
    {0, 880, 2},
    {0, 882, 2},
    {0, 884, 2},
    {0, 886, 2},
    {1, 888, 888},
    {0, 892, 4},
    {0, 896, 64},
    {0, 960, 4},
    {0, 980, 4},
    {0, 964, 4},
    {3, 72, 2},
    {0, 984, 4},
    {0, 968, 4},
    {0, 972, 4},
    {0, 976, 4},
};
static const unsigned step_count = sizeof steps / sizeof steps[0];
static const unsigned char* tables[] = {norrath_game_object_vtable_view, norrath_actor_view_4488f0, norrath_actor_view_44c170};

static int failures = 0;
static unsigned char* buffer = nullptr;
static const unsigned char* source_base = nullptr;
static int requests = 0;
static bool return_null = false;
static unsigned call_index = 0;
static unsigned total_calls = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
// Called when a list-head or state call happens. The next call in the table
// must match, every copy step before it must be complete and every copy step
// after it must not have run yet.
static void on_call(int kind, unsigned dest, unsigned src) {
    unsigned seen = 0, at = 0;
    for (unsigned i = 0; i < step_count; ++i) {
        if (steps[i].kind == 1 || steps[i].kind == 2) {
            if (seen == call_index) { at = i; break; }
            ++seen;
        }
    }
    expect(steps[at].kind == kind && steps[at].a == dest && steps[at].b == src, "call matches image order");
    for (unsigned i = 0; i < step_count; ++i) {
        if (steps[i].kind != 0) continue;
        bool done = true, fresh = true;
        for (unsigned j = 0; j < steps[i].b; ++j) {
            done = done && buffer[steps[i].a + j] == source_base[steps[i].a + j];
            fresh = fresh && buffer[steps[i].a + j] == 0xa5;
        }
        if (i < at) expect(done, "copy before call is done");
        else expect(fresh || done, "copy after call");
        if (i > at) expect(fresh, "copy after call not yet done");
    }
    ++call_index;
    ++total_calls;
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    requests = requested_size;
    return return_null ? nullptr : buffer;
}
extern "C" void norrath_list_head_copy_construct(void* destination, const void* source) {
    auto* dest = static_cast<unsigned char*>(destination);
    on_call(1, static_cast<unsigned>(dest - buffer), static_cast<unsigned>(static_cast<const unsigned char*>(source) - source_base));
    dest[0] = dest[1] = dest[2] = dest[3] = 0;
}
extern "C" void __15AnimationState8RC15AnimationState8(void* destination, const void* source) {
    on_call(2, static_cast<unsigned>(static_cast<unsigned char*>(destination) - buffer),
            static_cast<unsigned>(static_cast<const unsigned char*>(source) - source_base));
}

int main() {
    const unsigned size = 992u;
    static unsigned char source[992];
    static unsigned char created[992];
    for (unsigned i = 0; i < size; ++i) source[i] = static_cast<unsigned char>(0x10 + (i * 7u) % 211u);
    std::memset(created, 0xa5, sizeof created);
    buffer = created;
    source_base = source;
    void* result = clone__3Cat(source);
    expect(result == created, "returns the allocation");
    expect(requests == 992, "allocation size");
    unsigned calls = 0;
    for (unsigned i = 0; i < step_count; ++i) if (steps[i].kind == 1 || steps[i].kind == 2) ++calls;
    expect(total_calls == calls, "every call made once");
    static bool covered[992];
    for (unsigned i = 0; i < step_count; ++i) {
        if (steps[i].kind == 0) {
            for (unsigned j = 0; j < steps[i].b; ++j) { covered[steps[i].a + j] = true; expect(created[steps[i].a + j] == source[steps[i].a + j], "copied byte"); }
        } else if (steps[i].kind == 1) {
            for (unsigned j = 0; j < 4; ++j) { covered[steps[i].a + j] = true; expect(created[steps[i].a + j] == 0, "list head word zeroed"); }
        }
    }
    covered[72] = covered[73] = covered[74] = covered[75] = true;
    expect(read_word(created + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(tables[2])), "class table at +72");
    for (unsigned i = 0; i < size; ++i) if (!covered[i]) expect(created[i] == 0xa5, "gap byte untouched");
    return_null = true;
    std::memset(created, 0xa5, sizeof created);
    total_calls = 0;
    expect(clone__3Cat(source) == nullptr, "null allocation returns null");
    expect(total_calls == 0, "no calls on null");
    for (unsigned char b : created) expect(b == 0xa5, "null path writes nothing");
    return failures ? 1 : 0;
}
