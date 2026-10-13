// Host contract for processParams__7VehiclePPc; not an EE input. 32-bit words are offsets from a host base, strstr
// and sscanf are renamed to host versions (the game's scan set "_-." is written as a plain set), and the callees log.
// The authored function was also compared with the original in a private R5900 run (13 parameter lists: every rule,
// repeated and substring matches, and an allocation failure; all words reached except the unreachable
// alignment padding).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#define W12_SPELL_C_HOST 1
#define strstr host_strstr
#define sscanf host_sscanf
unsigned char* w12_spell_c_host_base;
unsigned w12_spell_c_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
#include "process_params.cpp"
#undef strstr
#undef sscanf

static std::string calls;
static unsigned char arena[0x4000] __attribute__((aligned(16)));
#define blocks_at(k) (arena + 0x2000 + 0x200 * (k))
static int blockIndex;
extern "C" {
char* host_strstr(const char* h, const char* n) { return const_cast<char*>(std::strstr(h, n)); }
int host_sscanf(const char* text, const char* format, ...) {
    // the three formats used here, parsed with the host's sscanf; only the pointers matter to the caller
    __builtin_va_list ap; __builtin_va_start(ap, format);
    int n = 0;
    if (std::strncmp(format, "seatoffset", 10) == 0) {
        int* a = __builtin_va_arg(ap, int*); float* x = __builtin_va_arg(ap, float*); float* y = __builtin_va_arg(ap, float*); float* z = __builtin_va_arg(ap, float*);
        n = std::sscanf(text, "seatoffset%d=%f,%f,%f", a, x, y, z);
    } else if (std::strncmp(format, "turret", 6) == 0) {
        int* a = __builtin_va_arg(ap, int*); char* name = __builtin_va_arg(ap, char*);
        float* x = __builtin_va_arg(ap, float*); float* y = __builtin_va_arg(ap, float*); float* z = __builtin_va_arg(ap, float*);
        n = std::sscanf(text, "turret%d=\"%63[a-zA-Z0-9_.-]\",%f,%f,%f", a, name, x, y, z);
    } else {
        int* a = __builtin_va_arg(ap, int*);
        float* f[6]; for (auto& p : f) p = __builtin_va_arg(ap, float*);
        n = std::sscanf(text, "stack%d=%f,%f,%f,%f,%f,%f", a, f[0], f[1], f[2], f[3], f[4], f[5]);
    }
    __builtin_va_end(ap);
    return n;
}
void* __builtin_new(unsigned size) { calls += "new(" + std::to_string(size) + ");"; std::memset(blocks_at(blockIndex), 0, 512); return blocks_at(blockIndex++); }
void* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("lump(") + std::to_string(lump - reinterpret_cast<char*>(arena)) + "," + name + ");"; return arena + 0x1800 + std::strlen(name); }
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask) { calls += "chunks(" + std::to_string(static_cast<unsigned char*>(model) - arena) + "," + std::to_string(mask) + ");"; return 0x1122334455667788ull; }
}

static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

int main() {
    int bad = 0;
    unsigned char* vehicle = arena + 0x100;
    char text1[] = "seatoffset2=1.5,2.5,-3.25";
    char text2[] = "turret1=\"gun_a.vif\",10.5,-20,30";
    char text3[] = "disable";
    char text4[] = "stack0=1,2,3,4,5,6";
    char text5[] = "stack1=7,8,9,10,11,12";
    char text6[] = "ignored";
    char* list[] = {text6, text1, text2, text3, text4, text5, nullptr};
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    blockIndex = 0;
    int n0 = 1; std::memcpy(vehicle + 2168, &n0, 4);   // stack count starts at 1
    processParams__7VehiclePPc(vehicle, list);
    if (getf(vehicle + 1932 + 24) != 1.5f || getf(vehicle + 1936 + 24) != 2.5f || getf(vehicle + 1940 + 24) != -3.25f) { std::puts("fail: seat offset"); bad = 1; }
    unsigned char* turret = blocks_at(0);
    if (calls != "new(304);lump(" + std::to_string(vehicle + 76 - arena) + ",gun_a.vif);lump(" + std::to_string(vehicle + 76 - arena) + ",gun_a.tex);chunks(" +
                     std::to_string(get32(turret + 0)) + ",0);") { std::puts(calls.c_str()); std::puts("fail: turret calls"); bad = 1; }
    if (turret[13] != 8 || getf(turret + 288) != 10.5f || getf(turret + 292) != -20.0f || getf(turret + 296) != 30.0f) { std::puts("fail: turret fields"); bad = 1; }
    if (get32(vehicle + 2088 + 4) != static_cast<unsigned>(turret - arena)) { std::puts("fail: turret slot"); bad = 1; }
    if (get32(turret + 0) != 0x1800 + 9 || get32(turret + 4) != 0x1800 + 9) { std::puts("fail: turret lumps"); bad = 1; }
    if (get32(vehicle + 1024) != 1) { std::puts("fail: disable"); bad = 1; }
    // the two stack rows go to counts 1 and 2 (row base 2172 + 24 * n); the count ends at 3
    if (getf(vehicle + 2172 + 24) != 1.0f || getf(vehicle + 2192 + 24) != 6.0f || getf(vehicle + 2172 + 48) != 7.0f || getf(vehicle + 2192 + 48) != 12.0f || get32(vehicle + 2168) != 3) {
        std::puts("fail: stack rows"); bad = 1;
    }
    // an empty list does nothing
    char* empty[] = {nullptr};
    calls.clear();
    processParams__7VehiclePPc(vehicle, empty);
    if (!calls.empty()) { std::puts("fail: empty list"); bad = 1; }
    return bad;
}
