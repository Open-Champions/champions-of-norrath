#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_HOST_CONTRACT 1
#include "../../experimental/sources/engine-init-startup/engine_init.cpp"

static int fail_code;
static std::vector<std::string> events;
static std::vector<unsigned> fill_addrs;
static unsigned expect_stack_bytes;
static unsigned expect_malloc;
static unsigned expect_tail;
static int thread_calls;
static int chan_calls;
static int tex_calls;
static int send_calls;
static int wait_calls;
static int printf_calls;
static int max_calls;
static int total_calls;
static int sema_calls;
static int lump_loads;
static int lump_finds;
static unsigned words[64];

static unsigned chan_a = 0x11111111u;
static unsigned chan_b = 0x22222222u;
static unsigned chan_gif = 0xffffffffu;
static unsigned chan_vif0 = 0u;
static unsigned chan_vif1 = 0x100u;
static unsigned char shadow_bytes[32];
static char* font_result = reinterpret_cast<char*>(static_cast<uintptr_t>(0x41));
static char* halo_result = reinterpret_cast<char*>(static_cast<uintptr_t>(0x42));
static void* block;
static unsigned link_end_value;

static void require(bool ok, int code) {
    if (!ok && fail_code == 0) fail_code = code;
}

static void note(const char* name) { events.emplace_back(name); }

extern "C" volatile unsigned norrath_host_memory_last;
extern "C" volatile unsigned norrath_host_stack_base;
volatile unsigned norrath_host_memory_last;
volatile unsigned norrath_host_stack_base;

extern "C" unsigned norrath_host_link_end() { return link_end_value; }
extern "C" unsigned norrath_host_cop0_count() { return 0x12345678u; }

extern "C" unsigned char norrath_host_load8(unsigned) { return 0; }

extern "C" void norrath_host_store8(unsigned address, unsigned char value) {
    require(value == 0xa5u, 30);
    fill_addrs.push_back(address);
}

extern "C" void norrath_host_store16(unsigned address, unsigned short value) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "st16:%x:%x", address, value);
    note(buf);
}

extern "C" unsigned norrath_host_load32(unsigned address) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "ld32:%x", address);
    note(buf);
    return words[address & 63u];
}

extern "C" void norrath_host_store32(unsigned address, unsigned value) {
    words[address & 63u] = value;
    char buf[64];
    std::snprintf(buf, sizeof buf, "st32:%x:%x", address, value);
    note(buf);
}

extern "C" void norrath_host_store64(unsigned address, unsigned long long value) {
    char buf[80];
    std::snprintf(buf, sizeof buf, "st64:%x:%llx", address, value);
    note(buf);
}

extern "C" void norrath_host_sync() { note("sync"); }

volatile int vblankCount;
volatile unsigned char vblankField;
int eRandState;
int vblankSemaphore;
int vblankSemaphore2;
int vblankHID;
int displayBuffer1StartAddress;
unsigned displayBuffer2StartAddress;
unsigned engineEnvTexAddress;
int engineShadowTexAddress;
unsigned textureBufferStartAddress;
unsigned engineDrawBufferStartAddress;
int engineDisplayDisable;
int memAllocSize;
void* memAllocPtr;
unsigned* dmaGif;
unsigned* dmaVif0;
unsigned* dmaVif1;
alignas(16) unsigned char dispEnv[64];
alignas(16) unsigned char ntsc_legallineTex[32];
alignas(16) unsigned char ntsc_legallinebottomTex[32];
char* defaultFont;
char* haloTexture1;
char engineCurrentLevelName[80];

extern "C" int GetThreadId() {
    note("get-thread");
    ++thread_calls;
    if (thread_calls == 1) {
        require(!fill_addrs.empty(), 31);
        const auto lo = *std::min_element(fill_addrs.begin(), fill_addrs.end());
        const auto hi = *std::max_element(fill_addrs.begin(), fill_addrs.end());
        require(fill_addrs.size() == static_cast<unsigned>(hi - lo + 1u), 32);
        std::vector<unsigned> sorted = fill_addrs;
        std::sort(sorted.begin(), sorted.end());
        for (std::size_t i = 1; i < sorted.size(); ++i) require(sorted[i] == sorted[i - 1] + 1u, 33);
        if (expect_stack_bytes == 0x40000u) {
            require(lo == 0x01fc0000u && hi == 0x01ffec00u, 34);
        } else {
            require(lo == 0x07ff0000u && hi == 0x07ffec00u, 35);
        }
        fill_addrs.clear();
    }
    return 4;
}

