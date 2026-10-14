// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onkeydown.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[1400];
alignas(8) static unsigned char ui_object[128];
alignas(8) static unsigned char options[160];
static unsigned char device[8];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(ui_object));
static std::string trace;
static int select_result, back_result;
static std::string off(const void* p) {
    const long d = static_cast<const unsigned char*>(p) - object;
    return d >= 0 && d < 1400 ? std::to_string(d) : "?";
}

extern "C" {
unsigned char* VIGetGameOptions__Fv() { trace += "opt;"; return options; }
void Decrement__12VIWndOptions(void*) { trace += "dec;"; }
void Increment__12VIWndOptions(void*) { trace += "inc;"; }
void Prev__12VIWndOptions(void*) { trace += "prev;"; }
void Next__12VIWndOptions(void*) { trace += "next;"; }
int Select__12VIWndOptions(void*) { trace += "select;"; return select_result; }
int Back__12VIWndOptions(void*) { trace += "back;"; return back_result; }
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* w, const unsigned short* a, const unsigned short* b, int prompt, const int* size, int n) {
    trace += "setup:" + off(w) + "," + std::to_string(reinterpret_cast<unsigned long>(a)) + "," + std::to_string(reinterpret_cast<unsigned long>(b)) + "," +
             std::to_string(prompt) + "," + std::to_string(size[0]) + "," + std::to_string(size[1]) + "," + std::to_string(n) + ";";
}
const unsigned short* GetString__13VIStringTableii(const void* t, int id, int col) {
    trace += std::string("gs:") + (t == ui_object ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(col) + ";";
    return reinterpret_cast<const unsigned short*>(id);
}
void SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(void* d, int c, float v) {
    trace += std::string("vol:") + (d == device ? "dev" : "?") + "," + std::to_string(c) + "," + std::to_string(v) + ";";
}
void SetFocus__5VIWndP5VIWndb(void* s, void* t, bool f) { trace += "focus:" + off(s) + "," + off(t) + "," + std::to_string(f) + ";"; }
}

static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }
static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }
static void reset() {
    std::memset(object, 0, sizeof object);
    for (int i = 0; i < 160; ++i) options[i] = static_cast<unsigned char>(i * 7 + 3);
    std::memset(ui_object, 0, sizeof ui_object);
    *reinterpret_cast<unsigned*>(ui_object + 48) = 3;
    *reinterpret_cast<unsigned*>(ui_object + 76) = static_cast<unsigned>(reinterpret_cast<unsigned long>(device));
    trace.clear();
}

int main() {
    bool ok = true;
    const struct { int key; const char* want; } simple[] = {{17, "dec;"}, {136, "dec;"}, {18, "inc;"}, {137, "inc;"}, {19, "prev;"}, {138, "prev;"},
                                                           {20, "next;"}, {139, "next;"}, {0, ""}, {21, ""}, {28, ""}, {131, ""}, {140, ""}};
    for (const auto& c : simple) {
        reset(); OnKeyDown__12VIWndOptionsi(object, c.key); ok = ok && trace == std::string("opt;") + c.want;
    }
    reset(); set_word(1184, 1); OnKeyDown__12VIWndOptionsi(object, 17); ok = ok && trace == "opt;";
    reset(); set_word(1188, 1); OnKeyDown__12VIWndOptionsi(object, 17); ok = ok && trace == "opt;";  // the 64-bit value at +1184 includes +1188
    reset(); set_word(1192, 1); set_word(1188, 5); OnKeyDown__12VIWndOptionsi(object, 17); ok = ok && trace == "opt;" && word(1192) == 1 && word(1188) == 5;
    for (int key : {13, 134}) {
        reset(); set_word(1192, 1); OnKeyDown__12VIWndOptionsi(object, key); ok = ok && trace == "opt;" && word(1192) == 0 && word(1188) == 0;
        for (int r : {0, 1, 2}) {
            reset(); select_result = r; set_word(52, 11); set_word(56, 22);
            OnKeyDown__12VIWndOptionsi(object, key);
            const std::string want = std::string("opt;select;") + (r == 1 ? "gs:ui,1633,3;gs:ui,1634,3;setup:1232,1633,1634,2,11,22,300;focus:0,1232,1;" : "");
            ok = ok && trace == want && word(1260) == (r == 1 ? 1 : 0);
        }
    }
    for (int key : {27, 132}) {
        for (int r : {0, 1}) {
            reset(); back_result = r; set_word(1156, 0x1234); object[1160] = 5; object[1161] = 6; object[1162] = 7; object[1172] = 8; object[1173] = 0;
            float a = 0.25f, b = 0.75f; std::memcpy(object + 1164, &a, 4); std::memcpy(object + 1168, &b, 4);
            set_word(20, 0x7000); set_word(28, 9);
            OnKeyDown__12VIWndOptionsi(object, key);
            if (r == 0) { ok = ok && trace == "opt;back;" && word(28) == 9; continue; }
            float ga, gb; std::memcpy(&ga, options + 8, 4); std::memcpy(&gb, options + 12, 4);
            ok = ok && trace == "opt;back;vol:dev,0,0.250000;vol:dev,1,0.750000;focus:0,?,1;" && options[6] == 0x34 && options[7] == 5 && options[4] == 6 &&
                 options[5] == 7 && options[145] == 8 && options[146] == 1 && ga == 0.25f && gb == 0.75f && word(28) == 0;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
