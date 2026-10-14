#include <cstring>
static unsigned char object[40];
void* scriptParticles[4];
static void* saw_definition;
static float saw_position[3];
static float saw_velocity[3];
static float saw_scale = 1.0f;
static int saw_address;
extern "C" int amx_GetAddr(void*, int address, int** physical) {
    static int cell;
    saw_address = address;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "brazier");
    return 0;
}
extern "C" unsigned char* objectFindObject__FPc(char* name) {
    return std::strcmp(name, "brazier") == 0 ? object : 0;
}
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position,
                                                         float* velocity, float scale) {
    saw_definition = definition;
    saw_position[0] = position[0];
    saw_position[1] = position[1];
    saw_position[2] = position[2];
    saw_velocity[0] = velocity[0];
    saw_velocity[1] = velocity[1];
    saw_velocity[2] = velocity[2];
    saw_scale = scale;
}
extern "C" int amxAddParticleOnObject__FP5__amxPi(void*, int*);
static void store_f32(unsigned char* address, float value) {
    union {
        float as_float;
        unsigned as_bits;
    } bits;
    bits.as_float = value;
    address[0] = static_cast<unsigned char>(bits.as_bits);
    address[1] = static_cast<unsigned char>(bits.as_bits >> 8);
    address[2] = static_cast<unsigned char>(bits.as_bits >> 16);
    address[3] = static_cast<unsigned char>(bits.as_bits >> 24);
}
int main() {
    int marker_a = 1;
    int marker_b = 2;
    scriptParticles[0] = &marker_a;
    scriptParticles[2] = &marker_b;
    store_f32(object + 16, 1.0f);
    store_f32(object + 20, 2.0f);
    store_f32(object + 24, 3.0f);
    int params[9] = {32, -1, 9, 10, 20, 30, 4, 5, 6};
    if (amxAddParticleOnObject__FP5__amxPi(0, params) != 0) return 1;
    if (saw_address != 9 || saw_definition != &marker_a || saw_scale != 0.0f) return 2;
    if (saw_position[0] != 11.0f || saw_position[1] != 22.0f || saw_position[2] != 33.0f) return 3;
    if (saw_velocity[0] != 4.0f || saw_velocity[1] != 5.0f || saw_velocity[2] != 6.0f) return 4;
    params[1] = 2;
    if (amxAddParticleOnObject__FP5__amxPi(0, params) != 0 || saw_definition != &marker_b) return 5;
    return 0;
}
