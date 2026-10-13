// Host check only; not a build input. Every callee records its arguments in a trace, scePrintf
// records its format text and (for the header line) its four values. Each message path is compared
// with a written-out trace and the expected object words.
#include "onmessage.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static unsigned char self_object[10000], other[16], mcerr_window[16];
static std::string trace;
static std::string off(const void* w) {
    return std::to_string(static_cast<const unsigned char*>(w) - self_object);
}
static std::string who(const void* p) {
    return p == self_object ? "T" : p == other ? "O" : p == mcerr_window ? "E" : p == nullptr ? "0" : "?";
}

extern "C" {
int scePrintf(const char* format, ...) {
    trace += std::string("[") + format + "]";
    if (std::strncmp(format, "VIWndSplash::OnMessage(", 23) == 0) {
        va_list args;
        va_start(args, format);
        for (int i = 0; i < 4; ++i) trace += std::to_string(va_arg(args, unsigned)) + ",";
        va_end(args);
    }
    return 0;
}
void OnMessage__5VIWndP5VIWndiii(void* self, void* from, int message, void* a, void* b) {
    trace += "base:" + off(self) + "," + who(from) + "," + std::to_string(message) + "," + who(a) + "," + who(b) + ";";
}
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    trace += "focus:" + off(self) + "," + who(window) + "," + std::to_string(force) + ";";
}
void AddChild__5VIWndP5VIWnd(void* self, void* child) { trace += "add:" + off(self) + "," + who(child) + ";"; }
void* McErrWnd__Fv() { trace += "mcerrwnd;"; return mcerr_window; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::to_string(ui) + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
}

static bool g_ok = true;
static void check(int line, bool good) {
    if (!good) { std::printf("failed at line %d (trace %s)\n", line, trace.c_str()); g_ok = false; }
}
static unsigned word_of_obj(const void* p) { return word_of(p); }
static int word(int offset) { int v; std::memcpy(&v, self_object + offset, 4); return v; }
static void set(int offset, int v) { std::memcpy(self_object + offset, &v, 4); }
static void fresh() { std::memset(self_object, 0x66, sizeof self_object); trace.clear(); }
static std::string head(void* from, int message, void* a, void* b) {
    return "[VIWndSplash::OnMessage(From = %d, MEssageId = %d, Param1 = %d, Param2 = %d)\n]" +
           std::to_string(word_of_obj(from)) + "," + std::to_string(message) + "," + std::to_string(word_of_obj(a)) + "," +
           std::to_string(word_of_obj(b)) + ",";
}

int main() {
    bool ok = true; (void)ok;
    // 295 with b != this: header and label only.
    fresh();
    OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 295, other, other);
    check(__LINE__, trace == head(other, 295, other, other) + "[VIWndSplash::OnMessage CHANGE_FOCUS\n]");
    // 295 with a in the exclusion set: nothing after the label.
    for (int skip : {416, 2408, 4776}) {
        fresh();
        OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 295, self_object + skip, self_object);
        check(__LINE__, trace == head(other, 295, self_object + skip, self_object) + "[VIWndSplash::OnMessage CHANGE_FOCUS\n]" &&
             word(2376) == 0x66666666);
    }
    // 295 full path, with and without the latch at +2376, a equal or not to the McErr window.
    for (int latch : {0, 7}) {
        for (void* a : {static_cast<void*>(other), static_cast<void*>(mcerr_window)}) {
            fresh();
            set(2376, latch);
            OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 295, a, self_object);
            const std::string want = head(other, 295, a, self_object) + "[VIWndSplash::OnMessage CHANGE_FOCUS\n]" +
                                     "init:40,4660,0,190,320,224,64,1,0;open:40/1;mcerrwnd;";
            check(__LINE__, trace == want);
            const bool cleared = latch != 0;
            check(__LINE__, word(2376) == 0 && word(9640) == (cleared ? 0 : 0x66666666) &&
                 word(9648) == (cleared ? 0 : 0x66666666) && word(9652) == (cleared ? 0 : 0x66666666) &&
                 word(9656) == (cleared ? 0 : 0x66666666));
            const bool zero_9660 = cleared || a != mcerr_window;
            check(__LINE__, word(9660) == (zero_9660 ? 0 : 0x66666666));
        }
    }
    // 307.
    fresh();
    OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 307, other, other);
    check(__LINE__, trace == head(other, 307, other, other) + "[VIWndSplash::OnMessage MESSAGE_CLOSE\n]focus:0,T,1;" &&
         word(9648) == 0 && word(9652) == 0 && word(9644) == 0x66666666);
    // 4352, 4354, 4356.
    fresh();
    OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 4352, other, other);
    check(__LINE__, trace == head(other, 4352, other, other) + "[VIWndSplash::OnMessage MCERR_ADD_CHILD\n]add:0,O;");
    fresh();
    OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 4354, other, other);
    check(__LINE__, trace == head(other, 4354, other, other) + "[VIWndSplash::OnMessage MCERR_REM_CHILD\n]" &&
         static_cast<unsigned>(word(9644)) == word_of_obj(other));
    fresh();
    OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 4356, other, other);
    check(__LINE__, trace == head(other, 4356, other, other) + "[VIWndSplash::OnMessage MCERR_SET_FOCUS\n]focus:0,O,1;");
    // 4358: state 1 with a == 1, state 1 with a != 1, other states.
    const std::string done = "[VIWndSplash::OnMessage MCERR_DONE\n]focus:0,T,1;";
    struct Row { int state; unsigned a; int want_state; int want_count; };
    const Row rows[] = {{1, 1, 2, 5}, {1, 0, 0, 0}, {1, 2, 0, 0}, {0, 1, 0, 6}, {2, 1, 2, 6}, {3, 0, 3, 6}};
    for (const Row& r : rows) {
        fresh();
        set(9660, r.state);
        set(9640, 5);
        void* a = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(r.a));
        OnMessage__11VIWndSplashP5VIWndiii(self_object, other, 4358, a, other);
        check(__LINE__, trace == head(other, 4358, a, other) + done && word(9660) == r.want_state && word(9640) == r.want_count);
    }
    // Everything else: DEFAULT and the base handler.
    for (int message : {0, 294, 296, 4096, 4351, 4353, 4355, 4357, 4359}) {
        fresh();
        OnMessage__11VIWndSplashP5VIWndiii(self_object, other, message, other, self_object);
        check(__LINE__, trace == head(other, message, other, self_object) + "[VIWndSplash::OnMessage DEFAULT\n]base:0,O," +
                                std::to_string(message) + ",O,T;");
    }
    std::puts(g_ok ? "ok" : "FAIL");
    return !g_ok;
}
