// Host comparison only. Not an EE build input. Callees are scripted; the
// endless stop loop is left by a longjmp from the CreateSema-error printf.
#include <csetjmp>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
extern "C" {
alignas(8) unsigned char GLibData[0x668];
unsigned char KbdMapTable[0x2ee];
}
static std::vector<std::string> events;
static std::jmp_buf stop;
static int sema_result;
static std::string last_format;
static int banner_args[5];
static const char* banner_text;
extern "C" int w9_printf_stub(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    last_format = f;
    if (last_format.compare(0, 8, "LgKbM li") == 0) {
        for (int i = 0; i < 3; ++i) banner_args[i] = __builtin_va_arg(ap, int);
        banner_text = __builtin_va_arg(ap, const char*);
        banner_args[3] = __builtin_va_arg(ap, int);
        events.push_back("banner");
    } else {
        events.push_back(last_format.compare(0, 9, "lgkbmlib:") == 0 ? "semaerr" : "already");
    }
    __builtin_va_end(ap);
    if (events.back() == "semaerr") std::longjmp(stop, 1);
    return 0;
}
extern "C" int CreateSema(void* p) {
    events.push_back("sema");
    int* w = static_cast<int*>(p);
    if (w[1] != 1 || w[2] != 1) events.push_back("BADSEMA");
    return sema_result;
}
static void* sorted_table; static int sort_lo, sort_hi;
extern "C" void qsort_translate_table(void* t, int lo, int hi) { events.push_back("sort"); sorted_table = t; sort_lo = lo; sort_hi = hi; }
extern "C" void connect_to_iop_server() { events.push_back("connect"); }
extern "C" void init_ee_timer() { events.push_back("timer"); }
extern "C" int get_time() { return 0; }
static int reader_priority = -99;
extern "C" void CreateReaderThread(int p) { events.push_back("reader"); reader_priority = p; }
#define printf w9_printf_stub
#include "lg_kb_m_init.cpp"
#undef printf

static int rd(unsigned off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
static int addr(const void* p) { return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p))); }
static int timer_stub() { return 5; }

int main() {
    int failures = 0;
    auto check = [&](bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } };
    // Normal path with default timer.
    sema_result = 12;
    int r = lgKbMInit(3, 9, 55, nullptr);
    check(r == 0, "returns 0");
    check(rd(64) == 3 && rd(68) == 4, "limits clamp to 4 above 4");
    check(rd(96) == 1 && rd(120) == 500 && rd(132) == 75 && rd(52) == 1 && rd(44) == 12, "plain fields");
    long long v; std::memcpy(&v, GLibData + 112, 8);
    check(v == 100, "64-bit word 100");
    check(rd(56) == 0 && rd(60) == 0, "zeroed words");
    check(rd(124) == addr(KbdMapTable) && sorted_table == KbdMapTable && sort_lo == 0 && sort_hi == 74, "table and sort");
    check(rd(128) == addr(reinterpret_cast<const void*>(get_time)), "default timer function");
    check(events == (std::vector<std::string>{"banner", "sort", "sema", "connect", "timer", "reader"}), "call order");
    check(banner_args[0] == 1 && banner_args[1] == 6 && banner_args[2] == 1 && std::string(banner_text) == "1.06.001" && banner_args[3] == 55, "banner arguments");
    check(reader_priority == 55, "priority forwarded");
    // Second call: refused, state untouched.
    events.clear();
    unsigned char before[0x668]; std::memcpy(before, GLibData, sizeof before);
    r = lgKbMInit(1, 1, 1, nullptr);
    check(r == static_cast<int>(0x80000000u) && events == std::vector<std::string>{"already"} && std::memcmp(before, GLibData, sizeof before) == 0, "second call refused untouched");
    // Fresh, supplied timer, negative / boundary limits.
    std::memset(GLibData, 0, sizeof GLibData); events.clear();
    r = lgKbMInit(-3, 5, 7, timer_stub);
    check(r == 0 && rd(64) == -3 && rd(68) == 4, "negative kept, five clamps");
    check(rd(128) == addr(reinterpret_cast<const void*>(timer_stub)), "supplied timer stored");
    check(events == (std::vector<std::string>{"banner", "sort", "sema", "connect", "reader"}), "no ee timer init when supplied");
    std::memset(GLibData, 0, sizeof GLibData); events.clear();
    lgKbMInit(4, 0, 7, timer_stub);
    check(rd(64) == 4 && rd(68) == 0, "limit 4 and 0 kept");
    // Semaphore failure: stops after the error text, flag stays clear, no connect.
    std::memset(GLibData, 0, sizeof GLibData); events.clear();
    sema_result = -1;
    if (setjmp(stop) == 0) { lgKbMInit(1, 1, 1, nullptr); check(false, "must not return"); }
    check(events == (std::vector<std::string>{"banner", "sort", "sema", "semaerr"}) && rd(52) == 0 && rd(44) == -1, "semaphore failure path");
    if (failures) return 2;
    std::printf("lgKbMInit cases passed\n");
    return 0;
}
