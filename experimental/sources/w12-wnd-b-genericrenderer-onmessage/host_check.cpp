// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the 32-bit pointer words
// stored in the window translate to host objects through a small tag table. Every callee records
// its arguments; each message path and each forwarding choice is compared with a written-out trace.
#define NORRATH_HOST_CONTRACT 1
#include "onmessage.cpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

int mcerror, mcerring;
static unsigned char self_object[2048], other[16], recv_object[256], dialog_object[256];
static std::string trace;
static int dialog_open;
static VtEntry recv_table[8], dialog_table[8];
static std::string who(const void* p) {
    return p == self_object ? "T" : p == other ? "O" : p == recv_object ? "R" : p == dialog_object ? "D" : p == nullptr ? "0" : "?";
}
static const unsigned TAG_RECV = 0x1000, TAG_DIALOG = 0x2000, TAG_RECV_TABLE = 0x5000, TAG_DIALOG_TABLE = 0x6000;

extern "C" {
char* norrath_host_translate(unsigned address) {
    switch (address) {
    case TAG_RECV: return reinterpret_cast<char*>(recv_object);
    case TAG_DIALOG: return reinterpret_cast<char*>(dialog_object);
    case TAG_RECV_TABLE: return reinterpret_cast<char*>(recv_table);
    case TAG_DIALOG_TABLE: return reinterpret_cast<char*>(dialog_table);
    default: return nullptr;
    }
}
int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += std::string("[") + format + "]";
    if (std::strchr(format, '%')) trace += std::to_string(va_arg(args, unsigned)) + ";";
    va_end(args);
    return 0;
}
void OnMessage__5VIWndP5VIWndiii(void*, void* from, int message, void* a, void* b) {
    trace += "base:" + who(from) + "," + std::to_string(message) + "," + who(a) + "," + who(b) + ";";
}
void SetFocus__5VIWndP5VIWndb(void*, void* window, bool force) { trace += "focus:" + who(window) + "," + std::to_string(force) + ";"; }
void AddChild__5VIWndP5VIWnd(void*, void* child) { trace += "add:" + who(child) + ";"; }
int IsOpen__11VIGUIDialog(void* d) { trace += "isopen:" + who(d) + ";"; return dialog_open; }
}

static void recv_entry(void* self, void* from, int message, void* a, void* b) {
    trace += "recv:" + std::to_string(static_cast<unsigned char*>(self) - recv_object) + "," + who(from) + "," +
             std::to_string(message) + "," + who(a) + "," + who(b) + ";";
}
static void dialog_entry(void* self, void* from, int message, void* a, void* b) {
    trace += "dialog:" + std::to_string(static_cast<unsigned char*>(self) - dialog_object) + "," + who(from) + "," +
             std::to_string(message) + "," + who(a) + "," + who(b) + ";";
}
static int word(int offset) { int v; std::memcpy(&v, self_object + offset, 4); return v; }
static void set(int offset, unsigned v) { std::memcpy(self_object + offset, &v, 4); }
static bool g_ok = true;
static void check(int line, bool good) {
    if (!good) { std::printf("failed at line %d (trace %s)\n", line, trace.c_str()); g_ok = false; }
}
static unsigned word_obj(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }

int main() {
    recv_table[4] = VtEntry{24, 4, recv_entry};
    dialog_table[4] = VtEntry{-8, 4, dialog_entry};
    std::memcpy(recv_object + 120, &TAG_RECV_TABLE, 4);
    std::memcpy(dialog_object + 120, &TAG_DIALOG_TABLE, 4);
    const unsigned other_word = word_obj(other);
    struct Msg { int message; void* a; std::string head; };
    const Msg msgs[] = {
        {4352, other, "[!!Entered OnMessage, id == %i\n]4352;add:O;"},
        {307, other, "[!!Entered OnMessage, id == %i\n]307;focus:T,1;"},
        {4356, other, "[!!Entered OnMessage, id == %i\n]4356;focus:O,1;"},
        {4354, other, "[!!Entered OnMessage, id == %i\n]4354;[!!!Remove Child.....\n][!!!removing child, param = %i\n]" + std::to_string(other_word) + ";"},
        {4358, other, "[!!Entered OnMessage, id == %i\n]4358;[DONE!  code = %i\n]" + std::to_string(other_word) + ";focus:T,1;"},
        {4358, reinterpret_cast<void*>(3), "[!!Entered OnMessage, id == %i\n]4358;[DONE!  code = %i\n]3;focus:T,1;"},
        {308, other, "[!!Entered OnMessage, id == %i\n]308;"},
        {0, other, "[!!Entered OnMessage, id == %i\n]0;[!!Yo, its the default!\n]base:O,0,O,T;"},
        {4353, other, "[!!Entered OnMessage, id == %i\n]4353;[!!Yo, its the default!\n]base:O,4353,O,T;"},
        {4355, other, "[!!Entered OnMessage, id == %i\n]4355;[!!Yo, its the default!\n]base:O,4355,O,T;"},
        {4357, other, "[!!Entered OnMessage, id == %i\n]4357;[!!Yo, its the default!\n]base:O,4357,O,T;"},
        {4351, other, "[!!Entered OnMessage, id == %i\n]4351;[!!Yo, its the default!\n]base:O,4351,O,T;"},
    };
    for (const Msg& m : msgs) {
        for (unsigned p52 : {0u, TAG_RECV}) {
            for (unsigned p56 : {0u, TAG_DIALOG}) {
                for (int open : {0, 1}) {
                    if (p56 == 0 && open) continue;
                    std::memset(self_object, 0x66, sizeof self_object);
                    set(52, p52);
                    set(56, p56);
                    set(1408, 41);
                    mcerror = 0;
                    mcerring = 7;
                    dialog_open = open;
                    trace.clear();
                    OnMessage__20VIWndGenericRendererP5VIWndiii(self_object, other, m.message, m.a, self_object);
                    std::string want = m.head;
                                if (p52 != 0) {
                        const std::string args = "," + std::to_string(m.message) + "," + (m.a == other ? "O" : "?") + ",T;";
                        if (p56 != 0) want += "isopen:D;";
                        want += (p56 != 0 && open) ? "dialog:-8,O" + args : "recv:24,O" + args;
                    }
                    check(__LINE__, trace == want);
                    const bool done = m.message == 4358;
                    check(__LINE__, mcerror == (done && m.a == reinterpret_cast<void*>(3) ? 1 : 0));
                    check(__LINE__, mcerring == (done ? 0 : 7));
                    check(__LINE__, word(1408) == (done ? 42 : 41));
                    check(__LINE__, static_cast<unsigned>(word(1412)) == (m.message == 4354 ? other_word : 0x66666666u));
                }
            }
        }
    }
    std::puts(g_ok ? "ok" : "FAIL");
    return !g_ok;
}