extern "C" void ChangeThreadPriority(int thread, int priority) {
    note("priority");
    require(thread == 4 && priority == 10, 36);
}

extern "C" int CreateSema(unsigned* param) {
    note("sema");
    require(param[1] == 0x7ffffc17u && param[2] == 0u, 37);
    return sema_calls++ == 0 ? 11 : 22;
}

extern "C" void sceGsResetGraph(int mode, int interlace, int ffmode, int version) {
    note("gs-reset");
    require(mode == 0 && interlace == 1 && ffmode == 2 && version == 1, 38);
    require(displayBuffer2StartAddress == 0x002bc000u, 39);
    require(engineEnvTexAddress == 0x00348000u, 40);
    require(engineShadowTexAddress == 0x00358000, 41);
    require(displayBuffer1StartAddress == 0x00230000, 42);
    require(textureBufferStartAddress == 0x00359000u, 43);
    require(vblankSemaphore == 11 && vblankSemaphore2 == 22, 44);
    require(engineDrawBufferStartAddress == 0u, 45);
}

extern "C" void vblankHandler__Fi(int) {}

extern "C" int AddIntcHandler(int cause, void (*handler)(int), int next) {
    note("intc");
    require(cause == 2 && handler == vblankHandler__Fi && next == 0, 46);
    return 7;
}

extern "C" void EnableIntc(int cause) {
    note("enable");
    require(cause == 2 && vblankHID == 7, 47);
}

extern "C" void WaitSema(int id) {
    note("wait");
    require(id == vblankSemaphore2 && id == 22, 48);
    ++wait_calls;
    if (wait_calls == 6) {
        require(engineCurrentLevelName[0] == 0 && engineCurrentLevelName[1] == 0x42, 49);
    }
    vblankCount += 1;
}

extern "C" unsigned* sceDmaGetChan(int channel) {
    note("get-chan");
    const int which = chan_calls++;
    const int expected_channel[] = {2, 2, 2, 0, 1};
    require(which < 5 && channel == expected_channel[which], 50);
    unsigned* ptrs[] = {&chan_a, &chan_b, &chan_gif, &chan_vif0, &chan_vif1};
    return ptrs[which];
}

static void write_header(unsigned char* texture, unsigned short qwc, unsigned packet) {
    texture[6] = static_cast<unsigned char>(qwc);
    texture[7] = static_cast<unsigned char>(qwc >> 8);
    texture[16] = static_cast<unsigned char>(packet);
    texture[17] = static_cast<unsigned char>(packet >> 8);
    texture[18] = static_cast<unsigned char>(packet >> 16);
    texture[19] = static_cast<unsigned char>(packet >> 24);
}

extern "C" void textureInitTexture__FP8_texture(unsigned char* texture) {
    note("tex");
    if (tex_calls == 0) {
        require(texture == ntsc_legallineTex, 51);
        write_header(texture, 0x21, 0x1000u);
    } else {
        require(texture == ntsc_legallinebottomTex, 52);
        write_header(texture, 0x22, 0x2000u);
    }
    ++tex_calls;
}

extern "C" void sceDmaSendN(unsigned* channel, void* packet, int qwc) {
    note("dma-send");
    const auto address = reinterpret_cast<uintptr_t>(packet);
    const int which = send_calls++;
    if (which == 0) require(channel == &chan_a && address == 0x1000u && qwc == 0x21, 53);
    if (which == 1) require(channel == &chan_b && address == 0x2000u && qwc == 0x22, 54);
    if (which == 2) require(channel == &chan_gif && address == 0x3000u && qwc == 0x23, 55);
}

extern "C" void sceGsSyncPath(int mode, int timeout) {
    note("gs-sync");
    require(mode == 0 && timeout == 0, 56);
}

