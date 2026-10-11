#include <cstring>
unsigned char arena[1024];
unsigned char* norrath_native_host_base = arena;
static int calls;
static int bad_calls;
static unsigned char* saw_self;
static int saw_arg;
static void bad_method(unsigned char*, int) { ++bad_calls; }
static void method(unsigned char* self, int arg) {
    ++calls;
    saw_self = self;
    saw_arg = arg;
}
void (*norrath_native_host_method2[4])(unsigned char*, int) = {bad_method, method, bad_method, bad_method};
static int empty;
extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int cell;
    *physical = &cell;
    return 0;
}
extern "C" int amx_GetString(char* destination, int*) {
    std::strcpy(destination, "drake");
    return 0;
}
extern "C" void objectFindObject__FPcPP10GameObjecti(char* name, void** list, int limit) {
    if (std::strcmp(name, "drake") != 0 || limit != 512 || empty) {
        list[0] = 0;
        return;
    }
    list[0] = arena + 64;
    list[1] = 0;
}
extern "C" int amxDeleteMonster__FP5__amxPi(void*, int*);
static void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    int params[2] = {4, 1};
    empty = 1;
    if (amxDeleteMonster__FP5__amxPi(0, params) != 0 || calls != 0) return 1;
    empty = 0;
    store_u32(arena + 64 + 72, 256u);
    arena[256 + 8] = 4;
    arena[256 + 9] = 0;
    store_u32(arena + 256 + 12, 1u);
    if (amxDeleteMonster__FP5__amxPi(0, params) != 0) return 2;
    if (calls != 1 || bad_calls != 0 || saw_arg != 3) return 3;
    if (saw_self != arena + 68) return 4;
    return 0;
}
