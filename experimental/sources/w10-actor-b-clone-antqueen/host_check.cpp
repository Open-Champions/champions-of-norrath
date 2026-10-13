// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
const unsigned char norrath_game_object_vtable_view[8] = {};
const unsigned char norrath_actor_view_4488f0[8] = {};
const unsigned char norrath_ctors_ant_lantern_view_44c440[8] = {};
}
alignas(8) static unsigned char source[1100];
alignas(8) static unsigned char created[1100];
static int allocation_succeeds;
static int allocated_size;
static std::string calls;
static unsigned vptr_at_call[8];
static int call_count;
extern "C" {
void* blockAlloc__Fi(int bytes) { allocated_size = bytes; return allocation_succeeds ? created : nullptr; }
void norrath_list_head_copy_construct(void* destination, const void* source_) {
    const unsigned dst = static_cast<unsigned>(static_cast<unsigned char*>(destination) - created);
    const unsigned src = static_cast<unsigned>(static_cast<const unsigned char*>(source_) - source);
    calls += "L" + std::to_string(dst) + (dst == src ? ";" : "!;");
    std::memcpy(&vptr_at_call[call_count++], created + 72, 4);
    std::memset(destination, 0xaa, 4);
}
void animation_state_copy(void* destination, const void* source_) asm("__15AnimationState8RC15AnimationState8");
void animation_state_copy(void* destination, const void* source_) {
    const unsigned dst = static_cast<unsigned>(static_cast<unsigned char*>(destination) - created);
    const unsigned src = static_cast<unsigned>(static_cast<const unsigned char*>(source_) - source);
    calls += "A" + std::to_string(dst) + (dst == src ? ";" : "!;");
    std::memcpy(&vptr_at_call[call_count++], created + 72, 4);
    std::memset(destination, 0xab, 1);
}
}
#include "clone.cpp"

// Byte ranges of the new object that receive a copy of the source (the callee-filled
// heads and the AnimationState at +288 are not in this list).
static const unsigned ranges[][2] = {
    {12, 42},
    {44, 58},
    {60, 72},
    {76, 77},
    {80, 86},
    {88, 157},
    {160, 174},
    {176, 190},
    {192, 206},
    {208, 234},
    {236, 286},
    {552, 571},
    {572, 574},
    {580, 750},
    {756, 801},
    {804, 888},
    {892, 961},
    {964, 969},
    {972, 990}
};
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

int main() {
    for (unsigned i = 0; i < sizeof source; ++i) source[i] = static_cast<unsigned char>(i * 7 + 1);
    // A failed allocation returns null.
    allocation_succeeds = 0; calls.clear();
    std::memset(created, 0xcc, sizeof created);
    expect(clone__8AntQueen(source) == nullptr && calls.empty() && allocated_size == 992, "null allocation");
    bool untouched = true;
    for (unsigned i = 0; i < sizeof created; ++i) if (created[i] != 0xcc) untouched = false;
    expect(untouched, "nothing written after a failed allocation");
    allocation_succeeds = 1; calls.clear(); call_count = 0;
    std::memset(created, 0xcc, sizeof created);
    void* result = clone__8AntQueen(source);
    expect(result == created, "returns the new object");
    expect(calls == "L0;L4;L8;A288;L576;L752;L888;", "constructor calls in order, each with matching offsets");
    unsigned expected[1100];
    for (unsigned i = 0; i < sizeof created; ++i) expected[i] = 0xcc;
    for (const auto& r : ranges) for (unsigned i = r[0]; i < r[1]; ++i) expected[i] = source[i];
    for (unsigned o : {0u, 4u, 8u, 576u, 752u, 888u}) for (unsigned i = 0; i < 4; ++i) expected[o + i] = 0xaa;
    expected[288] = 0xab;
    unsigned vtable;
    std::memcpy(&vtable, created + 72, 4);
    expect(vtable == rtti_c::address_bits(norrath_ctors_ant_lantern_view_44c440), "final vtable");
    for (unsigned i = 0; i < sizeof created; ++i) {
        if (i >= 72 && i < 76) continue;
        if (created[i] != expected[i]) { std::fprintf(stderr, "fail: byte %u is %02x, expected %02x\n", i, created[i], expected[i]); ++failures; break; }
    }
    expect(vptr_at_call[0] == rtti_c::address_bits(norrath_game_object_vtable_view) && vptr_at_call[1] == vptr_at_call[0] && vptr_at_call[2] == vptr_at_call[0], "GameObject vtable during the first heads");
    const unsigned creature = rtti_c::address_bits(norrath_actor_view_4488f0);
    expect(vptr_at_call[3] == creature && vptr_at_call[4] == creature && vptr_at_call[5] == creature && vptr_at_call[6] == creature, "Creature vtable for the later constructors");
    return failures ? 1 : 0;
}
