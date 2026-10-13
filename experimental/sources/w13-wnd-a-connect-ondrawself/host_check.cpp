// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Library string routines are
// renamed so the host program can supply simple versions.
#define strcpy host_strcpy
#define strcat host_strcat
#define strcmp host_strcmp
#define strncmp host_strncmp
#define strcasecmp host_strcasecmp
#define strstr host_strstr
#define sprintf host_sprintf
#define printf host_printf
#include "ondrawself.cpp"
#undef strcpy
#undef strcat
#undef strcmp
#undef strncmp
#undef strcasecmp
#undef strstr
#undef sprintf
#undef printf
#include <cstdarg>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[2100];
alignas(8) static int table[32];
alignas(8) static unsigned char entries[2 * 620];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
unsigned _5VIWnd$mFocus;
static std::string trace;
static int memcard, unet_status, font_width, status_word;
static std::string status_text;
static unsigned long long now = 1000;
static std::string off(const void* p) {
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 2100 ? std::to_string(d) : "?";
}
static std::string narrow(const unsigned short* s) {
    std::string r;
    for (; *s; ++s) r += static_cast<char>(*s);
    return r;
}

extern "C" {
int host_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
int host_strncmp(const char* a, const char* b, unsigned long n) { return std::strncmp(a, b, n); }
int host_strcasecmp(const char* a, const char* b) {
    for (;; ++a, ++b) {
        const int x = std::tolower(static_cast<unsigned char>(*a)), y = std::tolower(static_cast<unsigned char>(*b));
        if (x != y || x == 0) return x - y;
    }
}
char* host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
char* host_strcat(char* d, const char* s) { return std::strcat(d, s); }
char* host_strstr(const char* h, const char* n) { return const_cast<char*>(std::strstr(h, n)); }
int host_sprintf(char* d, const char* f, ...) {
    va_list args;
    va_start(args, f);
    const int v = va_arg(args, int);
    va_end(args);
    trace += std::string("sprintf:") + f + ";";
    return std::snprintf(d, 32, "%d", v);
}
int host_printf(const char* f, ...) { trace += std::string("printf:") + f; return 0; }
int VIMemCardStatus__FPi(int* p) { trace += std::string("memcard:") + (p ? "?" : "null") + ";"; return memcard; }
unsigned long long GetTime__7VITimer() { return now; }
void ResetTickPulse__10VIUIWindow(void* w) { trace += "tick:" + off(w) + ";"; }
void RemoveChild__5VIWndP5VIWnd(void* s, void* c) { trace += "rm:" + off(s) + "," + std::to_string(reinterpret_cast<unsigned long>(c)) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* s, void* t, bool f) { trace += "focus:" + off(s) + "," + off(t) + "," + std::to_string(f) + ";"; }
int ezNetCtlDownInterface(int v) { trace += "down:" + std::to_string(v) + ";"; return 0; }
int ezNetCtlGetStatus(void* p) {
    unsigned char* b = static_cast<unsigned char*>(p);
    std::memcpy(b + 12, &status_word, 4);
    std::strcpy(reinterpret_cast<char*>(b + 16), status_text.c_str());
    trace += "status;";
    return 0;
}
int unet_startup_async_status__Fv() { trace += "unetstatus;"; return unet_status; }
void unet_shutdown__Fv() { trace += "shutdown;"; }
void unet_startup_async__Fv() { trace += "startasync;"; }
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += std::string("snd:") + (ui == table ? "ui" : "?") + "," + std::to_string(s) + ";"; }
void Begin2D__4VIUI(void*) { trace += "begin;"; }
void End2D__4VIUI(void*) { trace += "end;"; }
void Draw__10VIUIWindow(void* w) { trace += "draw:" + off(w) + ";"; }
void DrawFrame__10VIUIWindowiiiiUi(void* w, int a, int b, int c, int d, unsigned e) {
    trace += "frame:" + off(w) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e) + ";";
}
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned e) {
    trace += "tb:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(t) < 100000 ? 0 : 1) + "," + std::to_string(a) + "," + std::to_string(b) + "," +
             std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(color) + "," + std::to_string(e) + ";";
}
void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int color, unsigned c) {
    trace += "text:" + off(w) + "," + narrow(t) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(color) + "," + std::to_string(c) + ";";
}
const unsigned short* T__10VIUIWindowPCc(void* w, const char* k) { trace += "T:" + off(w) + "," + k + ";"; return reinterpret_cast<const unsigned short*>(7); }
void SetTitle__10VIUIWindowPCUs(void* w, const unsigned short*) { trace += "title16:" + off(w) + ";"; }
void SetTitle__10VIUIWindowPCc(void* w, const char* t) { trace += "titlec:" + off(w) + "," + t + ";"; }
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, unsigned ui, const int* p, const int* s, int a, int b, int c) {
    trace += "init:" + off(w) + "," + std::string(ui == _5VIWnd$mUI ? "ui" : "?") + "," + std::to_string(p[0]) + "," + std::to_string(p[1]) + "," +
             std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ";";
}
void Open__10VIUIWindowb(void* w, bool f) { trace += "open:" + off(w) + "/" + std::to_string(f) + ";"; }
void Close__10VIUIWindowb(void* w, bool f) { trace += "close:" + off(w) + "/" + std::to_string(f) + ";"; }
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* w, const unsigned short* a, const unsigned short* b, int prompt, const int* size, int n) {
    trace += "setup:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(a)) + "," + narrow(b) + "," + std::to_string(prompt) + "," +
             std::to_string(size[0]) + "," + std::to_string(size[1]) + "," + std::to_string(n) + ";";
}
void BeginConnect__12VIWndConnect(void* s) { trace += "beginconnect:" + off(s) + ";"; }
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void*, void*, void*, const unsigned short* t) { trace += "width16:" + narrow(t) + ";"; return font_width; }
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void*, void*, void*, const char* t) { trace += std::string("width:") + t + ";"; return font_width; }
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == table ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    static unsigned short pool[4][8];
    static int next;
    unsigned short* p = pool[next++ & 3];
    p[0] = static_cast<unsigned short>('a' + id % 20); p[1] = 0;
    return p;
}
void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) { int i = 0; for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]); d[i] = 0; }
void strcpy16__8VIStringPUsUiPCUs(unsigned short* d, unsigned, const unsigned short* s) { int i = 0; for (; s[i]; ++i) d[i] = s[i]; d[i] = 0; }
void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned, const unsigned short* s) { int e = 0; while (d[e]) ++e; int i = 0; for (; s[i]; ++i) d[e + i] = s[i]; d[e + i] = 0; }
void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned, const char* s) { int e = 0; while (d[e]) ++e; int i = 0; for (; s[i]; ++i) d[e + i] = static_cast<unsigned char>(s[i]); d[e + i] = 0; }
void snprintf16__8VIStringPUsiPCUse(unsigned short* d, int, const unsigned short* f, ...) { trace += "snprintf16;"; strcat16__8VIStringPUsUiPCUs(d, 0, f); }
}
static void vcall_entry(void* self, int key) { trace += "vcall:" + off(self) + "," + std::to_string(key) + ";"; }
alignas(8) static unsigned char vtable[128];

