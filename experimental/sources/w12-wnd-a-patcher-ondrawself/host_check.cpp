// Host check only; not a build input. The window, UI object and message window live in the arena. Callees log;
// the expected log comes from a separate model of the numbered step list in the source comment over modes
// -2..9, selection, flag words, message text, the prompt request word and the dot-animation byte states
// (including the unsigned-byte wrap). Dot-animation strings are checked through call counts and the final text.
#define NORRATH_HOST_ARENA 1
#include "ondrawself.cpp"
#include "../w12-wnd-a-patcher-locals/dot_state.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned _5VIWnd$mUI;
static const unsigned self_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x8000;
static std::string trace;
static unsigned short message_text[2];
static unsigned short strings[3000][8];
static std::string R(const void* p) { return p ? std::to_string(hw::guest(p) - hw::BASE) : "0"; }
static std::string Rg(unsigned g) { return std::to_string(g - hw::BASE); }
static std::string u(const unsigned short* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return std::to_string(v); }
static std::string N(long v) { return std::to_string(v); }

extern "C" void Begin2D__4VIUI(unsigned ui) { trace += std::string("B") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void End2D__4VIUI(unsigned ui) { trace += std::string("E") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void DrawFakeSplashInterface__12VIWndPatcher(void* s) { trace += "DF" + R(s) + ";"; }
extern "C" int Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, void* ui, const Pair* p, const Pair* s, int a, int b, int c) {
    trace += "IN" + R(w) + "," + (hw::guest(ui) == ui_g ? "ui" : "?") + "," + N(p->a) + "/" + N(p->b) + "," + N(s->a) + "/" + N(s->b) + "," + N(a) + "," + N(b) + "," + N(c) + ";";
    return 0;
}
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    if (hw::guest(table) != ui_g || arg != 55) trace += "BADSTR;";
    if (!strings[id][0]) { std::string s = "t" + N(id); for (size_t i = 0; i < s.size(); ++i) strings[id][i] = static_cast<unsigned short>(s[i]); }
    return strings[id];
}
extern "C" void Open__10VIUIWindowb(void* w, bool f) { trace += "OP" + R(w) + "," + (f ? "1" : "0") + ";"; }
extern "C" void Draw__10VIUIWindow(void* w) { trace += "DW" + R(w) + ";"; }
extern "C" const unsigned short* Message__C16VIWndReadMessage(const void* w) { trace += "MS" + R(w) + ";"; return message_text; }
extern "C" void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* w, const unsigned short* t, int x, int y, int color, unsigned flags) {
    trace += "TX" + R(w) + ",'" + u(t) + "'," + N(x) + "," + N(y) + "," + N(color) + "," + N(flags) + ";";
}
extern "C" void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned flags) {
    trace += "TB" + R(w) + ",'" + u(t) + "'," + N(a) + "," + N(b) + "," + N(c) + "," + N(d) + "," + N(color) + "," + N(flags) + ";";
}
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "S8:" + N(size) + ";";
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
extern "C" void strcpy16__8VIStringPUsUiPCUs(unsigned short* d, unsigned size, const unsigned short* s) {
    trace += "SY:" + N(size) + "," + u(s) + ";";
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = s[i];
    d[i] = 0;
}
extern "C" void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned size, const unsigned short* s) {
    trace += "SC:" + N(size) + ";";
    unsigned n = 0;
    while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = s[i]; if (!s[i]) break; }
}
extern "C" void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "SD:" + N(size) + ",'" + s + "';";
    unsigned n = 0;
    while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = static_cast<unsigned char>(s[i]); if (!s[i]) break; }
}
extern "C" int snprintf16__8VIStringPUsiPCce(unsigned short* d, int size, const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    std::string out;
    for (const char* f = format; *f; ++f) {
        if (f[0] == '%' && f[1] == 'S') { out += u(__builtin_va_arg(args, const unsigned short*)); ++f; }
        else out += *f;
    }
    __builtin_va_end(args);
    trace += "SN:" + N(size) + ",'" + format + "';";
    unsigned i = 0;
    for (; i < out.size(); ++i) d[i] = static_cast<unsigned short>(out[i]);
    d[i] = 0;
    return static_cast<int>(out.size());
}
extern "C" void TickR__10VIUIWindowiib(void* w, int a, int b, bool f) { trace += "TR" + R(w) + "," + N(a) + "," + N(b) + "," + (f ? "1" : "0") + ";"; }
extern "C" void DrawProgress__10VIUIWindowiiif(void* w, int a, int b, int c, float f) { trace += "DP" + R(w) + "," + N(a) + "," + N(b) + "," + N(c) + "," + fbits(f) + ";"; }
extern "C" void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int s) { trace += std::string("SP") + (ui == ui_g ? "" : "?") + N(s) + ";"; }
extern "C" void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* w, const unsigned short* a, const unsigned short* b, int prompt, const Pair* rect, int size) {
    trace += "SU" + R(w) + "," + u(a) + "," + u(b) + "," + N(prompt) + "," + N(rect->a) + "/" + N(rect->b) + "," + N(size) + ";";
}
extern "C" void SetFocus__5VIWndP5VIWndb(void* s, void* w, bool f) { trace += "SF" + R(s) + "," + R(w) + "," + (f ? "1" : "0") + ";"; }
static unsigned& W(unsigned off) { return hw::w(self_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    const float progress = 0.37f;
    const std::string win = Rg(self_g + 776), win2 = Rg(self_g + 408), msgw = Rg(self_g + 2312), self_r = Rg(self_g);
    auto T = [](int id) { return "t" + std::to_string(id); };
    for (int mode = -2; mode <= 9; ++mode) for (int sel : {0, 1, 2}) for (int flag1156 : {0, 1}) for (int has_text : {0, 1}) for (int q1164 : {0, 1})
    for (int dot0 : {0, 1, 2}) for (int delay0 : {0, 1, 2, 15, 127, 128, 129, 255}) {
        hw::clear();
        std::memset(strings, 0, sizeof strings);
        _5VIWnd$mUI = ui_g;
        hw::w(ui_g + 48) = 55;
        W(1148) = static_cast<unsigned>(mode); W(1144) = static_cast<unsigned>(sel); W(1156) = flag1156 ? 6 : 0; W(1160) = 0x31; W(1164) = q1164 ? 9 : 0;
        std::memcpy(hw::at(self_g + 1152), &progress, 4);
        W(776 + 20) = 700; W(408 + 20) = 500; W(788) = 111; W(792) = 222;
        W(1176 + 28) = 0x55;
        message_text[0] = has_text ? 0x4d : 0;
        norrath_patcher_dot = static_cast<signed char>(dot0);
        norrath_patcher_dot_delay = static_cast<unsigned char>(delay0);
        unsigned char before[0x1000];
        std::memcpy(before, hw::at(self_g), sizeof before);
        trace.clear();
        OnDrawSelf__12VIWndPatcher(hw::at(self_g));

        std::string want = "B;DF" + self_r + ";IN" + win + ",ui,0/0,320/246,430,8,0;";
        int exp_sel = sel;
        int exp_dot = dot0, exp_delay = delay0;
        bool exp_flag = false;
        unsigned exp1160 = 0x31, exp1164 = q1164 ? 9 : 0;
        const bool early = mode == -1;
        if (early) { want += ""; }
        else {
            std::string heading;
            switch (mode) {
            case 0: heading = T(757); break;
            case 1: heading = T(754); break;
            case 2: case 7: heading = T(768); break;
            case 3: heading = T(812); break;
            case 5: heading = T(792); break;
            case 6: heading = T(814); break;
            case 4: heading = T(764); want += "IN" + win + ",ui,0/0,320/246,430,9,0;OP" + win + ",1;"; break;
            default: break;
            }
            want += "DW" + win + ";";
            if (mode == 4) {
                want += "MS" + msgw + ";";
                if (has_text) want += "TX" + win + ",'t813',40,5," + N(exp_sel == 0) + ",0;";
                else { want += "S8:1024;SC:1024;TX" + win + ",'|-t813',40,5,0,0;"; exp_sel = 1; }
                want += "TX" + win + ",'" + (flag1156 ? "t793" : "t790") + "',40,6," + N(exp_sel == 1) + ",0;";
                want += std::string("S8:1024;SC:1024;SD:1024,'") + (flag1156 ? "   |-|t " : "   |t ") + "';SC:1024;";
                want += "TX" + win + ",'|x t212" + (flag1156 ? "   |-|t " : "   |t ") + "t750',40,8,0,0;";
                want += "TR" + win + ",36," + N(exp_sel + 5) + ",1;";
            } else {
                want += "TB" + win + ",'t763',40,5,620,2,0,8;";
            }
            exp_delay = static_cast<unsigned char>(delay0 - 1);
            if (static_cast<signed char>(exp_delay) <= 0) { exp_delay = 15; exp_dot = exp_dot + 1; if (exp_dot == 3) exp_dot = 0; }
            std::string final_text;
            if (!heading.empty()) {
                want += "SY:1024," + heading + ";";
                if (mode == 3 || mode == 5 || mode == 6) for (int i = 0; i < exp_dot + 1; ++i) want += "SD:1024,'.';";
                want += "SN:1024,'%S %S';";
                final_text = "t803 " + heading;
            } else {
                want += "SN:1024,'%S';";
                final_text = "t803";
            }
            want += "TB" + win + ",'" + final_text + "',40,1,620,2,0,0;";
            want += "DP" + win + ",40,3,620," + fbits(progress) + ";";
            if (mode == 2 || mode == 7) {
                want += "DW" + win2 + ";TB" + win2 + ",'" + T(mode == 2 ? 769 : 787) + "',40,1,420,3,0,8;S8:1024;SC:1024;TX" + win2 + ",'|x t793',40,5,0,0;";
            }
            if (q1164) {
                exp1164 = 0; exp1160 = 0; exp_flag = true;
                want += "SP15;SU" + Rg(self_g + 1176) + ",t785,t770,0,111/222,500;SF" + self_r + "," + Rg(self_g + 1176) + ",1;";
            }
            want += "E;";
        }
        if (trace != want) {
            std::fputs(("MISMATCH mode=" + N(mode) + " sel=" + N(sel) + " f1156=" + N(flag1156) + " text=" + N(has_text) + " q=" + N(q1164) + " dot=" + N(dot0) + " delay=" + N(delay0) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            return 1;
        }
        if (!early) {
            if (norrath_patcher_dot != exp_dot || norrath_patcher_dot_delay != exp_delay) ok = false;
        } else if (norrath_patcher_dot != dot0 || norrath_patcher_dot_delay != delay0) ok = false;
        if (static_cast<int>(W(1144)) != exp_sel || W(1160) != exp1160 || W(1164) != exp1164) ok = false;
        if (W(1176 + 28) != (exp_flag ? 1u : 0x55u)) ok = false;
        for (unsigned i = 0; i < sizeof before; ++i) {
            const bool known = (i >= 1144 && i < 1148) || (i >= 1160 && i < 1168) || (i >= 1176 + 28 && i < 1176 + 32);
            if (!known && hw::at(self_g)[i] != before[i]) ok = false;
        }
        ++scenarios;
        if (!ok) { std::fputs("state mismatch\n", stdout); return 1; }
    }
    std::puts(("ok (" + std::to_string(scenarios) + " scenarios)").c_str());
    return 0;
}