extern "C" void sceGsSetDefDispEnv(unsigned char* env, int a, int b, int c, int d, int e) {
    note("set-disp");
    require(env == dispEnv && a == 0 && b == 640 && c == 224 && d == 0 && e == 0, 57);
}

extern "C" void sceGsPutDispEnv(unsigned char* env) {
    note("put-disp");
    require(env == dispEnv, 58);
}

extern "C" int norrath_host_sprintf(char* destination, const char* format, ...) {
    note("sprintf");
    require(std::strcmp(format, "a%d%f\n") == 0, 59);
    require(engineDisplayDisable == 0, 60);
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const int number = __builtin_va_arg(args, int);
    const double value = __builtin_va_arg(args, double);
    __builtin_va_end(args);
    require(number == 1, 61);
    unsigned long long bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    require(bits == 0x3fb9999980000000ull, 62);
    if (destination) destination[0] = 0;
    return 0;
}

extern "C" int norrath_host_sscanf(const char* text, const char* format, ...) {
    note("sscanf");
    require(std::strcmp(text, ".1") == 0 && std::strcmp(format, "%lf") == 0, 63);
    __builtin_va_list args;
    __builtin_va_start(args, format);
    double* out = __builtin_va_arg(args, double*);
    __builtin_va_end(args);
    require(out != nullptr, 64);
    *out = 0.0;
    return 1;
}

extern "C" double norrath_host_strtod(const char* text, char** end) {
    note("strtod");
    require(std::strcmp(text, "1.0004") == 0 && end == nullptr, 65);
    return 0.0;
}

extern "C" void* __wrap_malloc(unsigned size) {
    note("malloc");
    require(size == expect_malloc, 66);
    block = std::malloc(size);
    require(block != nullptr, 67);
    return block;
}

extern "C" void* norrath_host_memset(void* destination, int value, unsigned count) {
    note("memset");
    require(value == 0xa5 && count == 1024u, 68);
    require(memAllocPtr == block, 69);
    require(memAllocSize == static_cast<int>(expect_tail), 70);
    require(destination == static_cast<unsigned char*>(block) + expect_tail, 71);
    auto* bytes = static_cast<unsigned char*>(destination);
    for (unsigned i = 0; i < count; ++i) bytes[i] = static_cast<unsigned char>(value);
    return destination;
}

extern "C" void memInit__FPvi(void* memory, int size) {
    note("mem-init");
    require(memory == block && size == static_cast<int>(expect_tail), 72);
    require(static_cast<unsigned char*>(block)[expect_tail] == 0xa5u, 73);
    require(static_cast<unsigned char*>(block)[expect_tail + 1023u] == 0xa5u, 74);
}

extern "C" void _engineCheckStack__FPCci(const char* site, int line) {
    note("check");
    require(std::strcmp(site, "engine.cc") == 0 && line == 1075, 75);
}

extern "C" void FlushCache(int mode) {
    note("flush");
    require(mode == 0, 76);
}

extern "C" void initSif__Fv() { note("sif"); }
extern "C" void padInit__Fv() { note("pad"); }
extern "C" void cdInit__Fv() { note("cd"); }
extern "C" void sceGsResetPath() { note("gs-reset-path"); }
extern "C" void sceDmaReset(int mode) {
    note("dma-reset");
    require(mode == 1, 77);
}
extern "C" void initVIF__Fv() { note("vif"); }
extern "C" void emathInit__Fv() { note("emath"); }
extern "C" void textureAllocateCacheBuffer__Fv() { note("tex-cache"); }
extern "C" void textureInit__Fii(int address, int size) {
    note("tex-init");
    require(address == 0x00359000 && size == 0x000a7000, 78);
}
extern "C" void drawInit__Fv() { note("draw"); }
extern "C" void blockAllocInit__Fv() { note("block"); }
extern "C" void listInit__Fv() { note("list"); }
extern "C" void soundInit__Ff(float value) {
    note("sound");
    unsigned bits = 0;
    std::memcpy(&bits, &value, sizeof bits);
    require(bits == 0x3f7fffffu, 79);
}
extern "C" void scriptInit__Fv() { note("script"); }
extern "C" void animInit__Fv() { note("anim"); }
extern "C" void objectInit__Fv() { note("object"); }
extern "C" void worldInit__Fv() { note("world"); }
extern "C" void floorInit__Fv() { note("floor"); }
extern "C" void initSupersample__Fv() { note("super"); }

