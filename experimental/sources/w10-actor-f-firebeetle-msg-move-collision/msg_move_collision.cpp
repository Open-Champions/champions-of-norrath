#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_moveCollision__10FireBeetlePP10GameObject at 0x00211618, 372 bytes.
// When the signed halfword at object+572 is not 13, the work is Creature's msg_moveCollision on the same
// two arguments (tail position, nothing else runs). Otherwise, for each non-null entry of the list, in order:
//  1. The run-time type of the entry is read (the function at word +4 of the table that the entry's word
//     +72 points to, called with no arguments) and compared, through __eq__C9type_infoRC9type_info, with the
//     same read on this object (a null this calls __throw_bad_typeid).
//  2. When the types differ: a "keep" flag, initially 1, becomes 0 unless bit 2 (4) of the entry's word at
//     +12 is set; then eRand__10GameObject(this) gives r, and the entry's virtual slot at table byte 16
//     (Creature's msg_hurt in the vtables seen) is called on the entry (adjusted by the slot's signed
//     halfword) with (float)((r & 7) + 4), this, and 0x40001.
// After the list, when the word at object+576 is non-zero and the keep flag is 0, this object's virtual
// slot at table byte 184 (setState in the vtables seen) is called, adjusted, with the value 3.
// Unknown: what the 13 state, the +576 word, the 0x40001 value and bit 2 of the flags mean.
extern "C" {
void msg_moveCollision__8CreaturePP10GameObject(void* self, void** list);
unsigned eRand__10GameObject(void* self);
bool __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
}

namespace {
typedef void (*HurtFunction)(void* object, float amount, void* source, int flags);
typedef void (*StateFunction)(void* object, int state);
typedef void* (*TypeFunction)();

void* run_time_type(void* object) {
    using namespace vslots_b;
    if (object == nullptr) __throw_bad_typeid();
    const Address table = address(pointer(read_word(address(object) + 72u)));
    TypeFunction function = reinterpret_cast<TypeFunction>(pointer(read_word(table + 4u)));
    return function();
}
}  // namespace

extern "C" void msg_moveCollision__10FireBeetlePP10GameObject(void* self, void** list) {
    using namespace vslots_b;
    if (read_i16(address(self) + 572u) != 13) {
        msg_moveCollision__8CreaturePP10GameObject(self, list);
        return;
    }
    bool keep = true;
    for (unsigned index = 0; read_word(address(list) + index * 4u) != 0u; ++index) {
        void* other = pointer(read_word(address(list) + index * 4u));
        void* other_type = run_time_type(other);
        void* own_type = run_time_type(self);
        if (!__eq__C9type_infoRC9type_info(other_type, own_type)) {
            if ((read_word(address(other) + 12u) & 4u) == 0u) keep = false;
            const unsigned roll = eRand__10GameObject(self);
            void* target = pointer(read_word(address(list) + index * 4u));
            void* adjusted;
            HurtFunction hurt = slot_function<HurtFunction>(target, 16u, &adjusted);
            hurt(adjusted, static_cast<float>(static_cast<int>((roll & 7u) + 4u)), self, 0x40001);
        }
    }
    if (read_word(address(self) + 576u) != 0u && !keep) {
        void* adjusted;
        StateFunction state = slot_function<StateFunction>(self, 184u, &adjusted);
        state(adjusted, 3);
    }
}