static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }
static void reset() {
    std::memset(object, 0, sizeof object);
    std::memset(entries, 0, sizeof entries);
    for (int i = 0; i < 2; ++i) {
        int first = 1000 + i;
        std::memcpy(entries + 620 * i, &first, 4);
        std::strcpy(reinterpret_cast<char*>(entries) + 620 * i + 4, "n4");
        std::strcpy(reinterpret_cast<char*>(entries) + 620 * i + 260, i ? "SCE/Modem (Network Adaptor)" : "plain device");
    }
    table[12] = 3;
    std::memset(vtable, 0, sizeof vtable);
    // The host entry is wider than the 8-byte EE entry, so the fields are written at their host offsets.
    const short delta = 8;
    std::memcpy(vtable + 40, &delta, sizeof delta);
    void (*fn)(void*, int) = vcall_entry;
    std::memcpy(vtable + 40 + offsetof(VtEntry, function), &fn, sizeof fn);
    const unsigned long vt = reinterpret_cast<unsigned long>(vtable);
    set_word(32, static_cast<int>(vt));
    set_word(1972, static_cast<int>(reinterpret_cast<unsigned long>(entries)));
    set_word(1188, 111); set_word(1192, 222); set_word(1176 + 20, 640); set_word(1544 + 20, 500);
    _5VIWnd$mFocus = static_cast<unsigned>(reinterpret_cast<unsigned long>(object));
    memcard = 0; unet_status = 1; font_width = 250; status_word = 0; status_text = "Ready"; now = 1000;
    final_delay = 60; dot = 0; dot_delay = 15;
    trace.clear();
}

