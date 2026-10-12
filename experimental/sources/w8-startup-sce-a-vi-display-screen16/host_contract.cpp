#define NORRATH_HOST_CONTRACT 1
#define sprintf norrath_host_sprintf
#define printf norrath_host_printf
#define memset norrath_host_memset
#include "display_screen16.cpp"
#undef sprintf
#undef printf
#undef memset

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

unsigned char sCdRmode[8];
unsigned gCdFile[8];

static std::string log_text;
static void note(const std::string& s) { log_text += s; log_text += ' '; }
static char* blocks[3];
static unsigned sizes[3];
static int allocated;
static int search_calls, read_calls;
static GifPacket* packet_seen;
static unsigned mode_at_read[4];
static std::string name_seen;

extern "C" void* __wrap_memalign(unsigned a, unsigned s) {
    char* block = static_cast<char*>(std::calloc(1, s + 64));
    blocks[allocated] = block;
    sizes[allocated] = s;
    ++allocated;
    note("alloc" + std::to_string(a) + ":" + std::to_string(s));
    return block;
}
extern "C" void __wrap_free(void* p) {
    for (int i = 0; i < 3; ++i) if (blocks[i] == p) { note("free" + std::to_string(i)); return; }
    note("free?");
}
extern "C" int sceCdDiskReady(int m) { note("ready" + std::to_string(m)); return 2; }
extern "C" int sceCdSearchFile(void* file, const char* name) {
    note("search");
    if (file != gCdFile) note("BADFILE");
    name_seen = name;
    return ++search_calls < 3 ? 0 : 1;
}
extern "C" int sceCdRead(unsigned lsn, unsigned count, void* buffer, void* mode) {
    note("read:" + std::to_string(lsn) + ":" + std::to_string(count));
    if (buffer != blocks[0] || mode != sCdRmode) note("BADREAD");
    std::memcpy(mode_at_read, mode, 4);
    return ++read_calls < 2 ? 0 : 1;
}
extern "C" int sceCdSync(int m) { note("sync" + std::to_string(m)); return 0; }
extern "C" int norrath_host_sprintf(char* out, const char* format, ...) {
    va_list args; va_start(args, format);
    const int n = std::vsnprintf(out, 256, format, args);
    va_end(args);
    note("sprintf");
    return n;
}
extern "C" int norrath_host_printf(const char* format, ...) { note(std::string("print:") + format); return 0; }
extern "C" void* norrath_host_memset(void* d, int v, unsigned n) {
    note("memset:" + std::to_string(n) + ":" + std::to_string(v) + (d == blocks[2] ? ":spare" : ":BAD"));
    return std::memset(d, v, n);
}
extern "C" int sceGsSyncV(int m) { note("vsync" + std::to_string(m)); return 0; }
extern "C" int sceGsResetGraph(int a, int b, int c, int d) {
    note("reset:" + std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d));
    return 0;
}
extern "C" void sceGifPkInit(GifPacket* p, void* buffer) {
    packet_seen = p;
    p->first = 0x1111; p->second = 0x2222;
    note(buffer == blocks[1] ? "pkinit" : "pkinit?");
}
extern "C" void sceGifPkReset(GifPacket* p) { note(p == packet_seen ? "pkreset" : "pkreset?"); }
extern "C" void sceGifPkRefLoadImage(GifPacket* p, int a1, int a2, int a3, void* image, int qw, int a6, int a7, int w, int h) {
    note("load:" + std::to_string(a1) + "," + std::to_string(a2) + "," + std::to_string(a3) + "," +
         std::to_string(qw) + "," + std::to_string(a6) + "," + std::to_string(a7) + "," + std::to_string(w) + "," + std::to_string(h) +
         (image == blocks[0] && p == packet_seen ? "" : ",BAD"));
    p->second = 0x3333;
}
extern "C" void sceGifPkEnd(GifPacket*, int a, int b, int c) { note("end:" + std::to_string(a) + std::to_string(b) + std::to_string(c)); }
extern "C" void sceGifPkTerminate(GifPacket*) { note("term"); }
extern "C" void* sceDmaGetChan(int id) { note("chan" + std::to_string(id)); return reinterpret_cast<void*>(0x5000); }
extern "C" void FlushCache(int m) { note("flush" + std::to_string(m)); }
extern "C" int sceDmaSend(void* c, unsigned a) {
    char buf[48];
    std::snprintf(buf, sizeof buf, "send:%s:%x", c == reinterpret_cast<void*>(0x5000) ? "chan" : "BAD", a);
    note(buf);
    return 0;
}
extern "C" int sceGsSyncPath(int a, int b) { note("path" + std::to_string(a) + std::to_string(b)); return 0; }
extern "C" void norrath_host_store64(unsigned a, unsigned long long v) {
    char buf[64];
    std::snprintf(buf, sizeof buf, "st:%x=%llx", a, v);
    note(buf);
}

static void reset() {
    log_text.clear(); allocated = 0; search_calls = 0; read_calls = 0;
    std::memset(sCdRmode, 0xee, sizeof sCdRmode);
}

int main() {
    gCdFile[0] = 4321;
    reset();
    VIDisplayScreen16__FPCcib("LOADING", 2, true);
    if (name_seen != "\\DATA2\\LOADING2.16;1") { std::fputs(name_seen.c_str(), stderr); return 1; }
    const std::string expected =
        "alloc64:573440 alloc64:1024 alloc64:40960 ready0 sprintf "
        "search print:sceCdSearchFile FAIL\n search print:sceCdSearchFile FAIL\n search "
        "read:4321:280 print:CD Retry!\n read:4321:280 sync0 memset:40960:0:spare vsync0 "
        "reset:0120 st:12000000=7f20 pkinit pkreset load:0,10,10,35840,0,0,640,448 end:000 term chan2 flush0 vsync0 "
        "send:chan:3333 path00 free1 free0 free2 vsync0 "
        "st:12000000=7f23 st:12000090=51400 st:120000a0=1bf9ff0183227c st:12000070=51400 "
        "st:12000080=1bf9ff0183327d st:120000d0=0 vsync0 ";
    if (log_text != expected) { std::fprintf(stderr, "got:\n%s\nwant:\n%s\n", log_text.c_str(), expected.c_str()); return 2; }
    if (sCdRmode[0] != 0 || sCdRmode[1] != 1 || sCdRmode[2] != 0 || sCdRmode[3] != 0) return 3;
    if (reinterpret_cast<unsigned char*>(mode_at_read)[1] != 1) return 4;
    // Without the flag, no reset and no first register store.
    reset();
    VIDisplayScreen16__FPCcib("A", 0, false);
    if (log_text.find("reset:") != std::string::npos || log_text.find("st:12000000=7f20") != std::string::npos) return 5;
    if (log_text.find("st:12000000=7f23") == std::string::npos) return 6;
    std::puts("vi display screen16 contract passed");
    return 0;
}
