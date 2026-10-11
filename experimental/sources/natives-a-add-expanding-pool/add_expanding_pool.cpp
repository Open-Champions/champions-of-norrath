// amxAddExpandingPoolUnder is the 124-byte native at 0x0013b400. Cell 1 names
// one object and cell 3 is an integer converted to float. Twelve bytes at
// object+16 are copied and passed to floorAdd with 0, that float, 10, and 2.
// The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
extern "C" void floorAdd__FG6Point3sfii(float* point, int zero, float scale, int ten, int two);
extern "C" int amxAddExpandingPoolUnder__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    float point[3];
    amx_GetAddr(amx, params[1], &cell);
    amx_GetString(name, cell);
    unsigned char* object = objectFindObject__FPc(name);
    for (int index = 0; index < 12; ++index)
        reinterpret_cast<unsigned char*>(point)[index] = object[16 + index];
    floorAdd__FG6Point3sfii(point, 0, static_cast<float>(params[3]), 10, 2);
    return 0;
}