int main() {
    bool ok = true;
    const std::string list_line = "begin;draw:1176;frame:1176,40,1,560,2,32;sprintf:%d;T:1176,1000;tb:1176,1,40,1,60,1,0,0;T:1176,n4;tb:1176,1,60,1,540,1,0,2;"
                                  "T:1176,plain device;tb:1176,1,60,2,540,1,0,2;gs:ui,756,3;gs:ui,750,3;text:1176,|x o   |c  ,40,4,0,0;";
    // The virtual key-135 path leaves at once.
    reset(); memcard = 1; set_word(1964, 5);
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace == "memcard:null;vcall:8,135;" && word(1964) == 5;
    // Idle: only the list line is drawn; the delay counter runs.
    reset(); set_word(1964, 5);
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("memcard:null;" + list_line.substr(0, 20)) == 0 && word(1964) == 0 && trace.find("end;") == trace.size() - 4 && dot_delay == 14 && dot == 0;
    reset(); dot_delay = 1; dot = 2;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && dot_delay == 15 && dot == 0;
    reset(); dot_delay = 1; dot = 0;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && dot_delay == 15 && dot == 1;
    // Connecting, no async start yet, status 3 counts down and starts the async part at zero.
    reset(); set_word(1920, 1); status_word = 3; status_text = "Connected"; final_delay = 1;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && final_delay == 30 && word(1924) == 1 && trace.find("startasync;") != std::string::npos && trace.find("tick:1176;") != std::string::npos;
    reset(); set_word(1920, 1); status_word = 2; status_text = "Ready"; now = 5000; set_word(1976, 70);
    { unsigned long long v = 9; std::memcpy(object + 1984, &v, 8); }
    OnDrawSelf__12VIWndConnect(object);
    { unsigned long long v; std::memcpy(&v, object + 1984, 8); ok = ok && v == 5070 && final_delay == 60; }
    ok = ok && trace.find("printf:retrying... (reset timeout)\n") != std::string::npos;
    // Status 5 raises the failure prompt on this + 40 and stops the connection.
    reset(); set_word(1920, 1); status_word = 5; status_text = "Ready"; font_width = 350; final_delay = 7;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("setup:40,") != std::string::npos && trace.find(",0,111,222,400;focus:0,40,1;") != std::string::npos && word(68) == 1 && word(1920) == 0 &&
         word(1924) == 0 && final_delay == 60;
    // Detached: virtual call, a prompt; the Ethernet/Modem entry takes the snprintf16 text.
    reset(); set_word(1920, 1); status_text = "detached"; set_word(1916, 1); font_width = 350;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("vcall:8,135;width:SCE/Modem (Network Adaptor);") != std::string::npos && trace.find("snprintf16;") != std::string::npos &&
         trace.find(",0,111,222,430;focus:0,40,1;") != std::string::npos && word(68) == 1;
    reset(); set_word(1920, 1); status_text = "detached"; font_width = 100;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("snprintf16;") == std::string::npos && trace.find(",0,111,222,380;focus:0,40,1;") != std::string::npos;
    // Async branch finishing: shutdown and focus release when the status reports done and final_delay is 0.
    reset(); set_word(1920, 1); set_word(1924, 1); final_delay = 0; unet_status = 0;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("shutdown;focus:0,?,0;") == std::string::npos && trace.find("shutdown;focus:0,?,1;") != std::string::npos && word(1964) == 0;
    reset(); set_word(1920, 1); set_word(1924, 1); final_delay = 5; unet_status = 1; font_width = 120; dot = 1;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && final_delay == 4 && word(1964) == 1 && trace.find("init:1544,ui,0,0,111,222,320,3,0;") != std::string::npos && trace.find("tb:1544,1,40,1,500,1,0,0;") != std::string::npos;
    // Start countdown: 1956 reaching zero begins the connection.
    reset(); set_word(1956, 1); font_width = 300;
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && word(1956) == 0 && word(1920) == 1 && trace.find("snd:ui,15;beginconnect:0;close:1544/1;width:plain device;init:1544,ui,0,0,111,222,420,4,0;open:1544/0;end;") != std::string::npos;
    reset(); set_word(1956, 2);
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && word(1956) == 1 && word(1920) == 0;
    // A pending child is removed; the timeout resets the interface.
    reset(); set_word(1912, 77); set_word(1920, 1); now = 100; { unsigned long long v = 50; std::memcpy(object + 1984, &v, 8); }
    status_text = "Connected";
    OnDrawSelf__12VIWndConnect(object);
    ok = ok && trace.find("rm:0,77;") != std::string::npos && word(1912) == 0 && trace.find("down:0;snd:ui,8;") != std::string::npos;
    { unsigned long long v; std::memcpy(&v, object + 1984, 8); ok = ok && v == 100100; }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
