// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -fno-builtin -DNORRATH_HOST_ARENA host_check.cpp
// Every callee is a stub that appends its arguments to a trace; the clock, socket and receive queue are scripted. Each scenario below
// exercises one branch family and its expected calls and stores are written out by hand from the notes at the top of process.cpp.
#include <cstdio>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "process.cpp"

int frame_counter;
int connected;
long long g_lastSessionTouch;
int g_mode, g_myIndex, g_numClients, g_localPort;
char g_ips[96], g_localClientIps[96], g_localIp[24];
int g_ports[4], g_localClientPorts[4];
unsigned vi_wnd_mui;

static const unsigned OBJ = 0x20000, RENDERER = 0x2b000, DIALOG = 0x2b100, PAGE = 0x2b200, TABLE = 0x2b300, APP1 = 0x40000, APP2 = 0x60000;
static std::string trace;
static long long clockNow, clockStep;
static int appCalls, socketOpen, openWorks, resolveWorks, localWorks;
static std::string pageName;
static unsigned recvLength;
static unsigned char recvFrom[20], recvBuffer[1024];
static int recvPort;
static unsigned short strings[4000];
static std::string hex(const unsigned char* p, unsigned n) {
    static const char* d = "0123456789abcdef"; std::string s;
    for (unsigned i = 0; i < n; ++i) { s += d[p[i] >> 4]; s += d[p[i] & 15]; }
    return s;
}
static std::string off(const void* p, unsigned base) { return std::to_string(static_cast<const unsigned char*>(p) - arena_at(base)); }
static std::string formatText(const char* f, __builtin_va_list ap) {
    std::string out;
    for (const char* p = f; *p; ++p) {
        if (*p != '%') { out += *p; continue; }
        ++p;
        if (*p == 'i' || *p == 'd') out += std::to_string(__builtin_va_arg(ap, int));
        else if (*p == 'c') out += static_cast<char>(__builtin_va_arg(ap, int));
        else if (*p == 's') out += __builtin_va_arg(ap, const char*);
    }
    return out;
}
extern "C" {
int stub_printf(const char* f, ...) __asm__("_printf");
int stub_printf(const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    std::string t = formatText(f, ap);
    __builtin_va_end(ap);
    trace += "printf:" + t + ";";
    return 0;
}
int stub_sprintf(char* out, const char* f, ...) __asm__("_sprintf");
int stub_sprintf(char* out, const char* f, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, f);
    std::string t = formatText(f, ap);
    __builtin_va_end(ap);
    for (size_t i = 0; i <= t.size(); ++i) out[i] = t[i];
    trace += "sprintf:" + t + ";";
    return static_cast<int>(t.size());
}
void* stub_memset(void* d, int v, unsigned long n) __asm__("_memset");
void* stub_memset(void* d, int v, unsigned long n) {
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned long i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    if (n == 24 && p >= arena_at(OBJ) && p < arena_at(OBJ + 0x9800)) trace += "memset24:" + off(d, OBJ) + ";";
    return d;
}
int stub_memcmp(const void* a, const void* b, unsigned long n) __asm__("_memcmp");
int stub_memcmp(const void* a, const void* b, unsigned long n) {
    const unsigned char* x = static_cast<const unsigned char*>(a); const unsigned char* y = static_cast<const unsigned char*>(b);
    for (unsigned long i = 0; i < n; ++i) if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}
