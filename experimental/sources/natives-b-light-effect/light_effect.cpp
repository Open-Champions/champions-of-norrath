// Observed amxLightEffect at 0x0013b220, 252 bytes. The first AMX
// cell addresses three integers, converted to floats before the
// allocation. A null 188-byte block returns zero without reading the
// later cells. Otherwise LightEffect receives that point, cells 3
// through 12 as floats, 1, cell 2 multiplied by the word 0x3c23d70a,
// a null object, and zero.
extern "C" int amx_GetAddr(void* amx, int address, void** physical);
extern "C" void* blockAlloc__Fi(int bytes);
extern "C" void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point,
    float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float one, float scaled, void* object, int flag);

extern "C" int amxLightEffect__FP5__amxPi(void* amx, int* params) {
    void* cells = nullptr;
    (void)amx_GetAddr(amx, params[1], &cells);
    const int* point_cells = static_cast<const int*>(cells);
    float point[3] = {
        static_cast<float>(point_cells[0]),
        static_cast<float>(point_cells[1]),
        static_cast<float>(point_cells[2]),
    };
    void* block = blockAlloc__Fi(188);
    if (block == nullptr) return 0;
    union Bits { unsigned word; float value; } scale;
    scale.word = 0x3c23d70au;
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(
        block, point,
        static_cast<float>(params[3]), static_cast<float>(params[4]),
        static_cast<float>(params[5]), static_cast<float>(params[6]),
        static_cast<float>(params[7]), static_cast<float>(params[8]),
        static_cast<float>(params[9]), static_cast<float>(params[10]),
        static_cast<float>(params[11]), static_cast<float>(params[12]),
        1.f, static_cast<float>(params[2]) * scale.value, nullptr, 0);
    return 0;
}
