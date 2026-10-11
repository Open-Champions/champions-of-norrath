#include <cstring>
unsigned char arena[2048];
unsigned char* norrath_native_host_base = arena;
static int marker_other;
static int marker_prop;
static int state_calls;
static unsigned char* state_self;
static int state_arg;
static void* type_other() { return &marker_other; }
static void* type_prop() { return &marker_prop; }
static void state_method(unsigned char* self, int arg) {
    ++state_calls;
    state_self = self;
    state_arg = arg;
}
void* (*norrath_native_host_method0[3])() = {0, type_other, type_prop};
void (*norrath_native_host_method2[2])(unsigned char*, int) = {0, state_method};
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "door");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** list, int limit) {
    if (limit != 100) {
        list[0] = 0;
        return;
    }
    list[0] = arena + 64;
    list[1] = arena + 128;
    list[2] = 0;
}
extern "C" void* __tf8AutoProp() { return &marker_prop; }
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right) { return left == right ? 1 : 0; }
extern "C" int amxAutoPropSetState__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    store_u32(arena + 64 + 72, 400u);
    store_u32(arena + 400 + 4, 1u);
    store_u32(arena + 128 + 72, 700u);
    store_u32(arena + 700 + 4, 2u);
    arena[700 + 144] = 6;
    arena[700 + 145] = 0;
    store_u32(arena + 700 + 148, 1u);
    int params[3] = {8, 1, 42};
    if (amxAutoPropSetState__FP5__amxPi(0, params) != 0) return 1;
    if (state_calls != 1 || state_self != arena + 134 || state_arg != 42) return 2;
    return 0;
}
