// Observed amxShowCompass__FP5__amxPi at 0x0013a7d0, 176 bytes. Cells 1 and
// 2 are AMX strings. The first names an object. Twelve bytes at object
// byte 16 are copied, and hudShowCompass receives that point with the
// textGetString result of the second string. The native returns 0.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* dest, int* source);
extern "C" void* objectFindObject__FPc(char* name);
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void hudShowCompass__FG6Point3PUs(const float* point, unsigned short* text);

extern "C" int amxShowCompass__FP5__amxPi(void* amx, int* params) {
    char name[160];
    char key[160];
    int* physical = nullptr;
    amx_GetAddr(amx, params[1], &physical);
    amx_GetString(name, physical);
    amx_GetAddr(amx, params[2], &physical);
    amx_GetString(key, physical);
    auto* object = static_cast<unsigned char*>(objectFindObject__FPc(name));
    float point[3];
    auto* out = reinterpret_cast<unsigned char*>(point);
    for (int i = 0; i < 12; ++i) out[i] = object[16 + i];
    hudShowCompass__FG6Point3PUs(point, textGetString__FPc(key));
    return 0;
}
