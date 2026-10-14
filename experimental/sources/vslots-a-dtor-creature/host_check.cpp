#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_VSLOTS_A_HOST
#include "dtor.cpp"

unsigned long long norrath_vslots_a_host_base;
extern "C" const unsigned char norrath_actor_view_4488f0[8] = {};

static std::vector<std::string> calls;
static int failures = 0;
static unsigned char* expected_iterator = nullptr;
static unsigned char memory[4096];

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void host_write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned host_read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<uintptr_t>(pointer) - norrath_vslots_a_host_base);
}

static unsigned char* nodes[2];
static int node_index = 0;

extern "C" void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask) {
    calls.push_back("construct");
    expected_iterator = iterator;
    expect(head == memory + 888, "list head is creature+888");
    expect(mask == 0, "iterator mask is zero");
    expect(host_read_word(memory + 72) == word_of(norrath_actor_view_4488f0), "vtable stored before the walk");
    node_index = 0;
    host_write_word(iterator + 4, word_of(nodes[0]));
}
extern "C" void norrath_list_safe_iterator_next(unsigned char* iterator) {
    calls.push_back("next");
    expect(iterator == expected_iterator, "next sees the same iterator");
    ++node_index;
    host_write_word(iterator + 4, node_index < 2 ? word_of(nodes[node_index]) : 0u);
}
extern "C" void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags) {
    calls.push_back("destroy");
    expect(iterator == expected_iterator, "destroy sees the same iterator");
    expect(flags == 2, "iterator flag is 2");
}
extern "C" void norrath_list_destroy(void* head) {
    auto* bytes = static_cast<unsigned char*>(head);
    if (bytes == memory + 888) calls.push_back("list-888");
    else if (bytes == memory + 752) calls.push_back("list-752");
    else if (bytes == memory + 576) calls.push_back("list-576");
    else calls.push_back("list-other");
}
extern "C" void fx_creature_state_destructor(void* state, int flags) asm("_$_15FxCreatureState");
extern "C" void fx_creature_state_destructor(void* state, int flags) {
    calls.push_back("fx");
    expect(state == memory + 824, "fx state is creature+824");
    expect(flags == 2, "fx flag is 2");
}
extern "C" void animation_state_destructor(void* state, int flags) asm("_$_14AnimationState");
extern "C" void animation_state_destructor(void* state, int flags) {
    calls.push_back("anim");
    expect(state == memory + 288, "animation state is creature+288");
    expect(flags == 0, "animation flag is 0");
}
extern "C" void norrath_vt_dtor_10GameObject(void* object, int flags) {
    calls.push_back("gameobject");
    expect(object == memory, "game object is the creature");
    expect(flags == 0, "game object flag is 0");
}
static int frees = 0;
extern "C" void norrath_block_free(void* object) {
    calls.push_back("free");
    expect(object == memory, "frees the creature");
    ++frees;
}
static void slot_call(void* child, int argument) {
    calls.push_back("slot");
    expect(child == memory + 1200, "slot receives the child");
    expect(argument == 3, "slot argument is 3");
}

int main() {
    norrath_vslots_a_host_base = reinterpret_cast<uintptr_t>(memory);
    std::memset(memory, 0x3c, sizeof memory);
    nodes[0] = memory + 1000;
    nodes[1] = memory + 1100;
    unsigned char* child = memory + 1200;
    unsigned char* vtable = memory + 1400;
    host_write_word(nodes[0] + 12, word_of(child));
    host_write_word(nodes[1] + 12, 0u);
    host_write_word(child + 20, word_of(vtable));
    host_write_word(vtable + 8, 0u);
    host_write_word(vtable + 12, word_of(reinterpret_cast<void*>(slot_call)));
    const unsigned round = word_of(reinterpret_cast<void*>(slot_call));
    auto* recovered = reinterpret_cast<void (*)(void*, int)>(
        norrath_vslots_a_host_base + static_cast<long long>(static_cast<int>(round)));
    expect(recovered == slot_call, "slot pointer round-trips through 32 bits");

    creature_destructor(memory, 0);
    expect(frees == 0, "flag 0 does not free");
    const char* wanted[] = {
        "construct", "slot", "next", "next", "destroy",
        "list-888", "fx", "list-752", "list-576", "anim", "gameobject"
    };
    expect(calls.size() == 11, "eleven calls without free");
    for (int i = 0; i < 11 && i < static_cast<int>(calls.size()); ++i)
        expect(calls[static_cast<unsigned>(i)] == wanted[i], wanted[i]);

    calls.clear();
    creature_destructor(memory, 2);
    expect(frees == 0, "flag 2 does not free");
    calls.clear();
    creature_destructor(memory, 3);
    expect(frees == 1, "flag bit 0 frees");
    expect(!calls.empty() && calls.back() == "free", "free is last");
    return failures ? 1 : 0;
}