extern "C" void lumpLoad__FPc(char* name) {
    note("lump-load");
    const char* expected[] = {"engine.lmp", "font.lmp"};
    require(lump_loads < 2 && std::strcmp(name, expected[lump_loads]) == 0, 80);
    ++lump_loads;
}

extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    note("lump-find");
    if (lump_finds == 0) {
        require(std::strcmp(lump, "font.lmp") == 0 && std::strcmp(name, "gamefonta.fnt") == 0, 81);
        ++lump_finds;
        return font_result;
    }
    if (lump_finds == 1) {
        require(std::strcmp(lump, "engine.lmp") == 0 && std::strcmp(name, "shadowblob.tex") == 0, 82);
        write_header(shadow_bytes, 0x23, 0x3000u);
        ++lump_finds;
        return reinterpret_cast<char*>(shadow_bytes);
    }
    require(std::strcmp(lump, "engine.lmp") == 0 && std::strcmp(name, "halo1.tex") == 0, 83);
    ++lump_finds;
    return halo_result;
}

extern "C" void lumpLock__Fi(int value) {
    note("lump-lock");
    require(value == 100000 && defaultFont == font_result, 84);
}

extern "C" void P_Init__Fv() {
    note("p-init");
    require(haloTexture1 == halo_result, 85);
}
extern "C" void lightInit__Fv() { note("light"); }
extern "C" void sceDmaSync(unsigned* channel, int mode, int timeout) {
    note("dma-sync");
    require(channel == dmaGif && channel == &chan_gif && mode == 0 && timeout == 0, 86);
}

extern "C" int sceSifQueryMaxFreeMemSize() {
    note("sif-max");
    const int which = max_calls++;
    if (which == 4) return -2048;
    return (which + 1) << 10;
}

extern "C" int sceSifQueryTotalFreeMemSize() {
    note("sif-total");
    const int which = total_calls++;
    return (which + 11) << 10;
}

extern "C" void scePrintf(const char* text, ...) {
    note("printf");
    __builtin_va_list args;
    __builtin_va_start(args, text);
    if (printf_calls == 0) {
        const int size = __builtin_va_arg(args, int);
        require(std::strcmp(text, "stack size=%d\n") == 0, 87);
        require(static_cast<unsigned>(size) == expect_stack_bytes, 88);
        require(eRandState == 0x12345678, 89);
    } else {
        const unsigned left = __builtin_va_arg(args, unsigned);
        const unsigned right = __builtin_va_arg(args, unsigned);
        const char* formats[] = {
            "preCD IOP %dk %dk\n", "postCD IOP %dk %dk\n", "preSound IOP %dk %dk\n",
            "postSound IOP %dk %dk\n", "postInit IOP %dk %dk\n"};
        require(printf_calls >= 1 && printf_calls <= 5, 90);
        require(std::strcmp(text, formats[printf_calls - 1]) == 0, 91);
        const unsigned expected_left[] = {1u, 2u, 3u, 4u, 0x003ffffeu};
        const unsigned expected_right[] = {11u, 12u, 13u, 14u, 15u};
        require(left == expected_left[printf_calls - 1] && right == expected_right[printf_calls - 1], 92);
    }
    __builtin_va_end(args);
    ++printf_calls;
}

