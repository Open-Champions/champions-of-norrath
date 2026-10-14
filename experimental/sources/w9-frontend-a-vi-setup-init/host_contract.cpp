#include "init.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static int checked_calls;
static int fail_at = -1;
static unsigned char* object_base;
static void* seen_pad;
static float seen_color[4];

static std::string at(void* pointer) { return std::to_string(static_cast<unsigned char*>(pointer) - object_base); }
static int check(const std::string& name) {
    trace += name + ";";
    return checked_calls++ == fail_at ? -1 : 0;
}

void* sPad;

extern "C" {
int printf(const char* format, ...) {
    trace += std::string("printf:") + format + ";";
    return 0;
}
int Init__5VIPadii(void* pad, int, int) { trace += "pad@" + at(pad) + ";"; return 0; }
int Init__8VIRasterPviii(void* raster, void*, int width, int height, int flag) {
    return check("raster@" + at(raster) + "," + std::to_string(width) + "," + std::to_string(height) + "," + std::to_string(flag));
}
int Init__9VICollide(void* collide) { return check("collide@" + at(collide)); }
int Init__12VIDictionaryi(void* dictionary, int) { return check("dict@" + at(dictionary)); }
int Init__13VISoundDevice(void* sound) { return check("sound@" + at(sound)); }
long GetTime__7VITimer() { trace += "time;"; return 99; }
int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(void* scene, long time, void* dictionary, void* raster,
                                                                        void* sound, void* collide) {
    return check("scene@" + at(scene) + "," + std::to_string(time) + "," + at(dictionary) + "," + at(raster) + "," + at(sound) + "," + at(collide));
}
void* VIGetGameOptions__Fv() { trace += "options;"; return object_base + 1; }
void Load__13VIGameOptions(void* options) { trace += "load@" + at(options) + ";"; }
int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(void* ui, void* raster, void* sound, void* dictionary) {
    return check("ui@" + at(ui) + "," + at(raster) + "," + at(sound) + "," + at(dictionary));
}
void VISetWindowUI__FP4VIUI(void* ui) { trace += "setui@" + at(ui) + ";"; }
void* __10VIESFParse(void* object) { trace += "esf;"; return object; }
void* __9VICSFFile(void* object) { trace += "csf;"; return object; }
int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void*, const char* path, void*, void* scene) {
    return check(std::string("parse:") + path + "@" + at(scene));
}
void _$_10VIESFParse(void*, int flags) { trace += "esfend" + std::to_string(flags) + ";"; }
void VIDisplayLoadingScreen__Fb(bool flag) { trace += "loading" + std::to_string(flag) + ";"; }
void Flush__13VISoundDevice(void* sound) { trace += "flush@" + at(sound) + ";"; }
void SetTimeOfDay__7VIScenef(void* scene, float time) { trace += "tod@" + at(scene) + "," + std::to_string(static_cast<int>(time)) + ";"; }
void SetWireframe__8VIRasteri(void* raster, int value) { trace += "wire@" + at(raster) + "," + std::to_string(value) + ";"; }
void SetClearColor__8VIRasterRC10VIColor32F(void* raster, void* color) {
    trace += "clear@" + at(raster) + ";";
    for (int i = 0; i < 4; ++i) seen_color[i] = static_cast<float*>(color)[i];
}
void VISplashFxInit__FP8VIRasterP12VIDictionary(void* raster, void* dictionary) { trace += "fx@" + at(raster) + "," + at(dictionary) + ";"; }
void Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* manager, void* pad, void* keyboard) {
    seen_pad = pad;
    trace += "wm@" + at(manager) + "," + at(pad) + "," + std::to_string(keyboard == nullptr) + ";";
}
void Init__11VIWndSplashP7VIScene(void* window, void* scene) { trace += "splash@" + at(window) + "," + at(scene) + ";"; }
void SetAppPtr__5VIWndPv(void* window, void* application) { trace += "app@" + at(window) + "," + at(application) + ";"; }
void Init__12VIWndPatcher(void* window) { trace += "patcher@" + at(window) + ";"; }
void Init__9VIWndDnasP7VIScene(void* window, void* scene) { trace += "dnas@" + at(window) + "," + at(scene) + ";"; }
void Init__10VIWndLegal(void* window) { trace += "legal@" + at(window) + ";"; }
void Init__9VIWndEula(void* window) { trace += "eula@" + at(window) + ";"; }
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window) { trace += "root@" + at(manager) + "," + at(window) + ";"; }
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag) {
    trace += "focus@" + at(manager) + "," + at(window) + "," + std::to_string(flag) + ";";
}
void Init__7VIFaderP8VIRasterP12VIDictionaryf(void* fader, void* raster, void* dictionary, float value) {
    trace += "fader@" + at(fader) + "," + at(raster) + "," + at(dictionary) + "," + std::to_string(static_cast<int>(value)) + ";";
}
void Begin__7VIFaderff(void* fader, float from, float to) {
    trace += "begin@" + at(fader) + "," + std::to_string(static_cast<int>(from * 10)) + "," + std::to_string(static_cast<int>(to * 10)) + ";";
}
}

