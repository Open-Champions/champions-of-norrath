// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the 32-bit UI and entry-array words
// translate to host objects through tags. Every callee is a recording stub; the checks cover the busy-card and
// -8 paths (McErr, return -1), the message-box path for adapter names (both strstr hits, a miss, and the width
// floor) and the success path with the three property probes.
#define NORRATH_HOST_CONTRACT 1
#include "bringup.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
static const unsigned TAG_UI = 0x1000, TAG_ENT = 0x2000;
static unsigned char hl_ui[256];
static unsigned char entries[620 * 3];
static unsigned char self_object[2048] __attribute__((aligned(16)));
static std::string trace;
static unsigned char properties[4928];
static int status_result, comb_result, prop_result, width_result;
static long long time_result;
static unsigned short texts[2000][4];

extern "C" char* norrath_host_translate(unsigned address) {
    return address == TAG_UI ? reinterpret_cast<char*>(hl_ui) : address == TAG_ENT ? reinterpret_cast<char*>(entries) : nullptr;
}
static std::string narrow(const unsigned short* t) {
    std::string r;
    while (*t) r += static_cast<char>(*t++);
    return r;
}
extern "C" {
int VIMemCardStatus__FPi(int* v) { trace += v ? "status(ptr);" : "status(0);"; return status_result; }
void McErr__FP5VIWndRC8VIVect2Ii(void* window, const int* size, int screen) {
    trace += std::string("mcerr(") + (window == self_object ? "self" : "?") + "," + std::to_string(size[0]) + "/" + std::to_string(size[1]) + "," + std::to_string(screen) + ");";
}
int ezNetCtlSetCombination(const char* name, void* combination) {
    trace += std::string("setcomb(") + name + "," + std::to_string(static_cast<unsigned char*>(combination)[0]) + ");";
    return comb_result;
}
int ezNetCnfGetProperties(const char* name, void* combination, void* props) {
    trace += std::string("getprops(") + name + "," + std::to_string(static_cast<unsigned char*>(combination)[0]) + "," + (props == properties ? "buf" : "?") + ");";
    return prop_result;
}
int ezNetCtlDownInterface(int i) { trace += "down(" + std::to_string(i) + ");"; return 0; }
int ezNetCtlUpInterface(int i) { trace += "up(" + std::to_string(i) + ");"; return 0; }
void* __wrap_memalign(unsigned a, unsigned s) { trace += "memalign(" + std::to_string(a) + "," + std::to_string(s) + ");"; return properties; }
void __wrap_free(void* b) { trace += std::string("free(") + (b == properties ? "buf" : "?") + ");"; }
long long GetTime__7VITimer() { trace += "time;"; return time_result; }
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, void*, void*, const char* text) {
    trace += std::string("width(") + (font == hl_ui + 56 ? "font" : "?") + "," + text + ");";
    return width_result;
}
void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned n, const char* s) {
    trace += "s8(" + std::to_string(n) + "," + s + ");";
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned n, const char* s) {
    trace += "c8(" + std::to_string(n) + "," + s + ");";
    unsigned k = 0;
    while (d[k]) ++k;
    for (unsigned i = 0; s[i]; ++i) d[k++] = static_cast<unsigned char>(s[i]);
    d[k] = 0;
}
void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned n, const unsigned short* s) {
    trace += "c16(" + std::to_string(n) + "," + narrow(s) + ");";
    unsigned k = 0;
    while (d[k]) ++k;
    for (unsigned i = 0; s[i]; ++i) d[k++] = s[i];
    d[k] = 0;
}
int snprintf16__8VIStringPUsiPCUse(unsigned short* d, int n, const unsigned short* f, ...) {
    trace += "snp(" + std::to_string(n) + "," + narrow(f) + ");";
    unsigned i = 0;
    for (; f[i]; ++i) d[i] = f[i];
    d[i] = 0;
    return 0;
}
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column) {
    trace += std::string("gs(") + (table == hl_ui ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(column) + ");";
    const std::string t = "T" + std::to_string(id);
    unsigned short* out = texts[id % 2000];
    unsigned i = 0;
    for (; i < t.size(); ++i) out[i] = static_cast<unsigned short>(t[i]);
    out[i] = 0;
    return out;
}
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* s, const unsigned short* title, const unsigned short* text, int prompt, const int* size, int width) {
    trace += std::string("setup(") + (s == self_object + 40 ? "window" : "?") + "," + narrow(title) + "," + narrow(text) + "," + std::to_string(prompt) + "," +
             std::to_string(size[0]) + "/" + std::to_string(size[1]) + "," + std::to_string(width) + ");";
}
void SetFocus__5VIWndP5VIWndb(void* s, void* w, bool f) {
    trace += std::string("focus(") + (s == self_object ? "self" : "?") + "," + (w == self_object + 40 ? "window" : "?") + "," + std::to_string(f) + "," +
             std::to_string(*reinterpret_cast<int*>(self_object + 68)) + ");";
}
}

