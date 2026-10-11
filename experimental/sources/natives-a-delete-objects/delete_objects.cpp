// amxDeleteObjects is the 172-byte native at 0x0013c230. It matches
// amxDeleteMonster except the object list limit is 25.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
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
static void call_entry(const unsigned char* entry, unsigned char* object, int argument) {
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
extern "C" int amxDeleteObjects__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    void* list[25];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    objectFindObject__FPcPP10GameObjecti(name, list, 25);
    if (list[0] == 0) return 0;
    int index = 0;
    for (;;) {
        unsigned char* object = static_cast<unsigned char*>(list[index]);
        call_entry(pointer_at(object + 72) + 8, object, 3);
        index += 1;
        if (list[index] == 0) break;
    }
    return 0;
}
