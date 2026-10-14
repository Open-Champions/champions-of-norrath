// Observed amxObjectInPlayerTrigger at 0x00138d08, 248 bytes. It finds
// the object named by the first AMX cell, then up to 50 objects of the
// class named by the first word of __tf7Trigger. A word of -1 at
// object+104 skips that object. Otherwise the signed halfword at
// vtable+136, from the pointer at object+72, adjusts the receiver and
// the function at vtable+140 is called with the named object. The
// first nonzero result returns 1.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" void* __tf7Trigger(void);
extern "C" void objectFindObjectByClass__FPcPP10GameObjecti(char* class_name, void** list, int limit);

using TriggerCall = int (*)(void* trigger, void* named);

static int word_at(const unsigned char* bytes) {
    return static_cast<int>(static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24));
}
static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}

extern "C" int amxObjectInPlayerTrigger__FP5__amxPi(void* amx, int* params) {
    char text[160];
    void* cells = nullptr;
    void* found[50];
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(text, cells);
    void* named = objectFindObject__FPc(text);
    if (named == nullptr) return 0;
    char* class_name = static_cast<char*>(load_ptr(static_cast<unsigned char*>(__tf7Trigger())));
    objectFindObjectByClass__FPcPP10GameObjecti(class_name, found, 50);
    for (void** slot = found; *slot != nullptr; ++slot) {
        unsigned char* trigger = static_cast<unsigned char*>(*slot);
        if (word_at(trigger + 104) == -1) continue;
        unsigned char* table = static_cast<unsigned char*>(load_ptr(trigger + 72));
        short adjust = 0;
        __builtin_memcpy(&adjust, table + 136, sizeof adjust);
        TriggerCall call = nullptr;
        __builtin_memcpy(&call, table + 140, sizeof call);
        if (call(trigger + adjust, named) != 0) return 1;
    }
    return 0;
}
