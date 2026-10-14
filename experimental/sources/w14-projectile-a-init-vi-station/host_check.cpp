// Host contract for Init__9VIStationiPCcPcT3T2T2i; not an EE input. printf, strcpy and memset are renamed to local functions with the
// same semantics; every other callee is a stub that appends a token to a call log (the checked callees return -1 when it is their turn
// to fail, the others return 0). The expected call order below follows the numbered steps of the source comment. The authored
// function was also compared with the original in a private R5900 run (48 scenarios: every failure position and the four combinations of
// the two optional strings; 314 of 315 original words covered, the rest is a delay-slot nop).
#include <cstdio>
#include <cstring>
#include <string>
#define printf host_printf
#define strcpy host_strcpy
#define memset host_memset
static std::string calls;
static int checked_index, fail_at;
extern "C" int host_printf(const char* format, ...);
extern "C" char* host_strcpy(char* to, const char* from);
extern "C" void* host_memset(void* to, int value, unsigned size);
#include "init.cpp"
#undef printf
#undef strcpy
#undef memset
static unsigned char heap_self[0x18000] __attribute__((aligned(16)));
extern "C" {
int host_printf(const char* format, ...) {
    calls += std::string("printf(") + format + ");";
    return 0;
}
char* host_strcpy(char* to, const char* from) { calls += "strcpy@" + std::to_string(to - reinterpret_cast<char*>(heap_self)) + "=" + from + ";"; std::strcpy(to, from); return to; }
void* host_memset(void* to, int value, unsigned size) {
    if (to >= heap_self && to < heap_self + sizeof heap_self) calls += "memset@" + std::to_string(static_cast<unsigned char*>(to) - heap_self) + "," + std::to_string(size) + ";";
    else calls += "memset(stack," + std::to_string(size) + ");";
    std::memset(to, value, size);
    return to;
}
int sceGsSyncV(int) { calls += "sync;"; return 0; }
static int checked() { return checked_index++ == fail_at ? -1 : 0; }
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - heap_self); }
int Init__5VIPadii(void* pad, int a, int b) { calls += "pad@" + off(pad) + "(" + std::to_string(a) + "," + std::to_string(b) + ");"; return 0; }
void Init__10VIKeyboard(void* keyboard) { calls += "kb@" + off(keyboard) + ";"; }
int Init__8VIRasterPviii(void* raster, void* surface, int width, int height, int flag) {
    calls += "raster@" + off(raster) + "(" + std::to_string(reinterpret_cast<long>(surface)) + "," + std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(flag) + ");";
    return checked();
}
void VIDisplayLoadingScreen__Fb(bool flag) { calls += std::string("loading(") + (flag ? "1" : "0") + ");"; }
int Init__9VICollide(void* collide) { calls += "collide@" + off(collide) + ";"; return checked(); }
int Init__12VIDictionaryi(void* dictionary, int value) { calls += "dict@" + off(dictionary) + "(" + std::to_string(value) + ");"; return checked(); }
int Init__13VISoundDevice(void* sound) { calls += "sound@" + off(sound) + ";"; return checked(); }
long GetTime__7VITimer() { calls += "time;"; return 0x1234; }
int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(void* scene, long time, void* dictionary, void* raster, void* sound, void* collide) {
    calls += "scene@" + off(scene) + "(" + std::to_string(time) + "," + off(dictionary) + "," + off(raster) + "," + off(sound) + "," + off(collide) + ");";
    return checked();
}
int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(void* ui, void* raster, void* sound, void* dictionary) {
    calls += "ui@" + off(ui) + "(" + off(raster) + "," + off(sound) + "," + off(dictionary) + ");"; return checked();
}
void VISetWindowUI__FP4VIUI(void* ui) { calls += "setui@" + off(ui) + ";"; }
void* __10VIESFParse(void* object) { calls += "esfctor;"; return object; }
void* __9VICSFFile(void* object) { calls += "csfctor;"; return object; }
int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void*, const char* path, void*, void* scene) { calls += std::string("parse(") + path + ")@" + off(scene) + ";"; return checked(); }
void _$_10VIESFParse(void*, int flags) { calls += "esfdtor(" + std::to_string(flags) + ");"; }
void SetTimeOfDay__7VIScenef(void* scene, float time) { calls += "tod@" + off(scene) + "(" + std::to_string(static_cast<int>(time)) + ");"; }
void SetWireframe__8VIRasteri(void* raster, int value) { calls += "wire@" + off(raster) + "(" + std::to_string(value) + ");"; }
void SetClearColor__8VIRasterRC10VIColor32F(void* raster, void* color) {
    const float* c = static_cast<const float*>(color);
    calls += "clear@" + off(raster) + "(" + std::to_string(static_cast<int>(c[0])) + "," + std::to_string(static_cast<int>(c[1])) + "," + std::to_string(static_cast<int>(c[2])) + "," + std::to_string(static_cast<int>(c[3])) + ");";
}
void Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* manager, void* pad, void* keyboard) { calls += "wm@" + off(manager) + "(" + off(pad) + "," + off(keyboard) + ");"; }
void SetAppPtr__5VIWndPv(void* window, void* application) { calls += "setapp@" + off(window) + "(" + (application == heap_self ? "self" : "other") + ");"; }
void SetStationName__20VIWndGenericRendererPCc(void* window, const char* name) { calls += "sname@" + off(window) + "(" + name + ");"; }
void SetPassword__20VIWndGenericRendererPCc(void* window, const char* password) { calls += "spw@" + off(window) + "(" + password + ");"; }
void Init__20VIWndGenericRenderer(void* window) { calls += "grinit@" + off(window) + ";"; }
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...) {
    calls += "snprintf16(" + std::to_string(size) + "," + format + ");";
    buffer[0] = 'H'; buffer[1] = 'i'; buffer[2] = 0; return 2;
}
int strlen16__8VIStringPCUs(const unsigned short* text) { calls += "strlen16(" + std::to_string(text[0]) + ");"; return 2; }
void Init__9VIWndEula(void* window) { calls += "eula@" + off(window) + ";"; }
void SetText__9VIWndEulaPCUsi(void* window, const unsigned short* text, int length) { calls += "settext@" + off(window) + "(" + std::to_string(text[0]) + "," + std::to_string(length) + ");"; }
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window) { calls += "setroot@" + off(manager) + "(" + off(window) + ");"; }
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag) { calls += "setfocus@" + off(manager) + "(" + off(window) + "," + (flag ? "1" : "0") + ");"; }
void Init__7VIFaderP8VIRasterP12VIDictionaryf(void* fader, void* raster, void* dictionary, float value) {
    calls += "fader@" + off(fader) + "(" + off(raster) + "," + off(dictionary) + "," + std::to_string(static_cast<int>(value)) + ");";
}
void Begin__7VIFaderff(void* fader, float from, float to) { calls += "begin@" + off(fader) + "(" + std::to_string(static_cast<int>(from * 10)) + "," + std::to_string(static_cast<int>(to * 10)) + ");"; }
}

