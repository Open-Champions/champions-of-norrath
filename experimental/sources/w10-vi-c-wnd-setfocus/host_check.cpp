// Host check only; not a build input. Build as x86_64 with static storage below 4 GiB so that
// 32-bit object words can hold real addresses (see the priv hostrun note). scePrintf records the
// format and arguments; table entries are host-sized records. The same eight state combinations
// (focus set or not, window null or not, mcErrOn zero or not, force false or true) were compared
// with the original in a private interpreter run (identical logs and changed memory).
#include "setfocus.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mFocus;
unsigned char sWndMcErr[0x210];
int mcErrOn;
static unsigned char focus_object[128], window_object[128];
static VtEntry focus_table[7], window_table[7];
static std::string trace;

static std::string who(const void* p) {
    unsigned w = word_of(p);
    if (w == 0) return "0";
    if (p == focus_object) return "F";
    if (p == window_object) return "W";
    if (p == sWndMcErr) return "E";
    return "?";
}
static std::string who(unsigned w) { return who(pointer_of(w)); }

extern "C" int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("P[") + format + "]";
    const int count = std::strstr(format, "SetFocus") ? 3 : std::strstr(format, "for Window") ? 1 : 2;
    for (int i = 0; i < count; ++i) trace += who(va_arg(args, unsigned)) + ",";
    trace += ";";
    va_end(args);
    return 0;
}
static void focus_message(void* self, void* from, int message, int a, int b) {
    trace += "focus:" + std::to_string(static_cast<unsigned char*>(self) - focus_object) + "," + who(from) + "," +
             std::to_string(message) + "," + who(static_cast<unsigned>(a)) + "," + who(static_cast<unsigned>(b)) + ";";
}
static void window_message(void* self, void* from, int message, int a, int b) {
    trace += "window:" + std::to_string(static_cast<unsigned char*>(self) - window_object) + "," + who(from) + "," +
             std::to_string(message) + "," + who(static_cast<unsigned>(a)) + "," + who(static_cast<unsigned>(b)) + ";";
}
extern "C" void OnMessage__10VIWndMcErrP5VIWndiii(void* self, void* from, int message, int a, int b) {
    trace += "mcerr:" + who(self) + "," + who(from) + "," + std::to_string(message) + "," +
             who(static_cast<unsigned>(a)) + "," + who(static_cast<unsigned>(b)) + ";";
}

static bool run(bool focus, bool window, int mc, bool force) {
    trace.clear();
    mcErrOn = mc;
    _5VIWnd$mFocus = focus ? word_of(focus_object) : 0;
    focus_table[6] = VtEntry{24, 3, focus_message};
    window_table[6] = VtEntry{-8, 3, window_message};
    unsigned t = word_of(focus_table); std::memcpy(focus_object + 32, &t, 4);
    t = word_of(window_table); std::memcpy(window_object + 32, &t, 4);
    void* win = window ? static_cast<void*>(window_object) : nullptr;
    SetFocus__5VIWndP5VIWndb(sWndMcErr, win, force);

    std::string want = "P[VIWnd :: SetFocus (this = %d, Window = %d, mFocus = %d)\n]E," + std::string(window ? "W," : "0,") + (focus ? "F," : "0,") + ";";
    unsigned expect_focus = window ? word_of(window_object) : 0;
    if (!force) {
        // only the store
    } else {
        if (focus) {
            want += "P[Calling OnMessage for mFocus = %d, Window = %d\n]F," + std::string(window ? "W," : "0,") + ";";
            want += "focus:24,F,295,F," + std::string(window ? "W" : "0") + ";";
        }
        if (!window) expect_focus = 0;
        else {
            want += "P[Calling OnMessage for Window = %d\n]W,;";
            const std::string f = focus ? "F" : "0";
            if (mc == 0) {
                want += "window:-8," + f + ",295," + f + ",W;";
            } else {
                want += "P[\tWindow = %d, sWndMcErr = %d\n]W,E,;";
                want += "mcerr:E," + f + ",295," + f + ",W;";
                expect_focus = word_of(sWndMcErr);
            }
        }
    }
    bool ok = trace == want && _5VIWnd$mFocus == expect_focus;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    return ok;
}

int main() {
    bool ok = true;
    for (int focus = 0; focus < 2; ++focus)
        for (int window = 0; window < 2; ++window)
            for (int mc = 0; mc < 2; ++mc)
                for (int force = 0; force < 2; ++force) ok = run(focus, window, mc * 5, force) && ok;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