int stub_strcmp(const char* a, const char* b) __asm__("_strcmp");
int stub_strcmp(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return static_cast<unsigned char>(*a) - static_cast<unsigned char>(*b); }
char* stub_strcpy(char* d, const char* s) __asm__("_strcpy");
char* stub_strcpy(char* d, const char* s) { char* r = d; while ((*d++ = *s++)) {} return r; }
char* stub_strstr(const char* h, const char* n) __asm__("strstr");
char* stub_strstr(const char* h, const char* n) {
    for (const char* p = h; *p; ++p) { const char *a = p, *b = n; while (*a && *b && *a == *b) { ++a; ++b; } if (!*b) return const_cast<char*>(p); }
    return nullptr;
}
long long GetTime__7VITimer() { long long t = clockNow; clockNow += clockStep; trace += "time;"; return t; }
void* GetAppPtr__5VIWnd(void* r) { trace += "app:" + off(r, RENDERER) + ";"; return arena_at(appCalls++ == 0 ? APP1 : APP2); }
void ForceDisconnect__20VIWndGenericRenderer(void*) { trace += "forcedisc;"; }
const char* GetName__9VIGUIPage(void*) { trace += "name;"; return pageName.c_str(); }
void SetStringValue__9VIGUIPagePCcT1b(void* p, const char* n, const char* v, int f) {
    trace += "ssv:" + off(p, PAGE) + "," + n + "=" + v + "," + std::to_string(f) + ";";
}
void SetStringValue__9VIGUIPagePCcPCUsb(void* p, const char* n, const unsigned short* v, int f) {
    trace += "ssvw:" + off(p, PAGE) + "," + n + "=" + std::to_string(v - strings) + "," + std::to_string(f) + ";";
}
void touchSession__Q212RealmService15RealmGatewayAPIPCUsPv(void*, const unsigned short* s, void* d) { trace += "touch:" + off(s, OBJ) + "," + std::to_string(d != nullptr) + ";"; }
void getRealmStats__Q212RealmService15RealmGatewayAPIPv(void*, void* d) { trace += "stats:" + std::to_string(d != nullptr) + ";"; }
void __Q212RealmService14MatchGameAttrs(void*) { trace += "attrs;"; }
void _$_Q212RealmService14MatchGameAttrs(void*, int f) { trace += "attrsend:" + std::to_string(f) + ";"; }
void matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv(
    void*, const unsigned short* s, short a, unsigned b, unsigned c, int d, const void*, bool e, unsigned f, unsigned g, void* h) {
    trace += "match:" + off(s, OBJ) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," +
             std::to_string(e) + "," + std::to_string(f) + "," + std::to_string(g) + "," + std::to_string(h != nullptr) + ";";
}
void cancelGame__Q212RealmService15RealmGatewayAPIPCUsPv(void*, const unsigned short* s, void* d) { trace += "cancel:" + off(s, OBJ) + "," + std::to_string(d != nullptr) + ";"; }
int IsConnected__11VIUDPSocket(void*) { trace += std::string("isconn:") + (socketOpen ? "1" : "0") + ";"; return socketOpen; }
void Open__11VIUDPSocket(void* s) { trace += "open:" + std::to_string(norrath_aoc::load_word(static_cast<unsigned char*>(s))) + ";"; if (openWorks) socketOpen = 1; }
void GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void*, void* a, const char* h, int port) {
    unsigned char* p = static_cast<unsigned char*>(a);
    trace += "resolve:" + off(a, OBJ) + "," + h + "," + std::to_string(port) + ";";
    for (int i = 0; i < 16; ++i) p[i] = static_cast<unsigned char>(0x40 + i);
    norrath_aoc::store_word(p + 16, resolveWorks ? 1 : 0);
}
void GetLocalAddress__11VIUDPSocketP13VIInetAddress(void*, void* a) {
    unsigned char* p = static_cast<unsigned char*>(a);
    trace += "local:" + off(a, OBJ) + ";";
    for (int i = 0; i < 16; ++i) p[i] = static_cast<unsigned char>(0x60 + i);
    norrath_aoc::store_word(p + 16, localWorks ? 1 : 0);
}
void GetStringAddress__11VIUDPSocketP13VIInetAddressPc(void*, void* a, char* out) {
    trace += "stringaddr:" + off(a, OBJ) + "," + off(out, OBJ) + ";";
    const char* t = "10.1.2.3"; for (int i = 0; i < 9; ++i) out[i] = t[i];
}
void UpdateNumPlayers__16VIRealmInterfacei(void* s, int n) { trace += "update:" + std::to_string(n) + ";"; norrath_aoc::store_word(static_cast<unsigned char*>(s) + 3476, static_cast<unsigned>(n)); }
int packDiscoveryPacket__Q212RealmService15RealmGatewayAPIPUcPCUs(void*, unsigned char* out, const unsigned short* n) {
    trace += "pack:" + std::to_string(n[0]) + ";"; for (int i = 0; i < 8; ++i) out[i] = static_cast<unsigned char>('P' + i); return 8;
}
int SendTo__11VIUDPSocketG13VIInetAddressiPCciPii(void*, const void* a, int port, const char* d, int n, int* x, int y) {
    trace += "send1:" + hex(static_cast<const unsigned char*>(a), 20) + "," + std::to_string(port) + "," + hex(reinterpret_cast<const unsigned char*>(d), n) + "," +
             std::to_string(n) + "," + std::to_string(x != nullptr) + std::to_string(y) + ";";
    return n;
}
int SendTo__11VIUDPSocketG13VIInetAddressiT1T1iPCciPii(void*, const void* a, int port, const void* b, const void* c, int i, const char* d, int n, int* x, int y) {
    trace += "send2:" + hex(static_cast<const unsigned char*>(a), 20) + "," + std::to_string(port) + "," + hex(static_cast<const unsigned char*>(b), 20) + "," +
             hex(static_cast<const unsigned char*>(c), 20) + "," + std::to_string(i) + "," + hex(reinterpret_cast<const unsigned char*>(d), n) + "," +
             std::to_string(n) + "," + std::to_string(x != nullptr) + std::to_string(y) + ";";
    return n;
}
int SendTo__11VIUDPSocketG13VIInetAddressiT1PCciT4iPii(void*, const void* a, int port, const void* b, const char* t, int i, const char* d, int n, int* x, int y) {
    trace += "send3:" + hex(static_cast<const unsigned char*>(a), 20) + "," + std::to_string(port) + "," + hex(static_cast<const unsigned char*>(b), 20) + "," +
             t + "," + std::to_string(i) + "," + hex(reinterpret_cast<const unsigned char*>(d), n) + "," + std::to_string(n) + "," +
             std::to_string(x != nullptr) + std::to_string(y) + ";";
    return n;
}
int RecvFrom__11VIUDPSocketP13VIInetAddressPiPciT2i(void*, void* from, int* port, char* buf, int n, int* x, int y) {
    trace += "recv:" + std::to_string(n) + std::to_string(x != nullptr) + std::to_string(y) + ";";
    for (int i = 0; i < 20; ++i) static_cast<unsigned char*>(from)[i] = recvFrom[i];
    *port = recvPort;
    for (unsigned i = 0; i < 1024; ++i) buf[i] = static_cast<char>(recvBuffer[i]);
    return static_cast<int>(recvLength);
}
void GoBack__20VIWndGenericRenderer(void*) { trace += "goback;"; }
void Finish__20VIWndGenericRenderer(void*) { trace += "finish;"; }
void Reset__16VIRealmInterface(void*) { trace += "reset;"; }
const unsigned short* GetString__13VIStringTableii(void* t, int id, int third) {
    trace += "get:" + off(t, TABLE) + "," + std::to_string(id) + "," + std::to_string(third) + ";";
    return strings + id;
}
void Show__11VIGUIDialogPCUsPCc(void*, const unsigned short* t, const char* x) { trace += "show:" + std::to_string(t - strings) + "," + std::to_string(x != nullptr) + ";"; }
void SetText__11VIGUIDialogPCUsb(void*, const unsigned short* t, int f) { trace += "settext:" + std::to_string(t - strings) + "," + std::to_string(f) + ";"; }
void process__Q212RealmService15RealmGatewayAPI(void*) { trace += "base;"; }
}

typedef std::vector<unsigned char> Bytes_;
static Bytes_ addrOf(unsigned char first, unsigned flag) {
    Bytes_ a; for (int i = 0; i < 16; ++i) a.push_back(static_cast<unsigned char>(first + i));
    for (int i = 0; i < 4; ++i) a.push_back(static_cast<unsigned char>(flag >> (8 * i)));
    return a;
}
static std::string H(const Bytes_& b) { return hex(b.data(), static_cast<unsigned>(b.size())); }
static void putText(Bytes_& b, unsigned at, const std::string& s) { for (size_t i = 0; i <= s.size(); ++i) b[at + i] = i < s.size() ? static_cast<unsigned char>(s[i]) : 0; }
static void putWord(Bytes_& b, unsigned at, unsigned v) { for (int i = 0; i < 4; ++i) b[at + i] = static_cast<unsigned char>(v >> (8 * i)); }