static void reset(const char* name, int index) {
    std::memset(self_object, 0, sizeof self_object);
    std::memset(entries, 0, sizeof entries);
    std::memset(properties, 0, sizeof properties);
    std::memset(hl_ui, 0, sizeof hl_ui);
    *reinterpret_cast<int*>(hl_ui + 48) = 3;
    _5VIWnd$mUI = TAG_UI;
    int* w = reinterpret_cast<int*>(self_object);
    w[479] = index;
    w[493] = static_cast<int>(TAG_ENT);
    w[297] = 11;
    w[298] = 22;
    w[480] = 9;
    w[481] = 9;
    std::strcpy(reinterpret_cast<char*>(entries + 620 * index + 260), name);
    entries[620 * index + 516] = 77;
    entries[620 * index + 524] = 88;
    trace.clear();
}
static int check(const char* label, const std::string& expected) {
    if (trace == expected) return 0;
    std::fprintf(stdout, "MISMATCH %s\n  got:  %s\n  want: %s\n", label, trace.c_str(), expected.c_str());
    return 1;
}

int main() {
    int bad = 0;
    int* w = reinterpret_cast<int*>(self_object);
    long long* t1976 = reinterpret_cast<long long*>(self_object + 1976);
    long long* t1984 = reinterpret_cast<long long*>(self_object + 1984);

    reset("x", 1);
    status_result = 3;
    bad += BringUpNetwork__12VIWndConnect(self_object) != -1;
    bad += check("busy card", "status(0);mcerr(self,11/22,4);");
    bad += w[480] != 0;

    reset("x", 2);
    status_result = 0;
    comb_result = -8;
    bad += BringUpNetwork__12VIWndConnect(self_object) != -1;
    bad += check("-8", "status(0);setcomb(mc0:,88);mcerr(self,11/22,4);");
    bad += w[480] != 0 || w[481] != 9;

    struct Msg { const char* name; const char* tail; int width; int expect; };
    const Msg msgs[] = {
        {"SCE/Ethernet (Network Adaptor)", "snp(512,T1026);", 100, 380},
        {"SCE/Modem (Network Adaptor)", "snp(512,T1026);", 400, 480},
        {"Other", "gs(ui,780,3);c16(512,T780);", 300, 380},
    };
    for (const Msg& m : msgs) {
        reset(m.name, 1);
        status_result = 0;
        comb_result = -3;
        width_result = m.width;
        BringUpNetwork__12VIWndConnect(self_object);
        std::string name = m.name;
        std::string snp = std::string(m.tail).rfind("snp", 0) == 0 ? "gs(ui,1026,3);" : "";
        std::string want = "status(0);setcomb(mc0:,88);width(font," + name + ");s8(512,\");c8(512," + name + ");c8(512,\"\n);";
        want += snp + m.tail;
        const bool replaced = std::string(m.tail).rfind("snp", 0) == 0;
        // snprintf16 starts the text over (the format string replaces the quoted name); the other branch appends.
        want += "gs(ui,767,3);setup(window,T767," + std::string(replaced ? "T1026" : "\"" + name + "\"\nT780") + ",0,11/22," + std::to_string(m.expect) + ");focus(self,window,1,1);";
        bad += check("message", want);
        bad += w[480] != 0 || w[481] != 0;
    }

    for (int probe : {-1, 0, 2048, 2304, 2560}) {
        reset("x", 0);
        status_result = 0;
        comb_result = 0;
        prop_result = probe == -1 ? 5 : 0;
        if (probe > 0) properties[probe] = 1;
        time_result = 1000;
        BringUpNetwork__12VIWndConnect(self_object);
        const long long timeout = probe == -1 ? 180000 : (probe == 0 ? 45000 : 120000);
        bad += check("success", std::string("status(0);setcomb(mc0:,88);memalign(64,4928);getprops(mc0:,88,buf);free(buf);down(0);up(0);time;"));
        bad += *t1976 != timeout || *t1984 != 1000 + timeout;
    }
    std::puts(bad ? "FAIL" : "ok");
    return bad != 0;
}