static const char* expected_log[] = {
    "st64:12000000:0", "printf", "get-thread", "priority", "sema", "sema", "gs-reset",
    "intc", "enable", "wait", "st64:12000000:0", "wait",
    "get-chan", "tex", "st16:20001014:2300", "sync", "dma-send", "gs-sync",
    "get-chan", "tex", "st16:20002014:2bc0", "sync", "dma-send", "gs-sync",
    "st64:12000090:1400", "set-disp", "put-disp", "wait", "wait", "wait",
    "st64:12000000:2", "st64:12000020:1", "st64:12000070:80000001518", "st64:12000090:1518",
    "st64:12000080:1be9ff0183227c", "st64:120000a0:1be9ff0183227c",
    "sprintf", "sscanf", "strtod", "malloc", "memset", "mem-init", "check", "flush",
    "sif", "pad", "sif-max", "sif-total", "printf", "cd", "sif-max", "sif-total", "printf",
    "gs-reset-path", "dma-reset", "get-chan", "get-chan", "flush", "get-chan", "flush",
    "st32:1000e020:100", "ld32:1000e020", "vif",
    "st32:10000010:82", "st32:10000000:0", "st32:10000810:9f", "emath",
    "tex-cache", "tex-init", "draw", "block", "list",
    "sif-max", "sif-total", "printf", "sound", "sif-max", "sif-total", "printf",
    "script", "anim", "object", "world", "floor", "super",
    "lump-load", "lump-load", "lump-find", "lump-lock", "lump-find",
    "st16:20003014:3580", "sync", "dma-send", "lump-find", "p-init", "light",
    "wait", "dma-sync", "flush", "sif-max", "sif-total", "printf"};

static void reset_case() {
    fail_code = 0;
    events.clear();
    fill_addrs.clear();
    thread_calls = chan_calls = tex_calls = send_calls = wait_calls = printf_calls = 0;
    max_calls = total_calls = sema_calls = lump_loads = lump_finds = 0;
    std::memset(words, 0, sizeof words);
    chan_a = 0x11111111u;
    chan_b = 0x22222222u;
    chan_gif = 0xffffffffu;
    chan_vif0 = 0u;
    chan_vif1 = 0x100u;
    std::memset(shadow_bytes, 0, sizeof shadow_bytes);
    std::memset(ntsc_legallineTex, 0, sizeof ntsc_legallineTex);
    std::memset(ntsc_legallinebottomTex, 0, sizeof ntsc_legallinebottomTex);
    vblankCount = 0;
    vblankField = 0x5a;
    eRandState = 0;
    engineDisplayDisable = 4;
    engineCurrentLevelName[0] = 0x41;
    engineCurrentLevelName[1] = 0x42;
    defaultFont = nullptr;
    haloTexture1 = nullptr;
    block = nullptr;
}

static int run_case() {
    union {
        unsigned bits;
        float value;
    } argument;
    argument.bits = 0x3f7fffffu;
    engineInit__Ff(argument.value);
    if (fail_code != 0) return fail_code;
    const std::size_t count = sizeof expected_log / sizeof expected_log[0];
    if (events.size() != count) {
        std::fprintf(stderr, "log %zu expected %zu\n", events.size(), count);
        for (std::size_t i = 0; i < events.size() && i < count + 8; ++i) {
            const char* want = i < count ? expected_log[i] : "-";
            std::fprintf(stderr, "%zu %s | %s\n", i, events[i].c_str(), want);
        }
        return 93;
    }
    for (std::size_t i = 0; i < count; ++i) {
        if (events[i] != expected_log[i]) {
            std::fprintf(stderr, "at %zu got %s want %s\n", i, events[i].c_str(), expected_log[i]);
            return 94;
        }
    }
    require(chan_gif == 0xffffffbfu && chan_vif0 == 0x40u && chan_vif1 == 0x140u, 95);
    require(dmaGif == &chan_gif && dmaVif0 == &chan_vif0 && dmaVif1 == &chan_vif1, 96);
    require(eRandState == 0x12345678 && engineDisplayDisable == 0, 97);
    require(engineCurrentLevelName[0] == 0 && engineCurrentLevelName[1] == 0x42, 98);
    std::free(block);
    block = nullptr;
    return fail_code;
}

int main() {
    norrath_host_memory_last = 0x01ffffffu;
    norrath_host_stack_base = 0x01fc0000u;
    link_end_value = 0x008ec91cu;
    expect_stack_bytes = 0x40000u;
    expect_malloc = 0x016c36e4u;
    expect_tail = 0x016c32e4u;
    reset_case();
    const int image = run_case();
    if (image != 0) return image;

    norrath_host_memory_last = 0u;
    norrath_host_stack_base = 0x07ff0000u;
    link_end_value = 0x059ae000u;
    expect_stack_bytes = 0x10000u;
    expect_malloc = 0x2000u;
    expect_tail = 0x1c00u;
    reset_case();
    return run_case();
}