static const char* const csf_paths[] = {"data2\\ArialUI.csf", "data2\\ui.csf", "data2\\statui.csf", "data2\\pregame.csf", "data2\\conui.csf"};

// the expected log, built from the numbered steps of the source comment; `upto` stops after the checked call number `fail` failed
static std::string expected(int fail, bool d, bool e) {
    std::string s;
    int index = 0;
    auto step = [&](const std::string& t) { s += t; };
    auto check = [&](const std::string& t) { step(t); return index++ == fail; };
    step("strcpy@4=alpha;printf(station::init()\n);sync;sync;strcpy@1028=bravo;strcpy@1284=charlie;printf(%s, %s\n);sync;sync;");
    step("memset@96820,64;memset@96884,64;");
    if (d) step("strcpy@96820=delta;");
    if (e) step("strcpy@96884=echo;");
    step("pad@46192(0,0);kb@46288;printf(Pad/Kb init\n);sync;sync;");
    if (check("raster@1552(0,640,448,0);")) { step("printf(raster init failed\n);"); return s; }
    step("printf(Raster Init\n);sync;sync;loading(0);printf(Loading Screen\n);sync;sync;");
    if (check("collide@21104;")) return s;
    if (check("dict@22160(0);")) return s;
    if (check("sound@22184;")) return s;
    step("time;");
    if (check("scene@26672(4660,22160,1552,22184,21104);")) return s;
    if (check("ui@46064(1552,22184,22160);")) return s;
    step("setui@46064;printf(Here, ui = %i\n);esfctor;csfctor;");
    for (const char* p : csf_paths) {
        if (check(std::string("parse(") + p + ")@26672;")) { step("esfdtor(2);"); return s; }
    }
    step("tod@26672(12);wire@1552(0);clear@1552(0,0,0,1);wm@47000(46192,46288);setapp@47136(self);sname@47136(bravo);spw@47136(charlie);grinit@47136;");
    step("memset(stack,2048);snprintf16(2048,Connecting to SOEGames.net...);setapp@87048(self);eula@87048;strlen16(72);settext@87048(72,2);");
    step("setroot@47000(47136);setfocus@47000(47136,1);fader@46152(1552,22160,1);begin@46152(0,5);esfdtor(2);");
    return s;
}
static int run(int fail, bool d, bool e) {
    std::memset(heap_self, 0x5a, sizeof heap_self);
    calls.clear(); checked_index = 0; fail_at = fail;
    char a[] = "alpha", b[] = "bravo", c[] = "charlie", dd[] = "delta", ee[] = "echo";
    const int r = Init__9VIStationiPCcPcT3T2T2i(heap_self, 77, a, b, c, d ? dd : nullptr, e ? ee : nullptr, 0x55);
    const std::string want = expected(fail, d, e);
    if (calls != want) { std::printf("fail=%d d=%d e=%d\ncalls %s\nwant  %s\n", fail, d, e, calls.c_str(), want.c_str()); return 1; }
    const bool ok = fail < 0 || fail >= 11;
    if (r != (ok ? 0 : -1)) return 2;
    unsigned w0, w1, w2, w3, w4, w5;
    std::memcpy(&w0, heap_self, 4); std::memcpy(&w1, heap_self + 0x17a30, 4); std::memcpy(&w2, heap_self + 0xb81c, 4);
    std::memcpy(&w3, heap_self + 0xb468, 4); std::memcpy(&w4, heap_self + 0xb794, 4); std::memcpy(&w5, heap_self + 0x17ab4, 4);
    if (w0 != 77 || w1 != 0 || w2 != 0 || w5 != 0x55) return 3;
    if (w3 != 1) return 4;   // set once the pad and keyboard are up, before any failure point
    if (ok && w4 != 0) return 5;   // the word at 0xb794 is cleared only after the parses succeed
    return 0;
}
int main() {
    int r;
    for (int fail = -1; fail <= 11; ++fail)
        for (int mask = 0; mask < 4; ++mask)
            if ((r = run(fail, (mask & 1) != 0, (mask & 2) != 0))) return r * 100 + fail + 2;
    return 0;
}
