// Host contract for msg_moveCollision__10FireBeetlePP10GameObject. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_move_collision.cpp"
#include <cstdio>
#include <cstring>
#include <vector>

static int type_a, type_b;
static void* type_a_function() { return &type_a; }
static void* type_b_function() { return &type_b; }
struct Hurt { void* object; float amount; void* source; int flags; };
static std::vector<Hurt> hurts;
static std::vector<int> states;
static std::vector<void*> state_objects;
static std::vector<unsigned> rolls;
static unsigned roll_index = 0;
static int base_calls = 0, rand_calls = 0, eq_calls = 0, throws = 0;
static void *base_self, *base_list;
static void hurt_function(void* object, float amount, void* source, int flags) { hurts.push_back({object, amount, source, flags}); }
static void state_function(void* object, int state) { states.push_back(state); state_objects.push_back(object); }

extern "C" void msg_moveCollision__8CreaturePP10GameObject(void* self, void** list) { ++base_calls; base_self = self; base_list = list; }
extern "C" unsigned eRand__10GameObject(void* self) { (void)self; ++rand_calls; return rolls[roll_index++ % rolls.size()]; }
extern "C" bool __eq__C9type_infoRC9type_info(const void* l, const void* r) { ++eq_calls; return l == r; }
extern "C" void __throw_bad_typeid() { ++throws; }

struct Slots { unsigned char bytes[200]; };
static Slots table_a, table_b;
static void set_word(unsigned char* p, unsigned v) { vslots_b::write_word(vslots_b::address(p), v); }
static void make_table(Slots& t, void* (*type_fn)(), short hurt_adj) {
    std::memset(t.bytes, 0, sizeof t.bytes);
    set_word(t.bytes + 4, vslots_b::address_bits(reinterpret_cast<void*>(type_fn)));
    t.bytes[16] = static_cast<unsigned char>(hurt_adj); t.bytes[17] = static_cast<unsigned char>(hurt_adj >> 8);
    set_word(t.bytes + 20, vslots_b::address_bits(reinterpret_cast<void*>(hurt_function)));
    t.bytes[184] = 8; t.bytes[185] = 0;
    set_word(t.bytes + 188, vslots_b::address_bits(reinterpret_cast<void*>(state_function)));
}
static unsigned char objects[6][1200];
static unsigned list_words[8];
static void** const list_storage_ptr = reinterpret_cast<void**>(list_words);
static void put(int i, const void* p) { list_words[i] = p ? vslots_b::address_bits(p) : 0u; }
static void reset() {
    hurts.clear(); states.clear(); state_objects.clear(); roll_index = 0;
    base_calls = rand_calls = eq_calls = throws = 0;
}
int main() {
    using namespace vslots_b;
    make_table(table_a, type_a_function, 0);
    make_table(table_b, type_b_function, -16);
    host_note(&table_a); host_note(&table_b);
    host_note(reinterpret_cast<void*>(type_a_function)); host_note(reinterpret_cast<void*>(type_b_function));
    host_note(reinterpret_cast<void*>(hurt_function)); host_note(reinterpret_cast<void*>(state_function));
    for (auto& o : objects) { std::memset(o, 0, sizeof o); host_note(o); }
    unsigned char* self = objects[0];
    set_word(self + 72, address_bits(&table_a));
    // not state 13: base only
    for (unsigned half : {0u, 12u, 14u, 0xfff3u}) {
        reset();
        write_u16(address(self) + 572u, half);
        put(0, objects[1]); put(1, nullptr);
        set_word(objects[1] + 72, address_bits(&table_b));
        msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
        if (base_calls != 1 || base_self != self || base_list != list_storage_ptr || !hurts.empty() || rand_calls || eq_calls) return 1;
    }
    // state 13, empty list: nothing, even with +576 set
    reset(); write_u16(address(self) + 572u, 13u); set_word(self + 576, 5);
    put(0, nullptr);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    // empty list keeps "keep" true -> no setState
    if (base_calls || !hurts.empty() || !states.empty() || eq_calls) return 2;
    // same-type entries are skipped
    reset();
    set_word(objects[1] + 72, address_bits(&table_a));
    put(0, objects[1]); put(1, nullptr);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    if (eq_calls != 1 || !hurts.empty() || rand_calls || !states.empty()) return 3;
    // two other-type entries, both flagged 4: hurt both, no state change
    reset(); rolls = {3, 12};
    for (int i = 2; i <= 3; ++i) { set_word(objects[i] + 72, address_bits(&table_b)); set_word(objects[i] + 12, 4u); }
    put(0, objects[2]); put(1, objects[3]); put(2, nullptr);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    if (hurts.size() != 2 || !states.empty() || rand_calls != 2) return 4;
    if (hurts[0].object != objects[2] - 16 || hurts[0].amount != 7.0f || hurts[0].source != self || hurts[0].flags != 0x40001) return 5;
    if (hurts[1].object != objects[3] - 16 || hurts[1].amount != 8.0f) return 6;
    // one of them without bit 4: state 3 is requested, adjusted by +8
    reset(); rolls = {7};
    set_word(objects[3] + 12, 0xfffffffbu);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    if (hurts.size() != 2 || states.size() != 1 || states[0] != 3 || state_objects[0] != self + 8) return 7;
    if (hurts[0].amount != 11.0f) return 8;
    // +576 zero: no state change even without bit 4
    reset(); set_word(self + 576, 0);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    if (!states.empty() || hurts.size() != 2) return 9;
    // mixed list: same type, other type with bit 4, other type without
    reset(); set_word(self + 576, 1);
    set_word(objects[1] + 72, address_bits(&table_a));
    set_word(objects[2] + 12, 4u); set_word(objects[3] + 12, 0u);
    put(0, objects[1]); put(1, objects[2]); put(2, objects[3]); put(3, nullptr);
    msg_moveCollision__10FireBeetlePP10GameObject(self, list_storage_ptr);
    if (hurts.size() != 2 || hurts[0].object != objects[2] - 16 || hurts[1].object != objects[3] - 16 || states.size() != 1) return 10;
    return 0;
}