static unsigned char* O() { return arena_at(OBJ); }
static void setText(unsigned at, const std::string& s) { char* p = reinterpret_cast<char*>(arena_at(OBJ + at)); for (size_t i = 0; i <= s.size(); ++i) p[i] = s[i]; }
static void setWord(unsigned at, unsigned v) { norrath_aoc::store_word(O() + at, v); }
static void setTime(unsigned at, long long ago) { long long v = clockNow - ago; *reinterpret_cast<i64_t*>(O() + at) = v; }
static unsigned getWord(unsigned at) { return norrath_aoc::load_word(O() + at); }
static void setAddr(unsigned at, unsigned char first, unsigned flag) { Bytes_ a = addrOf(first, flag); for (size_t i = 0; i < 20; ++i) O()[at + i] = a[i]; }

static bool ok = true;
static int scenarioNo = 0;
static void reset() {
    for (unsigned i = 0; i < 0xa000; ++i) O()[i] = 0;
    for (unsigned i = 0; i < 0x20000; ++i) { arena_at(APP1)[i] = 0; arena_at(APP2)[i] = 0; }
    norrath_aoc::store_pointer(O() + 12, arena_at(RENDERER));
    norrath_aoc::store_pointer(arena_at(RENDERER) + 52, arena_at(PAGE));
    norrath_aoc::store_pointer(arena_at(RENDERER) + 56, arena_at(DIALOG));
    norrath_aoc::store_pointer(reinterpret_cast<unsigned char*>(&vi_wnd_mui), arena_at(TABLE));
    norrath_aoc::store_word(arena_at(TABLE) + 48, 77);
    clockNow = 1000000; clockStep = 0; appCalls = 0; socketOpen = 0; openWorks = 1; resolveWorks = 1; localWorks = 1;
    pageName = "Other"; recvLength = 0; recvPort = 0; frame_counter = 0;
    for (int i = 0; i < 20; ++i) recvFrom[i] = 0;
    for (int i = 0; i < 1024; ++i) recvBuffer[i] = 0;
    connected = 0; g_lastSessionTouch = clockNow - 1; g_mode = g_myIndex = g_numClients = 0; g_localPort = 4321;
    const char* ip = "192.168.0.5"; for (int i = 0; i < 12; ++i) g_localIp[i] = ip[i];
    for (int i = 0; i < 96; ++i) g_ips[i] = g_localClientIps[i] = 0;
    for (int i = 0; i < 4; ++i) g_ports[i] = g_localClientPorts[i] = 0;
    setTime(9728, 1000); setTime(9720, 1000); setTime(9664, 1000);
    unsigned char* a1 = arena_at(APP1);
    const char* nm = "Hero"; for (int i = 0; i < 5; ++i) a1[0x17a34 + i] = nm[i];
    const char* cl = "Warrior"; for (int i = 0; i < 8; ++i) a1[0x17a74 + i] = cl[i];
    norrath_aoc::store_word(a1 + 0x17ab4, 37);
    setText(1300, "srv.host"); setText(2348, "cli.host"); setText(9692, "10.9.9.9");
    setWord(3372, 2222); setWord(2324, 3333);
    setAddr(0x947c, 0x70, 0); setAddr(0x8b08, 0x80, 0);
    setText(16, "");
    unsigned short* nameUnits = reinterpret_cast<unsigned short*>(O() + 16); nameUnits[0] = 78; nameUnits[1] = 0;
}
static void run(const char* what, const std::string& want, unsigned expectFrameAfter = 1) {
    trace.clear();
    process__16VIRealmInterface(O());
    bool good = trace == want && static_cast<unsigned>(frame_counter) == expectFrameAfter;
    ++scenarioNo;
    if (!good) {
        ok = false;
        std::fputs((std::string("scenario ") + what + "\n got  " + trace + "\n want " + want + "\n frame " + std::to_string(frame_counter) + "\n").c_str(), stdout);
    }
}
int main() {
    arena_at(0);
    const std::string head = "time;app:0;", tail = "base;";
    auto ssv = [](const std::string& k, const std::string& v) { return "ssv:0," + k + "=" + v + ",0;"; };

    // A. idle
    reset(); run("idle", head + "name;isconn:0;" + tail);
    // B. forced disconnect on the 30001 boundary
    reset(); setTime(9728, 30001); run("force disconnect", head + "forcedisc;name;isconn:0;" + tail);
    reset(); setTime(9728, 30000); run("no force disconnect", head + "name;isconn:0;" + tail);
    // C. roster page
    {
        reset(); pageName = "GameServ"; setWord(3476, 2);
        setText(6180, "Bob"); setText(6204, "Mage"); setWord(6228, 7);
        std::string want = head + "name;sprintf:L37;" + ssv("Player1Name", "Hero") + ssv("Player1Class", "Warrior") + ssv("Player1Level", "L37");
        want += "sprintf:Player2Name;sprintf:Player2Class;sprintf:Player2Level;sprintf:L7;" + ssv("Player2Name", "Bob") + ssv("Player2Class", "Mage") + ssv("Player2Level", "L7");
        for (int p = 3; p <= 4; ++p)
            want += "sprintf:Player" + std::to_string(p) + "Name;sprintf:Player" + std::to_string(p) + "Class;sprintf:Player" + std::to_string(p) + "Level;" +
                    ssv("Player" + std::to_string(p) + "Name", ".........") + ssv("Player" + std::to_string(p) + "Class", "......") + ssv("Player" + std::to_string(p) + "Level", "...");
        run("roster", want + "isconn:0;" + tail);
        reset(); pageName = "GameServ"; setWord(3476, 2); setText(6180, ""); setWord(6228, 9);
        want = head + "name;sprintf:L37;" + ssv("Player1Name", "Hero") + ssv("Player1Class", "Warrior") + ssv("Player1Level", "L37");
        want += "sprintf:Player2Name;sprintf:Player2Class;sprintf:Player2Level;sprintf:L9;" + ssv("Player2Name", "?") + ssv("Player2Class", "?") + ssv("Player2Level", "?");
        for (int p = 3; p <= 4; ++p)
            want += "sprintf:Player" + std::to_string(p) + "Name;sprintf:Player" + std::to_string(p) + "Class;sprintf:Player" + std::to_string(p) + "Level;" +
                    ssv("Player" + std::to_string(p) + "Name", ".........") + ssv("Player" + std::to_string(p) + "Class", "......") + ssv("Player" + std::to_string(p) + "Level", "...");
        run("roster empty name", want + "isconn:0;" + tail);
    }
    // D. session touch
    reset(); g_lastSessionTouch = clockNow - 10001; connected = 1; pageName = "FindGame";
    run("touch with find page", head + "name;time;printf:Touching session\n;touch:16,0;name;stats:0;attrs;match:16,2,0,0,-1,1,100,1024,0;attrsend:2;isconn:0;" + tail);
    reset(); g_lastSessionTouch = clockNow - 10001; connected = 1; pageName = "Other";
    run("touch other page", head + "name;time;printf:Touching session\n;touch:16,0;name;isconn:0;" + tail);
    reset(); g_lastSessionTouch = clockNow - 10001; connected = 0;
    run("touch not connected", head + "name;time;isconn:0;" + tail);
    reset(); g_lastSessionTouch = clockNow - 10000;
    run("touch not due", head + "name;isconn:0;" + tail);

    auto setRow = [](int i, unsigned char addrFirst, unsigned flag, unsigned port, unsigned status, long long ago, const std::string& text, const std::string& host,
                     unsigned w6176, const std::string& name, const std::string& klass, unsigned level) {
        const unsigned b = 1144 * i;
        setAddr(b + 6116, addrFirst, flag); setWord(b + 6112, port); setWord(b + 6136, status); setTime(b + 6144, ago);
        setText(b + 6152, text); setText(b + 5088, host); setWord(b + 6176, w6176);
        setText(b + 6180, name); setText(b + 6204, klass); setWord(b + 6228, level);
    };
    const std::string recvTail = "isconn:1;recv:102400;app:0;";
    // E. state 1: opens, resolves, learns the local address, sends the discovery packet
    {
        reset(); setWord(3468, 1); setWord(3476, 2);
        std::string want = head + "name;isconn:0;open:3000;resolve:2328,srv.host,5000;isconn:1;local:9672;memset24:9692;stringaddr:9672,9692;"
                           "printf:!!!!!!!!!!!!My address done equal (10.1.2.3)\n;update:2;pack:78;send1:" + H(addrOf(0x40, 1)) + ",3333,5051525354555657,8,00;" + recvTail + tail;
        run("state 1 frame 0", want);
        if (getWord(2344) != 1 || getWord(9688) != 1) { ok = false; std::fputs("state 1 words\n", stdout); }
        reset(); setWord(3468, 1); frame_counter = 1; socketOpen = 1; setWord(2344, 1);
        run("state 1 frame 1", head + "name;isconn:1;isconn:1;" + recvTail + tail, 2);
        reset(); setWord(3468, 1); socketOpen = 1; setWord(2344, 1); setWord(9688, 1); setAddr(2328, 0x40, 1);
        run("state 1 local known", head + "name;isconn:1;isconn:1;pack:78;send1:" + H(addrOf(0x40, 1)) + ",3333,5051525354555657,8,00;" + recvTail + tail);
    }
    // F. state 5
    {
        reset(); setWord(3468, 5); setWord(3476, 2);
        std::string want = head + "name;printf:Doing client discovery\n;isconn:0;open:3000;resolve:2328,srv.host,5000;isconn:1;local:9672;memset24:9692;stringaddr:9672,9692;"
                           "printf:!!!!!!!!!!!!My address done equal (10.1.2.3)\n;pack:78;send1:" + H(addrOf(0x40, 1)) + ",3333,5051525354555657,8,00;" + recvTail + tail;
        run("state 5 frame 0", want);
        reset(); setWord(3468, 5); frame_counter = 1;
        run("state 5 frame 1", head + "name;isconn:0;" + tail, 2);
    }
    // G. state 12: send the 108-byte packet
    {
        reset(); setWord(3468, 12);
        Bytes_ pk(108, 0); putWord(pk, 0, 12); putText(pk, 4, "10.9.9.9"); putText(pk, 28, "192.168.0.5"); putWord(pk, 52, 4321);
        putText(pk, 56, "Hero"); putText(pk, 80, "Warrior"); putWord(pk, 104, 37);
        std::string want = head + "name;isconn:0;printf:\tserver socket not connected\n;open:3000;printf:\tserver address not initialized\n;resolve:3376,cli.host,5000;isconn:1;"
                           "send2:" + H(addrOf(0x40, 1)) + ",2222," + H(addrOf(0x70, 0)) + "," + H(addrOf(0x80, 0)) + ",3000," + H(pk) + ",108,00;" + recvTail + tail;
        run("state 12 send", want);
        reset(); setWord(3468, 11); setWord(3392, 1); socketOpen = 1; frame_counter = 1; setTime(9720, 10001);
        run("state 11 timeout", head + "name;isconn:1;isconn:1;goback;get:0,1840,77;show:1840,0;get:0,1858,77;settext:1858,0;reset;" + recvTail + tail, 2);
        if (getWord(3468) != 0) { ok = false; std::fputs("state 11 timeout word\n", stdout); }
        reset(); setWord(3468, 11); setWord(3392, 1); socketOpen = 1; frame_counter = 1; setTime(9720, 10000);
        run("state 11 no timeout", head + "name;isconn:1;isconn:1;" + recvTail + tail, 2);
    }
    // H. states 2-4: ping to one row, start-game packet to the next
    {
        reset(); setWord(3468, 2); setWord(3476, 3); socketOpen = 1; setWord(3472, 2);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRow(1, 0x20, 1, 4100, 9, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        Bytes_ ping(256, 0); putWord(ping, 0, 7); putText(ping, 4, "10.9.9.9"); putText(ping, 28, "3/2");
        putText(ping, 32, "Hero"); putText(ping, 56, "Warrior"); putWord(ping, 80, 37);
        putText(ping, 84, "Bob"); putText(ping, 108, "Mage"); putWord(ping, 132, 11);
        putText(ping, 136, "Cy"); putText(ping, 160, "Monk"); putWord(ping, 184, 12);
        Bytes_ start(236, 0); putWord(start, 0, 9); putWord(start, 4, 3); putText(start, 8, "cli.host"); putText(start, 32, "hostA"); putText(start, 56, "hostB");
        putWord(start, 104, 2222); putWord(start, 108, 4000); putWord(start, 112, 4100); putText(start, 120, "10.9.9.9"); putText(start, 144, "ipA"); putText(start, 168, "ipB");
        putWord(start, 216, 3000); putWord(start, 220, 5); putWord(start, 224, 6); putWord(start, 232, 2);
        std::string want = head + "name;isconn:1;printf:Server sending ping packet to client, state = 7\n;sprintf:3/2;"
                           "send3:" + H(addrOf(0x10, 1)) + ",4000," + H(addrOf(0x70, 0)) + ",ipA,3000," + H(ping) + ",256,00;"
                           "isconn:1;sprintf:cli.host;sprintf:10.9.9.9;sprintf:hostA;sprintf:ipA;sprintf:hostB;sprintf:ipB;printf:Server sending start game packet to client\n;"
                           "send3:" + H(addrOf(0x20, 1)) + ",4100," + H(addrOf(0x70, 0)) + ",ipB,6," + H(start) + ",236,00;"
                           "printf:Server sending start game packet to client, sent 236 of 236 bytes\n;" + recvTail + tail;
        run("states 2-4 ping and start", want);
    }
    // I. a row that has not been heard from for 20001 is removed
    {
        reset(); setWord(3468, 2); setWord(3476, 3); socketOpen = 1; resolveWorks = 0;
        setRow(0, 0x10, 0, 4000, 0, 20001, "ipA", "hostA", 5, "Bob", "Mage", 11);     // flag word 0 -> resolved first, then no send
        setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        std::string want = head + "name;resolve:6116,hostA,5000;isconn:1;update:2;" + recvTail + tail;
        // rows after the removal: the old second row now sits where the first was
        run("row timeout", want);
        if (getWord(6112) != 4100 || getWord(3476) != 2) { ok = false; std::fputs("row timeout words\n", stdout); }
        reset(); setWord(3468, 2); setWord(3476, 3); socketOpen = 1;
        setRow(0, 0x10, 1, 4000, 0, 20000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRow(1, 0x20, 1, 4100, 0, 20000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        std::string pingA = "isconn:1;printf:Server sending ping packet to client, state = 0;sprintf:3/0;";
        (void)pingA;
    }
    // J. state 3 launch, state 4 cancel
    {
        reset(); setWord(3468, 3); setWord(3476, 3); frame_counter = 1; socketOpen = 1; setTime(9664, 5001);
        setRow(0, 0x10, 1, 4000, 0, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        run("state 3 launch", head + "name;sprintf:cli.host;sprintf:10.9.9.9;sprintf:hostA;sprintf:ipA;sprintf:hostB;sprintf:ipB;finish;reset;" + recvTail + tail, 2);
        bool g = g_mode == 1 && g_numClients == 3 && std::string(g_ips) == "cli.host" && std::string(g_ips + 24) == "hostA" && std::string(g_ips + 48) == "hostB" &&
                 g_ports[0] == 2222 && g_ports[1] == 4000 && g_ports[2] == 4100 && std::string(g_localClientIps) == "10.9.9.9" &&
                 std::string(g_localClientIps + 24) == "ipA" && std::string(g_localClientIps + 48) == "ipB" && g_localClientPorts[0] == 3000 &&
                 g_localClientPorts[1] == 5 && g_localClientPorts[2] == 6 && getWord(3468) == 0;
        if (!g) { ok = false; std::fputs("state 3 globals\n", stdout); }
        reset(); setWord(3468, 3); setWord(3476, 3); frame_counter = 1; setTime(9664, 5000);
        run("state 3 too early", head + "name;isconn:0;" + tail, 2);
        reset(); setWord(3468, 4); setWord(3476, 1); frame_counter = 1; setTime(9664, 10);
        run("state 4 alone", head + "name;time;reset;goback;cancel:16,0;isconn:0;" + tail, 2);
        reset(); setWord(3468, 4); setWord(3476, 2); frame_counter = 1; setTime(9664, 5001);
        run("state 4 late", head + "name;time;reset;goback;cancel:16,0;isconn:0;" + tail, 2);
        reset(); setWord(3468, 4); setWord(3476, 2); frame_counter = 1; setTime(9664, 5000);
        run("state 4 waiting", head + "name;time;isconn:0;" + tail, 2);
    }

    // K. datagrams
    auto setRecv = [](unsigned type, unsigned length, int port, const Bytes_& from) {
        socketOpen = 1; recvLength = length; recvPort = port;
        for (int i = 0; i < 20; ++i) recvFrom[i] = from[i];
        for (int i = 0; i < 1024; ++i) recvBuffer[i] = 0;
        recvBuffer[0] = static_cast<unsigned char>(type);
    };
    auto putBuf = [](unsigned at, const std::string& t) { for (size_t i = 0; i <= t.size(); ++i) recvBuffer[at + i] = i < t.size() ? static_cast<unsigned char>(t[i]) : 0; };
    auto got = [](int port, unsigned length) { return "printf:client received packet from server, port = " + std::to_string(port) + "\n;printf:ret returned " + std::to_string(length) + "\n;"; };
    auto setApp2 = [](unsigned v) { norrath_aoc::store_word(arena_at(APP2), v); };
    {
        reset(); setApp2(2); setRecv(7, 8, 1234, addrOf(0x50, 0));
        run("reply 13", head + "name;" + recvTail + "send1:" + H(addrOf(0x50, 0)) + ",1234,0d000000,4,00;" + tail);
        reset(); setApp2(2); setRecv(7, 0, 1234, addrOf(0x50, 0));
        run("empty datagram", head + "name;" + recvTail + tail);
        reset(); setApp2(1); setRecv(13, static_cast<unsigned>(-1), 4000, addrOf(0x10, 1));
        run("negative length", head + "name;" + recvTail + tail);
        // type 13 removes the matching row
        reset(); setWord(3476, 3); setApp2(1);
        setRow(0, 0x10, 1, 4000, 0, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11); setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        setRecv(13, 8, 4000, addrOf(0x10, 1));
        run("type 13 match", head + "name;" + recvTail + got(4000, 8) + "update:2;" + tail);
        if (getWord(6112) != 4100 || getWord(3476) != 2) { ok = false; std::fputs("type 13 words\n", stdout); }
        reset(); setWord(3476, 3); setApp2(1);
        setRow(0, 0x10, 1, 4000, 0, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11); setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        setRecv(13, 8, 4001, addrOf(0x10, 1));
        run("type 13 other port", head + "name;" + recvTail + got(4001, 8) + tail);
        reset(); setWord(3476, 3); setApp2(1);
        setRow(0, 0x10, 1, 4000, 0, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11); setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        setRecv(13, 8, 4000, addrOf(0x10, 0));
        run("type 13 other flag word", head + "name;" + recvTail + got(4000, 8) + tail);
    }
    {
        // type 11: ping answered by a known row
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRecv(11, 100, 4000, addrOf(0x10, 0)); putBuf(4, "ipA"); putBuf(28, "dsc"); recvBuffer[52] = 7; putBuf(56, "Newname"); putBuf(80, "Rogue"); recvBuffer[104] = 42;
        run("ping direct match", head + "name;" + recvTail + got(4000, 100) + "printf:!!!!received ping from local ip ipA!!!!\n;printf:\tdiscoveredIp = dsc\n;printf:\tdiscoveredPort = 7\n;time;" + tail);
        bool pg = getWord(6136) == 8 && getWord(6176) == 3000 && std::string(reinterpret_cast<char*>(O() + 6152)) == "ipA" && std::string(reinterpret_cast<char*>(O() + 6180)) == "Newname" &&
                  std::string(reinterpret_cast<char*>(O() + 6204)) == "Rogue" && getWord(6228) == 42 && getWord(6144) == static_cast<unsigned>(clockNow & 0xffffffff);
        if (!pg) { ok = false; std::fputs("ping direct words\n", stdout); }
        // too short to carry the player block
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 8, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRecv(12, 50, 4000, addrOf(0x10, 0)); putBuf(4, "ipA"); putBuf(56, "Newname");
        run("ping short", head + "name;" + recvTail + got(4000, 50) + "printf:!!!!received ping from local ip ipA!!!!\n;printf:\tdiscoveredIp = \n;printf:\tdiscoveredPort = 0\n;time;" + tail);
        if (std::string(reinterpret_cast<char*>(O() + 6180)) != "Bob" || getWord(6136) != 8) { ok = false; std::fputs("ping short words\n", stdout); }
        // row without a text: matched on address and port alone
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "", "hostA", 5, "Bob", "Mage", 11);
        setRecv(11, 60, 4000, addrOf(0x10, 0)); putBuf(4, "ipZ");
        run("ping no text", head + "name;" + recvTail + got(4000, 60) + "printf:!!!!received ping from local ip ipZ!!!!\n;printf:\tdiscoveredIp = \n;printf:\tdiscoveredPort = 0\n;time;" + tail);
        if (std::string(reinterpret_cast<char*>(O() + 6152)) != "ipZ" || getWord(6136) != 8) { ok = false; std::fputs("ping no text words\n", stdout); }
        // matched through the discovered (host, port): the address and port are updated
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "", "hostA", 5, "Bob", "Mage", 11);
        setRecv(11, 100, 5555, addrOf(0x90, 0)); putBuf(4, "ipZ"); putBuf(28, "hostA"); recvBuffer[52] = 0xa0; recvBuffer[53] = 0x0f;   // discovered port 4000
        run("ping discovered", head + "name;" + recvTail + got(5555, 100) + "printf:!!!!received ping from local ip ipZ!!!!\n;printf:\tdiscoveredIp = hostA\n;printf:\tdiscoveredPort = 4000\n;time;" + tail);
        Bytes_ moved = addrOf(0x90, 0);
        bool dd = getWord(6112) == 5555 && getWord(6136) == 8 && getWord(6176) == 3000 && memcmp(O() + 6116, moved.data(), 20) == 0;
        if (!dd) { ok = false; std::fputs("ping discovered words\n", stdout); }
        // unknown host, but a row with the same 16 address bytes and another port: only the port moves
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRecv(12, 100, 5000, addrOf(0x10, 0)); putBuf(4, "ipQ"); putBuf(28, "other"); putBuf(56, "Q");
        run("ping port move", head + "name;" + recvTail + got(5000, 100) + "printf:!!!!received ping from local ip ipQ!!!!\n;printf:\tdiscoveredIp = other\n;printf:\tdiscoveredPort = 0\n;time;" + tail);
        if (getWord(6112) != 5000 || std::string(reinterpret_cast<char*>(O() + 6152)) != "ipQ" || getWord(6136) != 8) { ok = false; std::fputs("ping port move words\n", stdout); }
        // nothing matches
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRecv(11, 100, 4000, addrOf(0x99, 0)); putBuf(4, "ipQ"); putBuf(28, "other");
        run("ping unknown", head + "name;" + recvTail + got(4000, 100) + "printf:!!!!received ping from local ip ipQ!!!!\n;printf:\tdiscoveredIp = other\n;printf:\tdiscoveredPort = 0\n;" + tail);
    }
    {
        // type 7: the server answers
        reset(); setWord(3468, 6); setApp2(2);
        { Bytes_ f = addrOf(0x50, 0); f[4] = 5; f[5] = 0; f[6] = 168; f[7] = 192; setRecv(7, 8, 4000, f); }
        putBuf(4, "ipN");
        run("type 7 same NAT", head + "name;" + recvTail + got(4000, 8) + "time;sprintf:192.168.0.5;printf:Received message from machine behind the same NAT, localIP = ipN\n;resolve:3376,ipN,1000;"
                                  "printf:Received REALM_SERVER_CLIENT_INITIAL\n;get:0,1842,77;ssvw:0,ServerStatus=1842,1;" + tail);
        if (getWord(3468) != 12 || getWord(3372) != 3000) { ok = false; std::fputs("type 7 NAT words\n", stdout); }
        reset(); setWord(3468, 6); setApp2(2);
        { Bytes_ f = addrOf(0x50, 0); f[4] = 1; f[5] = 2; f[6] = 3; f[7] = 4; setRecv(7, 8, 4000, f); }
        run("type 7 other network", head + "name;" + recvTail + got(4000, 8) + "time;sprintf:4.3.2.1;printf:Received REALM_SERVER_CLIENT_INITIAL\n;get:0,1842,77;ssvw:0,ServerStatus=1842,1;" + tail);
        Bytes_ f = addrOf(0x50, 0); f[4] = 1; f[5] = 2; f[6] = 3; f[7] = 4;
        if (getWord(3468) != 12 || getWord(3372) != 4000 || memcmp(O() + 3376, f.data(), 20) != 0) { ok = false; std::fputs("type 7 other words\n", stdout); }
        reset(); setWord(3468, 6); setApp2(2);
        { Bytes_ g = addrOf(0x50, 0); g[4] = 1; g[5] = 2; g[6] = 3; g[7] = 4; setRecv(7, 8, 3000, g); }
        run("type 7 port 3000", head + "name;" + recvTail + got(3000, 8) + "time;sprintf:4.3.2.1;printf:Received REALM_SERVER_CLIENT_INITIAL\n;get:0,1842,77;ssvw:0,ServerStatus=1842,1;" + tail);
        if (getWord(3372) != 2222) { ok = false; std::fputs("type 7 3000 words\n", stdout); }
    }
    {
        // type 8: roster for the client page
        reset(); setWord(3468, 6); setApp2(2); pageName = "GameClnt";
        setRecv(8, 236, 4000, addrOf(0x50, 0)); recvBuffer[28] = 'A'; recvBuffer[30] = 'B';
        putBuf(32, "Ann"); putBuf(56, "Elf"); recvBuffer[80] = 3;
        putBuf(136, "Cy"); putBuf(160, "Orc"); recvBuffer[184] = 5;
        putBuf(188, "Di"); putBuf(212, "Imp"); recvBuffer[236 - 0 - 0] = 0;
        std::string want = head + "name;" + recvTail + got(4000, 236) + "time;sprintf:A / B;name;";
        const char* names[4] = {"Ann", "", "Cy", "Di"}; const char* classes[4] = {"Elf", "", "Orc", "Imp"}; int levels[4] = {3, 0, 5, 0};
        for (int p = 1; p <= 4; ++p) {
            const std::string n = std::to_string(p);
            want += "sprintf:Player" + n + "Name;sprintf:Player" + n + "Class;sprintf:Player" + n + "Level;sprintf:L" + std::to_string(levels[p - 1]) + ";";
            if (names[p - 1][0]) want += ssv("Player" + n + "Name", names[p - 1]) + ssv("Player" + n + "Class", classes[p - 1]) + ssv("Player" + n + "Level", "L" + std::to_string(levels[p - 1]));
            else want += ssv("Player" + n + "Name", ".........") + ssv("Player" + n + "Class", "......") + ssv("Player" + n + "Level", "...");
        }
        want += "printf:Received REALM_SERVER_CLIENT_RECEIVED_PACKET\n;get:0,1843,77;ssvw:0,ServerStatus=1843,1;ssv:0,NumPlayers=A / B,0;" + tail;
        // the fourth entry (bytes 188..239) has a name but the level word is zero
        run("type 8 roster", want);
        if (getWord(3468) != 12) { ok = false; std::fputs("type 8 state\n", stdout); }
        reset(); setWord(3468, 6); setApp2(2);
        setRecv(8, 236, 4000, addrOf(0x50, 0)); recvBuffer[28] = 'A'; recvBuffer[30] = 'B';
        run("type 8 other page", head + "name;" + recvTail + got(4000, 236) + "time;sprintf:A / B;name;printf:Received REALM_SERVER_CLIENT_RECEIVED_PACKET\n;get:0,1843,77;ssvw:0,ServerStatus=1843,1;ssv:0,NumPlayers=A / B,0;" + tail);
    }
    {
        // type 9: the host sends the start-game roster
        reset(); setWord(3468, 12); setApp2(2); setAddr(3376, 0x40, 1); setWord(3392, 1); frame_counter = 1;
        setRecv(9, 236, 3000, addrOf(0x40, 1)); recvBuffer[4] = 2; putBuf(8, "1.1.1.1"); putBuf(32, "2.2.2.2"); recvBuffer[104] = 101; recvBuffer[108] = 102;
        putBuf(120, "3.3.3.3"); putBuf(144, "4.4.4.4"); recvBuffer[216] = 201; recvBuffer[220] = 202; recvBuffer[232] = 1;
        run("type 9 from host", head + "name;isconn:1;isconn:1;" + recvTail + got(3000, 236) + "time;sprintf:1.1.1.1;sprintf:3.3.3.3;sprintf:2.2.2.2;sprintf:4.4.4.4;printf:!!g_numClients = 2\n;finish;reset;" + tail, 2);
        bool nine = g_mode == 2 && g_myIndex == 1 && g_numClients == 2 && std::string(g_ips) == "1.1.1.1" && std::string(g_ips + 24) == "2.2.2.2" &&
                    g_ports[0] == 101 && g_ports[1] == 102 && std::string(g_localClientIps) == "3.3.3.3" && std::string(g_localClientIps + 24) == "4.4.4.4" &&
                    g_localClientPorts[0] == 201 && g_localClientPorts[1] == 202 && getWord(3468) == 0;
        if (!nine) { ok = false; std::fputs("type 9 globals\n", stdout); }
        reset(); setWord(3468, 12); setApp2(2); setAddr(3376, 0x40, 1); setWord(3392, 1); frame_counter = 1;
        setRecv(9, 236, 3000, addrOf(0x41, 1)); recvBuffer[4] = 2;
        run("type 9 from stranger", head + "name;isconn:1;isconn:1;" + recvTail + got(3000, 236) + "time;" + tail, 2);
        reset(); setWord(3468, 12); setApp2(2); setAddr(3376, 0x40, 1); setWord(3392, 1); setAddr(0x8b08, 0x41, 1); frame_counter = 1;
        setRecv(9, 236, 3000, addrOf(0x41, 1)); recvBuffer[4] = 0; recvBuffer[232] = 3;
        run("type 9 from second address", head + "name;isconn:1;isconn:1;" + recvTail + got(3000, 236) + "time;printf:!!g_numClients = 0\n;finish;reset;" + tail, 2);
        if (g_myIndex != 3 || g_mode != 2) { ok = false; std::fputs("type 9 second address words\n", stdout); }
        reset(); setWord(3468, 11); setApp2(2); setWord(3392, 1); frame_counter = 1;
        setRecv(9, 236, 3000, addrOf(0x40, 1));
        run("type 9 wrong state", head + "name;isconn:1;isconn:1;" + recvTail + got(3000, 236) + tail, 2);
        // type 10 and 14
        reset(); setWord(3468, 6); setApp2(2); setRecv(10, 8, 3000, addrOf(0x40, 1));
        run("type 10", head + "name;" + recvTail + got(3000, 8) + "time;goback;goback;get:0,1840,77;show:1840,0;get:0,1841,77;settext:1841,0;reset;" + tail);
        if (getWord(3468) != 0 || getWord(9720) != static_cast<unsigned>(clockNow & 0xffffffff)) { ok = false; std::fputs("type 10 words\n", stdout); }
        reset(); setWord(3468, 6); setApp2(1); setRecv(10, 8, 3000, addrOf(0x40, 1));
        run("type 10 wrong app", head + "name;" + recvTail + got(3000, 8) + tail);
        reset(); setApp2(2); setRecv(10, 8, 3000, addrOf(0x40, 1)); setWord(3468, 0);
        run("type 10 state 0 is the reply case", head + "name;" + recvTail + "send1:" + H(addrOf(0x40, 1)) + ",3000,0d000000,4,00;" + tail);
        reset(); setWord(3468, 6); setApp2(2); pageName = "GameClnt"; setRecv(14, 8, 3000, addrOf(0x40, 1));
        run("type 14 on the client page", head + "name;" + recvTail + got(3000, 8) + "name;goback;goback;reset;get:0,1869,77;show:1869,0;get:0,1871,77;settext:1871,0;" + tail);
        if (getWord(3468) != 0) { ok = false; std::fputs("type 14 state\n", stdout); }
        reset(); setWord(3468, 6); setApp2(2); setRecv(14, 8, 3000, addrOf(0x40, 1));
        run("type 14 other page", head + "name;" + recvTail + got(3000, 8) + "name;" + tail);
        reset(); setWord(3468, 6); setApp2(2); setRecv(99, 8, 3000, addrOf(0x40, 1));
        run("unknown type", head + "name;" + recvTail + got(3000, 8) + tail);
    }

    // boundary scenarios that pin the frame-rate gate, the state ranges, the roster copy bounds and the packet-size threshold
    {
        reset(); setWord(3468, 1); setWord(3476, 2); socketOpen = 1; setWord(2344, 1); setWord(9688, 1); setAddr(2328, 0x40, 1); frame_counter = 30;
        run("state 1 frame 30", head + "name;isconn:1;isconn:1;pack:78;send1:" + H(addrOf(0x40, 1)) + ",3333,5051525354555657,8,00;" + recvTail + tail, 31);
        reset(); setWord(3468, 1); setWord(3476, 2); socketOpen = 1; setWord(2344, 1); setWord(9688, 1); setAddr(2328, 0x40, 1); frame_counter = 31;
        run("state 1 frame 31", head + "name;isconn:1;isconn:1;" + recvTail + tail, 32);
        reset(); setWord(3468, 13); socketOpen = 1; frame_counter = 0;
        run("state 13 does nothing", head + "name;" + recvTail + tail);
        reset(); setWord(3468, 10); socketOpen = 1;
        run("state 10 does nothing", head + "name;" + recvTail + tail);
        reset(); setWord(3468, 2); setWord(3476, 2); socketOpen = 1; setWord(3472, 1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRow(1, 0x20, 1, 4100, 7, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);   // not a client yet: count is 2, so only row 0 counts
        Bytes_ ping(256, 0); putWord(ping, 0, 7); putText(ping, 4, "10.9.9.9"); putText(ping, 28, "2/1");
        putText(ping, 32, "Hero"); putText(ping, 56, "Warrior"); putWord(ping, 80, 37);
        putText(ping, 84, "Bob"); putText(ping, 108, "Mage"); putWord(ping, 132, 11);
        run("one client", head + "name;isconn:1;printf:Server sending ping packet to client, state = 7\n;sprintf:2/1;send3:" + H(addrOf(0x10, 1)) + ",4000," + H(addrOf(0x70, 0)) +
                              ",ipA,3000," + H(ping) + ",256,00;" + recvTail + tail);
        // removal moves exactly the rows after the removed one
        reset(); setWord(3468, 2); setWord(3476, 3); socketOpen = 1; resolveWorks = 0;
        setRow(0, 0x10, 0, 4000, 0, 20001, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRow(1, 0x20, 1, 4100, 0, 1000, "ipB", "hostB", 6, "Cy", "Monk", 12);
        run("row timeout keeps later rows", head + "name;resolve:6116,hostA,5000;isconn:1;update:2;" + recvTail + tail);
        if (getWord(6112) != 4100 || getWord(1144 + 6112) != 4100 || getWord(1144 + 6228) != 12) { ok = false; std::fputs("row timeout later row\n", stdout); }
        // a ping of exactly 51 bytes carries the player block
        reset(); setWord(3476, 2); setApp2(1);
        setRow(0, 0x10, 1, 4000, 7, 1000, "ipA", "hostA", 5, "Bob", "Mage", 11);
        setRecv(11, 51, 4000, addrOf(0x10, 0)); putBuf(4, "ipA"); putBuf(56, "Newname");
        run("ping 51", head + "name;" + recvTail + got(4000, 51) + "printf:!!!!received ping from local ip ipA!!!!\n;printf:\tdiscoveredIp = \n;printf:\tdiscoveredPort = 0\n;time;" + tail);
        if (std::string(reinterpret_cast<char*>(O() + 6180)) != "Newname") { ok = false; std::fputs("ping 51 words\n", stdout); }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