static std::string expected(bool options, int stop_after_checked) {
    // Checked calls in order: raster, collide, dict, sound, scene, ui, then five parses.
    const char* head[] = {"raster@0,640,448,1;", "collide@19552;", "dict@20608;", "sound@20632;",
                          "time;scene@25120,99,20608,0,20632,19552;", nullptr};
    std::string out = "printf:INIT\n;pad@44632;";
    int checked = 0;
    for (const char* piece : head) {
        if (piece == nullptr) {
            if (options) out += "options;load@1;";
            out += "ui@44512,0,20632,20608;";
        } else {
            out += piece;
        }
        if (checked++ == stop_after_checked) return out;
    }
    out += "setui@44512;esf;csf;";
    const char* parses[] = {"parse:data2\\ArialUI.csf@25120;", "parse:data2\\ui.csf@25120;", "parse:data2\\conui.csf@25120;",
                            "parse:data2\\statui.csf@25120;", "parse:data2\\setup.csf@25120;"};
    for (int i = 0; i < 5; ++i) {
        out += parses[i];
        if (i == 0) {
            if (checked++ == stop_after_checked) return out + "esfend2;";
            out += "loading1;";
        } else if (checked++ == stop_after_checked) {
            return out + "esfend2;";
        }
    }
    out += "flush@20632;tod@25120,12;wire@0,0;clear@0;fx@0,20608;wm@44900,44632,1;splash@45032,25120;app@45032,0;patcher@54704;"
           "dnas@73824,25120;legal@86928;eula@77160;root@44900,86928;focus@44900,86928,1;fader@44600,0,20608,1;begin@44600,0,5;esfend2;";
    return out;
}

int main() {
    static unsigned char storage[0x20000];
    object_base = storage;
    for (int options = 0; options < 2; ++options) {
        for (int fail = -1; fail < 10; ++fail) {
            trace.clear();
            checked_calls = 0;
            fail_at = fail;
            seen_pad = nullptr;
            sPad = nullptr;
            storage[0x153c4] = 0;
            const int result = Init__7VISetupi(storage, options * 5);
            std::string want = expected(options != 0, fail);
            if (trace != want) {
                std::puts(trace.c_str());
                std::puts(want.c_str());
                return 1 + fail;
            }
            if (result != (fail < 0 ? 0 : -1)) return 20;
            if (fail < 0) {
                if (sPad != storage + 0xae58 || seen_pad != sPad) return 21;
                if (seen_color[0] != 0.0f || seen_color[1] != 0.0f || seen_color[2] != 0.0f || seen_color[3] != 1.0f) return 22;
                int words[7];
                const unsigned offsets[7] = {0x153c4, 0x153dc, 0x153cc, 0x153c8, 0x153e0, 0x153c0, 0xaf60};
                for (int i = 0; i < 7; ++i) std::memcpy(&words[i], storage + offsets[i], 4);
                const int want_words[7] = {-1, 0, 0, 1, -1, 0, 0};
                for (int i = 0; i < 7; ++i) if (words[i] != want_words[i]) return 23;
            }
        }
    }
    std::puts("vi setup init passed");
    return 0;
}
