// Observed amxParticleMorph at 0x0013a250, 256 bytes. Each of the two
// AMX cells fills a 512-pointer list. Only element 0 is considered,
// and only when the word at object+12 has bit 0x20 set. Both selected
// objects allocate 92 bytes and, when that block exists, construct
// ParticleMorphEffect. The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" int amx_GetString(char* destination, void* cells);
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit);
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __19ParticleMorphEffectP8CreatureT1(void* block, void* first, void* second);

static int word_at(const unsigned char* bytes) {
    return static_cast<int>(static_cast<unsigned>(bytes[0])
        | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16)
        | (static_cast<unsigned>(bytes[3]) << 24));
}
static void* selected(void** list) {
    unsigned char* object = static_cast<unsigned char*>(list[0]);
    if (object == nullptr || (word_at(object + 12) & 0x20) == 0) return nullptr;
    return object;
}

extern "C" int amxParticleMorph__FP5__amxPi(void* amx, int* params) {
    char first_name[160];
    char second_name[160];
    void* cells = nullptr;
    void* first_list[512];
    void* second_list[512];
    (void)amx_GetAddr(amx, params[1], &cells);
    (void)amx_GetString(first_name, cells);
    objectFindObject__FPcPP10GameObjecti(first_name, first_list, 512);
    (void)amx_GetAddr(amx, params[2], &cells);
    (void)amx_GetString(second_name, cells);
    objectFindObject__FPcPP10GameObjecti(second_name, second_list, 512);
    void* first = selected(first_list);
    void* second = selected(second_list);
    if (first != nullptr && second != nullptr) {
        void* block = blockAlloc__Fi(92);
        if (block != nullptr) __19ParticleMorphEffectP8CreatureT1(block, first, second);
    }
    return 0;
}
