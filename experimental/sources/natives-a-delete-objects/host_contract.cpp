#include <cstring>
unsigned char arena[1024];
unsigned char* norrath_native_host_base = arena;
static int calls;
static unsigned char* selves[2];
static void method(unsigned char* self, int arg) {
    if (arg == 3 && calls < 2) selves[calls] = self;
    ++calls;
}
void (*norrath_native_host_method2[2])(unsigned char*, int) = {0, method};
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "barrel");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char*, void** list, int limit) {
    if (limit != 25) {
        list[0] = 0;
        return;
    }
    list[0] = arena + 32;
    list[1] = arena + 128;
    list[2] = 0;
}
extern "C" int amxDeleteObjects__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static void plant(unsigned char* object, unsigned vtable, int adjust) {
    store_u32(object + 72, vtable);
    arena[vtable + 8] = static_cast<unsigned char>(adjust);
    arena[vtable + 9] = static_cast<unsigned char>(static_cast<unsigned>(adjust) >> 8);
    store_u32(arena + vtable + 12, 1u);
}
int main() {
    plant(arena + 32, 400u, 0);
    plant(arena + 128, 480u, 8);
    int params[2] = {4, 1};
    if (amxDeleteObjects__FP5__amxPi(0, params) != 0 || calls != 2) return 1;
    if (selves[0] != arena + 32 || selves[1] != arena + 136) return 2;
    return 0;
}
