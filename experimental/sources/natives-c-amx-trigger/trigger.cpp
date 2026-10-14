// Observed amxTrigger__FP5__amxPi at 0x0013a6e8, 176 bytes. Cell 1 is an
// AMX string and cell 2 is the argument. Up to 100 objects are collected.
// A null first object returns 0. Each later object is invoked through the
// virtual slot at vtable byte 24: signed halfword adjust, function four
// bytes later, this = object + adjust, second argument = cell 2. The
// unadjusted object was still in the third argument register and is not
// passed here. The native returns 0. amx_GetAddr's stack slot is zeroed
// first; the original slot was left uninitialized when that call failed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** objects, int count);

namespace {
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

using SlotCall = void (*)(void* self, int argument);

SlotCall load_slot(const unsigned char* bytes) {
    SlotCall value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

void call_trigger(unsigned char* object, int argument) {
    auto* vtable = static_cast<unsigned char*>(load_pointer(object + 72));
    const short adjust = load_half(vtable + 24);
    load_slot(vtable + 28)(object + adjust, argument);
}
}

extern "C" int amxTrigger__FP5__amxPi(void* amx, int* params) {
    char name[160];
    void* objects[101];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    objects[0] = nullptr;
    objectFindObject__FPcPP10GameObjecti(name, objects, 100);
    for (int index = 0; objects[index] != nullptr; ++index)
        call_trigger(static_cast<unsigned char*>(objects[index]), params[2]);
    return 0;
}
