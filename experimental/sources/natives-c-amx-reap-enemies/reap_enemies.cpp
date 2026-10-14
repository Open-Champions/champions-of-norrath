// Observed amxReapEnemies__FP5__amxPi at 0x00139928, 424 bytes. The box is
// every point from the .lit4 word 0xc76a6000 to 0x476a6000, collecting up
// to 512 objects. A null first object returns 0. An object whose word at
// byte 12 has bits 0x28 set skips the type test. Otherwise the function
// pointer at vtable byte 4 is called with no argument assignment; the
// leftover first argument is not represented. Its result is compared with
// PhProp, then Prop, then ItemProp, calling that pointer again for each
// comparison. A null object calls __throw_bad_typeid. A match, or the 0x28
// bits, reloads the flags. Bit 6 skips the object. Otherwise the virtual
// slot at vtable byte 8 is called with this-adjust and second argument 3.
// The native returns 0.
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* minimum, const float* maximum, void** objects, int count);
extern "C" void* __tfP6PhProp();
extern "C" void* __tfP4Prop();
extern "C" void* __tfP8ItemProp();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
extern "C" void __throw_bad_typeid();

namespace {
float bits_to_float(unsigned bits) {
    float value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = static_cast<unsigned char>(bits);
    out[1] = static_cast<unsigned char>(bits >> 8);
    out[2] = static_cast<unsigned char>(bits >> 16);
    out[3] = static_cast<unsigned char>(bits >> 24);
    return value;
}

int load_int(const unsigned char* bytes) {
    int value;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    out[0] = bytes[0];
    out[1] = bytes[1];
    out[2] = bytes[2];
    out[3] = bytes[3];
    return value;
}

void* load_pointer(const unsigned char* bytes) {
    void* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

short load_half(const unsigned char* bytes) {
    const unsigned value = bytes[0] | (static_cast<unsigned>(bytes[1]) << 8);
    return static_cast<short>(value);
}

using TypeCall = void* (*)();
using ReapCall = void (*)(void* self, int kind);

TypeCall load_type(const unsigned char* bytes) {
    TypeCall value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

ReapCall load_reap(const unsigned char* bytes) {
    ReapCall value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

void* query_type(unsigned char* object) {
    if (object == nullptr) {
        __throw_bad_typeid();
        return nullptr;
    }
    auto* vtable = static_cast<unsigned char*>(load_pointer(object + 72));
    return load_type(vtable + 4)();
}

bool acceptable(unsigned char* object) {
    if (__eq__C9type_infoRC9type_info(query_type(object), __tfP6PhProp())) return true;
    if (__eq__C9type_infoRC9type_info(query_type(object), __tfP4Prop())) return true;
    return __eq__C9type_infoRC9type_info(query_type(object), __tfP8ItemProp()) != 0;
}

void reap(unsigned char* object) {
    auto* vtable = static_cast<unsigned char*>(load_pointer(object + 72));
    const short adjust = load_half(vtable + 8);
    load_reap(vtable + 12)(object + adjust, 3);
}
}

extern "C" int amxReapEnemies__FP5__amxPi(void*, int*) {
    const float minimum[3] = {bits_to_float(0xc76a6000u), bits_to_float(0xc76a6000u), bits_to_float(0xc76a6000u)};
    const float maximum[3] = {bits_to_float(0x476a6000u), bits_to_float(0x476a6000u), bits_to_float(0x476a6000u)};
    void* objects[513];
    objects[0] = nullptr;
    objectFindInBox__FG6Point3T0PP10GameObjecti(minimum, maximum, objects, 512);
    void** cursor = objects;
    if (*cursor == nullptr) return 0;
    while (*cursor != nullptr) {
        auto* object = static_cast<unsigned char*>(*cursor);
        const int flags = load_int(object + 12);
        const bool selected = (flags & 0x28) == 0x28 || acceptable(object);
        if (selected) {
            const int current = load_int(object + 12);
            if ((current & 0x40) == 0 && object != nullptr) reap(object);
        }
        cursor += 1;
    }
    return 0;
}
