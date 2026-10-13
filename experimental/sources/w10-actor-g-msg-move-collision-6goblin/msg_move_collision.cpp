// Observed msg_moveCollision__6GoblinPP10GameObject at 0x0022f240, 268 bytes.
// When the signed halfword at +572 is 8 and bit 5 (0x20) of the halfword at +294 is clear: a
// one-shot animation is added to the member at +288 (header = word at (table word +36) + 172, both
// float arguments the constant with bits 0x3dcccccc, flags 32); then every object in the
// null-terminated array is tested, and for each whose type node (class table word +4, a
// no-argument function) equals __tf9Container(), the table entry at (its word +72) + 16 is called
// with that object adjusted by the entry's signed halfword, this object, the int 1 and the float
// 1.0. Nothing is returned and the Creature routine is not called.
// Inferred: the +16 entry is a Container interaction. Unknown: what it does. __tf9Container is an
// unresolved import. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

extern "C" {
void* __tf9Container();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
}

extern "C" void msg_moveCollision__6GoblinPP10GameObject(void* self, void** others) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<short*>(object + 572) != 8) return;
    if ((*reinterpret_cast<unsigned short*>(object + 294) & 0x20u) != 0) return;
    unsigned char* table = w10g::from_word(w10g::word_at(self, 36));
    void* header = w10g::from_word(*reinterpret_cast<unsigned*>(table + 172));
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(object + 288, header, 0x1.999998p-4f, 0x1.999998p-4f, 32);
    for (int i = 0; others[i] != nullptr; ++i) {
        const void* node = w10g::type_node_of(others[i]);
        if (__eq__C9type_infoRC9type_info(node, __tf9Container())) {
            void* other = others[i];
            w10g::fn_from_word<void (*)(void*, void*, int, float)>(w10g::entry_function(other, 16))(
                w10g::adjusted(other, 16), self, 1, 1.0f);
        }
    }
}
