#define NORRATH_VSLOTS_B_HOST 1
#include "run_spells.cpp"
#include <cstdio>
#include <cstring>
static int constructs = 0;
static int nexts = 0;
static int destroys = 0;
static int calls = 0;
static int seen_mask = -1;
static int seen_flags = -1;
static int seen_half = 0;
static void* seen_element = nullptr;
static unsigned char seen_point[12];
static unsigned char* active_iterator = nullptr;
static unsigned sequence[2];
static int sequence_index = 0;
extern "C" void norrath_list_safe_iterator_construct(unsigned char* iterator, unsigned char* head, int mask) {
    ++constructs;
    seen_mask = mask;
    active_iterator = iterator;
    (void)head;
    vslots_b::write_word(vslots_b::address(iterator) + 4u, sequence[sequence_index]);
}
extern "C" void norrath_list_safe_iterator_next(unsigned char* iterator) {
    ++nexts;
    ++sequence_index;
    vslots_b::write_word(vslots_b::address(iterator) + 4u, sequence[sequence_index]);
}
extern "C" void norrath_list_safe_iterator_destroy(unsigned char* iterator, int flags) {
    ++destroys;
    seen_flags = flags;
    if (iterator != active_iterator) destroys = -10;
}
extern "C" void spell_stub(void* element, void* point, int half) {
    ++calls;
    seen_element = element;
    seen_half = half;
    std::memcpy(seen_point, point, 12);
    static_cast<unsigned char*>(point)[0] = 0xff;
}
int main() {
    unsigned char object[900];
    unsigned char node[16];
    unsigned char element[32];
    unsigned char table[24];
    unsigned char point[12];
    std::memset(object, 0, sizeof object);
    std::memset(node, 0, sizeof node);
    std::memset(element, 0, sizeof element);
    std::memset(table, 0, sizeof table);
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(i + 1);
    vslots_b::write_word(vslots_b::address(object) + 888u, 0u);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 9u);
    sequence[0] = 0u;
    sequence[1] = 0u;
    sequence_index = 0;
    runSpells__8CreatureG6Point3(object, point);
    if (constructs != 1 || calls != 0 || destroys != 1 || seen_mask != 0 || seen_flags != 2) return 1;
    if (point[0] != 1) return 1;

    vslots_b::host_note(node);
    vslots_b::host_note(element);
    vslots_b::host_note(table);
    vslots_b::host_note(reinterpret_cast<void*>(spell_stub));
    vslots_b::write_word(vslots_b::address(object) + 888u, 1u);
    vslots_b::write_u16(vslots_b::address(table) + 16u, 4u);
    vslots_b::write_word(vslots_b::address(table) + 20u, vslots_b::address_bits(reinterpret_cast<void*>(spell_stub)));
    vslots_b::write_word(vslots_b::address(element) + 20u, vslots_b::address_bits(table));
    vslots_b::write_word(vslots_b::address(node) + 12u, vslots_b::address_bits(element));
    sequence[0] = vslots_b::address_bits(node);
    sequence[1] = 0u;
    sequence_index = 0;
    calls = 0;
    nexts = 0;
    runSpells__8CreatureG6Point3(object, point);
    if (calls != 1 || nexts != 1 || seen_element != element + 4 || seen_half != 9) return 1;
    if (seen_point[0] != 1 || seen_point[11] != 12 || point[0] != 1) return 1;
    return 0;
}
