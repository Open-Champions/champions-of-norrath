#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void dtor_9WorldPart(void* object, int flags) asm("_$_9WorldPart");
extern "C" unsigned char norrath_actors_a_view_44d0a8[8] = {};
extern "C" unsigned char norrath_actors_b_view_4844a0[4] = {};

static int failures = 0;
static int sequence = 0;
static int set_order = 0, release_order = 0, base_order = 0, free_order = 0;
static int set_calls = 0, release_calls = 0, base_calls = 0, free_calls = 0;
static void* set_world = nullptr;
static void* release_world = nullptr;
static int set_index = 0, release_index = 0, base_flags = 99;
static float set_height = 0;
static void* base_object = nullptr;
static void* free_object = nullptr;
static unsigned vtable_seen_by_first_call = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void put_word(unsigned char* p, unsigned v) {
    p[0] = static_cast<unsigned char>(v); p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16); p[3] = static_cast<unsigned char>(v >> 24);
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}
static unsigned char* g_object = nullptr;

extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void* world, int index, float height) {
    ++set_calls; set_order = ++sequence;
    set_world = world; set_index = index; set_height = height;
    vtable_seen_by_first_call = read_word(g_object + 72);
    return 0;
}
extern "C" int worldReleaseDoor__FP12_worldHeaderi(void* world, int index) {
    ++release_calls; release_order = ++sequence;
    release_world = world; release_index = index;
    // The world word is read again for the second call.
    put_word(norrath_actors_b_view_4844a0, 0xCAFE0000u);
    return 0;
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    ++base_calls; base_order = ++sequence;
    base_object = object; base_flags = flags;
}
extern "C" void norrath_block_free(void* object) {
    ++free_calls; free_order = ++sequence;
    free_object = object;
}
static void reset() {
    sequence = 0;
    set_order = release_order = base_order = free_order = 0;
    set_calls = release_calls = base_calls = free_calls = 0;
    set_world = release_world = nullptr; base_object = free_object = nullptr;
    set_index = release_index = 0; base_flags = 99; set_height = 0; vtable_seen_by_first_call = 0;
}

static void run_case(unsigned short index, unsigned short height_bits, int flags, int expect_free) {
    unsigned char object[96];
    std::memset(object, 0x33, sizeof object);
    g_object = object;
    object[76] = static_cast<unsigned char>(index); object[77] = static_cast<unsigned char>(index >> 8);
    object[78] = static_cast<unsigned char>(height_bits); object[79] = static_cast<unsigned char>(height_bits >> 8);
    put_word(norrath_actors_b_view_4844a0, 0x1234500u);
    reset();
    dtor_9WorldPart(object, flags);
    const int sindex = static_cast<short>(index);
    const float sheight = static_cast<float>(static_cast<short>(height_bits));
    expect(set_calls == 1 && release_calls == 1 && base_calls == 1, "each callee runs once");
    expect(set_order < release_order && release_order < base_order, "set height, release door, base destructor in that order");
    expect(set_index == sindex && release_index == sindex, "the signed door index from +76 goes to both calls");
    expect(set_height == sheight, "the signed halfword at +78 becomes the single height");
    expect(pointer_word(set_world) == 0x1234500u, "first call receives the world word");
    expect(pointer_word(release_world) == 0x1234500u && read_word(norrath_actors_b_view_4844a0) == 0xCAFE0000u,
           "the second call reads the world word before the release call changes it");
    expect(vtable_seen_by_first_call == pointer_word(norrath_actors_a_view_44d0a8), "vtable is stored before the first call");
    expect(base_object == object && base_flags == 0, "base destructor gets the object and flag zero");
    expect((free_calls == 1) == (expect_free != 0), "free follows bit 0 of the incoming flag");
    if (expect_free) expect(free_object == object && free_order > base_order, "free comes last on the same object");
    unsigned char expected[96];
    std::memset(expected, 0x33, sizeof expected);
    expected[76] = object[76]; expected[77] = object[77]; expected[78] = object[78]; expected[79] = object[79];
    put_word(expected + 72, pointer_word(norrath_actors_a_view_44d0a8));
    expect(std::memcmp(object, expected, sizeof object) == 0, "only +72 is written");
}

int main() {
    run_case(5, 100, 0, 0);
    run_case(5, 100, 1, 1);
    run_case(0xFFFF, 0xFFFE, 2, 0);        // -1 and -2
    run_case(0x8000, 0x7FFF, 3, 1);        // -32768 and 32767
    run_case(7, 0, 4, 0);
    run_case(9, 0x8000, -1, 1);
    return failures == 0 ? 0 : 1;
}
