#include "../detail.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
unsigned char norrath_ctors_ant_lantern_view_44e9d0[4];
unsigned char norrath_ctors_ant_lantern_view_4711f0[16] = "A%d", norrath_ctors_ant_lantern_view_471208[16] = "B%d",
    norrath_ctors_ant_lantern_view_471218[16] = "C%d_%d", norrath_ctors_ant_lantern_view_471230[16] = "D%d_%d",
    norrath_ctors_ant_lantern_view_485760[16] = "E%d", norrath_ctors_ant_lantern_view_471248[16] = "F%d_%d",
    norrath_ctors_ant_lantern_view_471260[16] = "G%d_%d", norrath_ctors_ant_lantern_view_485768[16] = "special",
    norrath_ctors_ant_lantern_view_471278[16] = "other";
static unsigned char object[1300];
static unsigned char blocks[4][400];
static int alloc_calls, alloc_size;
static bool block_null[4];
static bool has[4];
static int has_calls[8];
static int has_count;
static int particle_calls; static void* particle_object[4]; static int particle_number[4]; static char** particle_args[4];
static unsigned char particle_point[4][12];
static struct { const char* key; const char* text; int value; } tags[16];
static int tag_count;
static void add_string(const char* key, const char* text) { tags[tag_count++] = {key, text, 0}; }
static void add_int(const char* key, int v) { tags[tag_count++] = {key, nullptr, v}; }
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static unsigned char arg_slot[8];
extern "C" unsigned norrath_construct_AutoProp(void*, char*, const void* point, short, char**) {
    std::memset(object + 208, 0, 4);
    std::memcpy(object + 44, "\x00\x00\x00\x00", 4);
    const unsigned slot = bits(arg_slot);
    std::memcpy(object + 44, &slot, 4);
    const_cast<unsigned char*>(static_cast<const unsigned char*>(point))[0] = 0xcc;
    return 0;
}
extern "C" int hasLightOrParticleNumber__FPPci(char**, int n) { has_calls[has_count++] = n; return has[n] ? 1 : 0; }
extern "C" unsigned char* norrath_block_alloc(int size) {
    alloc_size = size;
    const int k = alloc_calls++;
    return block_null[k] ? nullptr : blocks[k];
}
extern "C" unsigned __11AddParticleG6Point3PPci(void* o, const void* point, char** args, int n) {
    const int k = particle_calls++;
    particle_object[k] = o; particle_number[k] = n; particle_args[k] = args;
    std::memcpy(particle_point[k], point, 12);
    return 0;
}
extern "C" int norrath_factory_sprintf(char* dst, const char* fmt, ...) {
    va_list ap; va_start(ap, fmt);
    const int n = std::vsnprintf(dst, 64, fmt, ap);
    va_end(ap);
    return n;
}
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int) {
    for (int i = 0; i < tag_count; ++i) if (tags[i].text != nullptr && std::strcmp(tags[i].key, tag) == 0) return const_cast<char*>(tags[i].text);
    return nullptr;
}
extern "C" int norrath_game_object_find_tag_int(void*, char* tag, int) {
    for (int i = 0; i < tag_count; ++i) if (tags[i].text == nullptr && std::strcmp(tags[i].key, tag) == 0) return tags[i].value;
    return 0;
}
extern "C" int strticmp__FPCcT0(const char* l, const char* r) { return std::strcmp(l, r); }
static const unsigned NONE = 0xffffffffu;
static unsigned first_of(int table, int i, int j) { return word_at(object + 8 + (table ? 768 : 448) + 80 * i + 8 * j); }
static unsigned second_of(int table, int i, int j) { return word_at(object + 12 + (table ? 768 : 448) + 80 * i + 8 * j); }
static void run(const char* name) {
    std::memset(object, 0x5a, sizeof object);
    alloc_calls = particle_calls = has_count = 0;
    char* args[] = {nullptr};
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char nm[16];
    std::strcpy(nm, name);
    std::memset(object, 0x5a, sizeof object);
    expect(norrath_construct_AutoPropLP(object, nm, point, static_cast<short>(2), args) == 0u, "returns zero");
    expect(word_at(object + 72) == bits(norrath_ctors_ant_lantern_view_44e9d0), "vtable");
    expect(has_count == 4 && has_calls[0] == 0 && has_calls[1] == 1 && has_calls[2] == 2 && has_calls[3] == 3, "four particle numbers in order");
}
int main() {
    norrath_ctors_ant_lantern::host_note(arg_slot);
    for (auto& b : blocks) norrath_ctors_ant_lantern::host_note(b);
    // 1: nothing present
    std::memset(has, 0, sizeof has); std::memset(block_null, 0, sizeof block_null); tag_count = 0;
    std::memset(blocks, 0, sizeof blocks);
    run("plain");
    expect(alloc_calls == 0 && word_at(object + 408) == 0 && word_at(object + 412) == 0 && word_at(object + 416) == 0 && word_at(object + 420) == 0, "no blocks");
    expect(word_at(object + 424) == 0 && word_at(object + 436) == 0, "copies");
    for (int i = 0; i < 4; ++i) {
        expect(word_at(object + 440 + 4 * i) == 0, "tag word cleared");
        for (int j = 0; j < 10; ++j)
            expect(first_of(0, i, j) == 0 && second_of(0, i, j) == NONE && first_of(1, i, j) == 0 && second_of(1, i, j) == NONE, "default entries (0, -1)");
    }
    expect(word_at(object + 208) == 0u, "208 stays at the base value without a tag");
    // 2: particles, the second allocation fails
    has[1] = true; has[3] = true;
    block_null[1] = true;
    for (auto& b : blocks) std::memset(b, 0, sizeof b);
    run("plain");
    expect(alloc_calls == 2 && alloc_size == 388, "two allocations of 388");
    expect(word_at(object + 408) == 0 && word_at(object + 412) == bits(blocks[0]) && word_at(object + 416) == 0 && word_at(object + 420) == 0, "null block stored as zero");
    expect(particle_calls == 1 && particle_object[0] == blocks[0] && particle_number[0] == 1 && particle_args[0] == reinterpret_cast<char**>(arg_slot), "AddParticle only for the live block, with word 44");
    expect(particle_point[0][0] == 0xcc && particle_point[0][1] == 2, "same mutated snapshot");
    expect(word_at(object + 428) == bits(blocks[0]) && word_at(object + 432) == 0, "copies follow");
    // 3: tags
    std::memset(has, 0, sizeof has); std::memset(block_null, 0, sizeof block_null);
    tag_count = 0;
    add_string("A1", "x");
    add_string("B2", "1010");
    add_int("C1_3", 7);
    add_int("D1_3", 5);
    add_int("D2_4", 9);
    add_string("E0", "01");
    add_int("F3_0", 4);
    add_int("G3_0", 6);
    run("plain");
    expect(word_at(object + 208) == 1u && word_at(object + 444) == 1 && word_at(object + 440) == 0, "A tag sets 208 and word 444");
    expect(first_of(0, 2, 0) == 0 && second_of(0, 2, 0) == NONE && first_of(0, 2, 1) == NONE && second_of(0, 2, 1) == 0 &&
           first_of(0, 2, 2) == 0 && second_of(0, 2, 2) == NONE && first_of(0, 2, 3) == NONE && second_of(0, 2, 3) == 0, "string pairs");
    expect(first_of(0, 2, 4) == 0 && second_of(0, 2, 4) == 9, "D turns -1 into 0 and stores the value");
    expect(first_of(0, 2, 5) == NONE && second_of(0, 2, 5) == 0, "rest of table one keeps (-1, 0) because flag one is set");
    expect(first_of(0, 1, 3) == 7 && second_of(0, 1, 3) == 5, "C then D on one entry");
    expect(first_of(0, 0, 0) == 0 && second_of(0, 0, 0) == NONE, "untouched table falls back");
    expect(first_of(1, 0, 0) == NONE && second_of(1, 0, 0) == 0 && first_of(1, 0, 1) == 0 && second_of(1, 0, 1) == NONE && first_of(1, 0, 2) == NONE, "table two string pairs");
    expect(first_of(1, 3, 0) == 4 && second_of(1, 3, 0) == 6, "table two F and G");
    expect(first_of(1, 3, 1) == NONE && second_of(1, 3, 1) == 0, "table two flag keeps -1,0");
    expect(first_of(1, 1, 0) == 0 && second_of(1, 1, 0) == NONE, "table two fallback");
    // 4: names
    std::memset(has, 0, sizeof has); tag_count = 0;
    has[0] = has[2] = true;
    run("special");
    expect(word_at(blocks[0] + 380) == 30 && word_at(blocks[1] + 380) == 30 && word_at(blocks[2] + 380) == 0 && word_at(object + 416) == bits(blocks[1]), "30 stored in each live block");
    run("other");
    expect(word_at(blocks[0] + 380) == 30, "second name also stores 30");
    for (auto& b : blocks) std::memset(b, 0, sizeof b);
    run("plain");
    expect(word_at(blocks[0] + 380) == 0, "no 30 for other names");
    if (fails != 0) return 1;
    std::puts("auto prop lp constructor passed");
    return 0;
}
