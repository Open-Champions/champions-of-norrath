// amxAutoPropSetState is the 248-byte native at 0x00138980. Cell 1 names
// objects, up to 100. The word at object byte 72 addresses a table. The
// function word at byte 4 of that table is called and compared, through
// __eq__ and __tf8AutoProp, with AutoProp. On a match, the entry at byte 144
// supplies a signed 16-bit this-adjustment and a function word called with
// cell 2. The native returns zero. The first call does not reload its first
// argument; that leftover is not reconstructed.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
extern "C" void* __tf8AutoProp();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static unsigned char* pointer_at(const unsigned char* address) {
    unsigned word = load_u32(address);
#if defined(NORRATH_HOST_CONTRACT)
    if (word == 0u) return 0;
    extern unsigned char* norrath_native_host_base;
    return norrath_native_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
static void* call_type(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    extern void* (*norrath_native_host_method0[])();
    return norrath_native_host_method0[word]();
#else
    return reinterpret_cast<void* (*)()>(static_cast<__UINTPTR_TYPE__>(word))();
#endif
}
static void call_state(const unsigned char* entry, unsigned char* object, int argument) {
    int adjust = static_cast<short>(static_cast<unsigned>(entry[0]) | (static_cast<unsigned>(entry[1]) << 8));
    unsigned word = load_u32(entry + 4);
    unsigned char* self = object + adjust;
#if defined(NORRATH_HOST_CONTRACT)
    extern void (*norrath_native_host_method2[])(unsigned char*, int);
    norrath_native_host_method2[word](self, argument);
#else
    reinterpret_cast<void (*)(unsigned char*, int)>(static_cast<__UINTPTR_TYPE__>(word))(self, argument);
#endif
}
extern "C" int amxAutoPropSetState__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    void* list[100];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    objectFindObject__FPcPP10GameObjecti(name, list, 100);
    if (list[0] == 0) return 0;
    int index = 0;
    for (;;) {
        unsigned char* object = static_cast<unsigned char*>(list[index]);
        unsigned char* table = pointer_at(object + 72);
        void* info = call_type(load_u32(table + 4));
        if (__eq__C9type_infoRC9type_info(info, __tf8AutoProp()) != 0)
            call_state(table + 144, object, params[2]);
        index += 1;
        if (list[index] == 0) break;
    }
    return 0;
}
