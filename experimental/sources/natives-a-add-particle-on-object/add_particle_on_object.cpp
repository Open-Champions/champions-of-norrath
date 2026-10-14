// amxAddParticleOnObject is the 292-byte native at 0x0013a350. Cell 1 selects
// a word of scriptParticles at 0x00333920. Cell 1 equal to -1 uses the first
// word. Cell 2 names an object. Cells 3 through 5 are an integer position
// added to the three floats at object+16. Cells 6 through 8 are an integer
// velocity. P_AddParticle receives the definition, those two points, and 0.
// The native returns zero.
extern "C" int amx_GetAddr(void* amx, int address, int** physical);
extern "C" int amx_GetString(char* destination, int* source);
extern "C" unsigned char* objectFindObject__FPc(char* name);
extern "C" void* scriptParticles[];
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position,
                                                         float* velocity, float scale);
static unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}
static float load_f32(const unsigned char* address) {
    union {
        unsigned as_bits;
        float as_float;
    } value;
    value.as_bits = load_u32(address);
    return value.as_float;
}
extern "C" int amxAddParticleOnObject__FP5__amxPi(void* amx, int* params) {
    int* cell = 0;
    char name[160];
    float position[3];
    float velocity[3];
    position[0] = static_cast<float>(params[3]);
    position[1] = static_cast<float>(params[4]);
    position[2] = static_cast<float>(params[5]);
    velocity[0] = static_cast<float>(params[6]);
    velocity[1] = static_cast<float>(params[7]);
    velocity[2] = static_cast<float>(params[8]);
    amx_GetAddr(amx, params[2], &cell);
    amx_GetString(name, cell);
    unsigned char* object = objectFindObject__FPc(name);
    position[0] += load_f32(object + 16);
    position[1] += load_f32(object + 20);
    position[2] += load_f32(object + 24);
    int index = params[1];
    void* definition = index == -1 ? scriptParticles[0] : scriptParticles[index];
    P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, 0.0f);
    return 0;
}
