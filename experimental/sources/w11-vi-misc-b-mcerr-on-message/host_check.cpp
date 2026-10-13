// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. scePrintf records
// the format and its integer arguments. Covers message 0, 294 and 296 (default path) and message
// 295 with all four combinations of (param1 == this, param2 == this).
#include "on_message.cpp"
#include <cstdarg>
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static unsigned char self_object[128], other[16];
static std::string trace;

extern "C" {
int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("[") + format + "]";
    if (std::string(format).find("%d") != std::string::npos)
        for (int i = 0; i < 4; ++i) trace += std::to_string(va_arg(args, unsigned)) + ",";
    va_end(args);
    return 0;
}
void Close__10VIUIWindowb(void* w, bool f) {
    trace += "close:" + std::to_string(static_cast<unsigned char*>(w) - self_object) + "/" + std::to_string(f) + ";";
}
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + std::to_string(static_cast<unsigned char*>(w) - self_object) + "," + std::to_string(ui) + "," +
             std::to_string(p[0]) + "," + std::to_string(p[1]) + "," + std::to_string(s[0]) + "," + std::to_string(s[1]) +
             "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) {
    trace += "open:" + std::to_string(static_cast<unsigned char*>(w) - self_object) + "/" + std::to_string(f) + ";";
}
}

static std::string head(void* from, int message, void* p1, void* p2) {
    return "[VIWndMcErr::OnMessage (From = %d, MessageId = %d, Param1 = %d, Param2 = %d)\n]" +
           std::to_string(word_of(from)) + "," + std::to_string(message) + "," + std::to_string(word_of(p1)) + "," +
           std::to_string(word_of(p2)) + ",";
}

int main() {
    bool ok = true;
    for (int message : {0, 294, 296}) {
        trace.clear();
        OnMessage__10VIWndMcErrP5VIWndiii(self_object, other, message, other + 4, self_object);
        ok = ok && trace == head(other, message, other + 4, self_object) + "[VIWndMcErr::OnMessage Default\n]";
    }
    for (int mask = 0; mask < 4; ++mask) {
        void* p1 = (mask & 1) ? static_cast<void*>(self_object) : static_cast<void*>(other + 4);
        void* p2 = (mask & 2) ? static_cast<void*>(self_object) : static_cast<void*>(other + 8);
        trace.clear();
        OnMessage__10VIWndMcErrP5VIWndiii(self_object, other, 295, p1, p2);
        std::string want = head(other, 295, p1, p2) + "[VIWndMcErr::OnMessage VIWND_CHANGEFOCUS\n]";
        if (mask & 2)
            want += "[VIWndMcErr::OnMessage VIWND_CHANGEFOCUS Gained new focus\n]close:40/1;init:40,4660,0,0,320,246,380,7,0;open:40/0;";
        if (mask & 1) want += "[VIWndMcErr::OnMessage VIWND_CHANGEFOCUS Lost focus\n]";
        if (trace != want) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
        ok = ok && trace == want;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
