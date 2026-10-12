// Observed cleanUpFunc2__FP10GameObject at 0x0017e208, 356 bytes (file-local in the original). It is an
// objectIterateGrid callback used by cleanUp__Fv. If the object word at byte 12 has bit 0x40 set it returns.
// The type query is the function pointer at vtable byte 4 (vtable pointer: word at object +72), called with the
// object pointer still in the first argument register, and a null object calls __throw_bad_typeid first.
//   Gold case (query result equals __tf4Gold through __eq__C9type_infoRC9type_info): the virtual slot at
//   vtable byte 48 is called with (object + adjustment, w), where the adjustment is the halfword at +48, the
//   function pointer the word at +52, and w the word at byte 21872 of `player`; then the function returns.
//   Other types: the slot at vtable byte 16 is called with (object + adjustment, w, 64, 300.0f); then, when
//   the object word at byte 12 has bit 0x20 set, the slot at byte 32 is called with (object + adjustment) only;
//   then the type query is repeated and, if it equals __tf9Container, the slot at byte 32 is called again the
//   same way; finally objectIterateGrid is called with killExpendableFunc__FP10GameObject.
// Inferred: the 64 and 300.0 arguments are copied from the instructions.
// Unknown: what any of the three slots do, what the word in `player` is, and the meaning of bits 0x20 and 0x40.
extern "C" {
extern unsigned char player[];
void* __tf4Gold();
void* __tf9Container();
int __eq__C9type_infoRC9type_info(void* left, void* right);
void __throw_bad_typeid();
void objectIterateGrid__FPFP10GameObject_v(void (*visit)(unsigned char*));
void killExpendableFunc__FP10GameObject(unsigned char* object);
}

namespace {
unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

short load_half(const unsigned char* bytes) {
    return static_cast<short>(static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8));
}

unsigned char* load_pointer(const unsigned char* bytes) {
    unsigned char* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

using TypeCall = void* (*)();
using GoldSlot = void (*)(void* self, int word);
using WideSlot = void (*)(void* self, int word, int sixtyFour, float threeHundred);
using PlainSlot = void (*)(void* self);

template <typename Fn>
Fn load_function(const unsigned char* bytes) {
    Fn value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

void* query_type(unsigned char* object) {
    if (object == nullptr) {
        __throw_bad_typeid();
        return nullptr;
    }
    return load_function<TypeCall>(load_pointer(object + 72) + 4)();
}
}

extern "C" void cleanUpFunc2__FP10GameObject(unsigned char* object) {
    if (load_word(object + 12) & 0x40u) return;
    if (__eq__C9type_infoRC9type_info(query_type(object), __tf4Gold())) {
        const int word = static_cast<int>(load_word(player + 21872));
        const unsigned char* vtable = load_pointer(object + 72);
        load_function<GoldSlot>(vtable + 52)(object + load_half(vtable + 48), word);
        return;
    }
    const int word = static_cast<int>(load_word(player + 21872));
    const unsigned char* vtable = load_pointer(object + 72);
    load_function<WideSlot>(vtable + 20)(object + load_half(vtable + 16), word, 64, 300.0f);
    if (load_word(object + 12) & 0x20u) {
        vtable = load_pointer(object + 72);
        load_function<PlainSlot>(vtable + 36)(object + load_half(vtable + 32));
    }
    if (__eq__C9type_infoRC9type_info(query_type(object), __tf9Container())) {
        vtable = load_pointer(object + 72);
        load_function<PlainSlot>(vtable + 36)(object + load_half(vtable + 32));
    }
    objectIterateGrid__FPFP10GameObject_v(killExpendableFunc__FP10GameObject);
}
