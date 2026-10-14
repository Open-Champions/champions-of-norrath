#include "../detail.hpp"
#include <cstdio>
#include <cstdint>
#include <cstring>
static int fails;
static void expect(bool cond, const char* message) {
    if (!cond) { std::printf("FAIL %s\n", message); ++fails; }
}
static int chest_calls;
static int alloc_calls;
static int particle_calls;
static int alloc_size;
static void* seen_object;
static char* seen_name;
static const void* chest_point;
static const void* particle_point;
static unsigned char chest_bytes[12];
static unsigned char particle_bytes[12];
static short seen_type;
static char** seen_args;
static char** particle_args;
static void* particle_object;
static unsigned char* next_block;
extern "C" unsigned norrath_construct_AutoChest(void* object, char* name, const void* point,
                                               short type, char** arguments) {
    ++chest_calls;
    seen_object = object;
    seen_name = name;
    chest_point = point;
    seen_type = type;
    seen_args = arguments;
    std::memcpy(chest_bytes, point, 12);
    auto* raw = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(point));
    raw[0] = 0xab;
    auto* obj = reinterpret_cast<unsigned char*>(object);
    obj[72] = 0x11;
    return 0xce5u;
}
extern "C" unsigned char* norrath_block_alloc(int requested_size) {
    ++alloc_calls;
    alloc_size = requested_size;
    return next_block;
}
extern "C" unsigned __11AddParticleG6Point3PPc(void* object, const void* point, char** arguments) {
    ++particle_calls;
    particle_object = object;
    particle_point = point;
    particle_args = arguments;
    std::memcpy(particle_bytes, point, 12);
    return 0xadd1u;
}
unsigned char norrath_ctors_ant_lantern_view_44ea78[4];
static unsigned word_at(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int main() {
    alignas(16) unsigned char object[256];
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    char name[] = "chest";
    char* args[] = {name, nullptr};
    alignas(16) unsigned char block[388];
    std::memset(object, 0x5a, sizeof object);
    next_block = block;
    const unsigned live = norrath_construct_AutoChestLP(object, name, point, static_cast<short>(-7), args);
    expect(live == 0xadd1u, "forwards AddParticle result");
    expect(chest_calls == 1 && alloc_calls == 1 && particle_calls == 1, "one call each");
    expect(seen_object == object && seen_name == name && seen_args == args, "chest object name and arguments");
    expect(chest_point != point && std::memcmp(chest_bytes, point, 12) == 0, "point snapshot");
    expect(seen_type == -7, "sign-extended short");
    expect(particle_object == block && particle_args == args, "particle receives the allocation and arguments");
    expect(particle_point == chest_point && particle_bytes[0] == 0xab, "particle sees the same mutated snapshot");
    expect(alloc_size == 388, "allocation size 388");
    expect(word_at(object + 72) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_ant_lantern_view_44ea78)),
           "vtable replaces the parent store");
    expect(word_at(object + 248) == static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(block)),
           "allocation stored at 248");
    next_block = nullptr;
    chest_calls = alloc_calls = particle_calls = 0;
    std::memset(object, 0x5a, sizeof object);
    const unsigned empty = norrath_construct_AutoChestLP(object, name, point, static_cast<short>(4), args);
    expect(empty == 0u && particle_calls == 0 && alloc_calls == 1, "null allocation skips AddParticle");
    expect(word_at(object + 248) == 0u, "null allocation stored");
    expect(seen_type == 4, "positive short preserved");
    if (fails != 0) return 1;
    std::puts("auto chest lp constructor passed");
    return 0;
}
