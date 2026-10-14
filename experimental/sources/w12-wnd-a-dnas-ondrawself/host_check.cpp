// Host check only; not a build input. The window, context and UI object live in the arena. Callees log; the
// expected log (including the text buffer contents) comes from a separate model of the numbered step list in the
// source comment over state, code and sub-state words, the dot-animation counter (including the reset above 120
// and the 30/60/90 thresholds), the icon id word and the line count.
#define NORRATH_HOST_ARENA 1
#include "ondrawself.cpp"
#include "../w12-wnd-a-dnas-locals/ellipse_count.cpp"
#include "../w16-wnd-a-common/host_world.hpp"
#include <vector>

unsigned _5VIWnd$mUI;
static const unsigned self_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x8000, ctx_g = hw::BASE + 0x9000, raster_g = hw::BASE + 0x20000, dict_g = hw::BASE + 0x21000;
static std::string trace;
static unsigned short strings[3000][8];
static unsigned short error_text[4] = {'E', 'R', 'R', 0};
static int line_count;
static std::string R(const void* p) { return p ? std::to_string(hw::guest(p) - hw::BASE) : "0"; }
static std::string Rg(unsigned g) { return std::to_string(g - hw::BASE); }
static std::string u(const unsigned short* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return std::to_string(v); }
static std::string N(long v) { return std::to_string(v); }
static unsigned sum(const char* s) { unsigned v = 0; for (; *s; ++s) v += static_cast<unsigned char>(*s); return v; }

extern "C" unsigned VIHashResourceID_Icon__FPCc(const char* name) { trace += std::string("HI:") + name + ";"; return sum(name); }
extern "C" void FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dict, unsigned hash, int type, int* out) {
    trace += "FT" + R(dict) + "," + N(hash) + "," + N(type) + "," + R(out) + ";";
    *out = 4242;
}
extern "C" void Begin2D__4VIUI(unsigned ui) { trace += std::string("B") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void End2D__4VIUI(unsigned ui) { trace += std::string("E") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void DrawFakeSplashInterface__9VIWndDnas(void* s) { trace += "DF" + R(s) + ";"; }
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    if (hw::guest(table) != ui_g || arg != 55) trace += "BADSTR;";
    if (!strings[id][0]) { std::string s = "t" + N(id); for (size_t i = 0; i < s.size(); ++i) strings[id][i] = static_cast<unsigned short>(s[i]); }
    return strings[id];
}
extern "C" void strcpy16__8VIStringPUsUiPCUs(unsigned short* d, unsigned size, const unsigned short* s) {
    trace += "SY:" + N(size) + "," + u(s) + ";";
    std::string copy = u(s);
    for (size_t i = 0; i <= copy.size(); ++i) d[i] = i < copy.size() ? static_cast<unsigned short>(copy[i]) : 0;
}
extern "C" void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned size, const unsigned short* s) {
    trace += "SC:" + N(size) + "," + u(s) + ";";
    unsigned n = 0; while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = s[i]; if (!s[i]) break; }
}
extern "C" void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "SD:" + N(size) + ",'" + s + "';";
    unsigned n = 0; while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = static_cast<unsigned char>(s[i]); if (!s[i]) break; }
}
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "S8:" + N(size) + ",'" + s + "';";
    unsigned i = 0; for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]); d[i] = 0;
}
extern "C" int snprintf16__8VIStringPUsiPCUse(unsigned short* d, int size, const unsigned short* format, ...) {
    trace += "SNU:" + N(size) + (format == d ? ",same" : ",other") + ";";
    return 0;  // the format is the buffer itself, so the buffer is unchanged
}
extern "C" int snprintf16__8VIStringPUsiPCce(unsigned short* d, int size, const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const unsigned short* argument = __builtin_va_arg(args, const unsigned short*);
    __builtin_va_end(args);
    trace += "SNC:" + N(size) + ",'" + format + "'," + (argument == d ? "same" : "other") + ";";
    std::string out;
    for (const char* f = format; *f; ++f) { if (f[0] == '%' && f[1] == 'S') { out += u(argument); ++f; } else out += *f; }
    for (size_t i = 0; i <= out.size(); ++i) d[i] = i < out.size() ? static_cast<unsigned short>(out[i]) : 0;
    return 0;
}
extern "C" const unsigned short* DnasErrorString__9VIWndDnasP14sceDNAS2Status(void* self, void* status) {
    trace += "DE" + R(self) + "," + R(status) + ";";
    return error_text;
}
extern "C" int CountLines__10VIUIWindowPCUsi(void* w, const unsigned short* t, int width) {
    trace += "CL" + R(w) + ",'" + u(t) + "'," + N(width) + ";";
    return line_count;
}
extern "C" int Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* w, void* ui, const Pair* p, const Pair* s, int a, int b, int c) {
    trace += "IN" + R(w) + "," + (hw::guest(ui) == ui_g ? "ui" : "?") + "," + N(p->a) + "/" + N(p->b) + "," + N(s->a) + "/" + N(s->b) + "," + N(a) + "," + N(b) + "," + N(c) + ";";
    return 0;
}
extern "C" void Draw__10VIUIWindow(void* w) { trace += "DW" + R(w) + ";"; }
extern "C" void Blit__8VIRasteriUiRC8VIVect2I16VIAlphaBlendModef(void* raster, int id, unsigned flags, const Pair* p, int mode, float scale) {
    trace += "BR" + R(raster) + "," + N(id) + "," + N(flags) + "," + N(p->a) + "/" + N(p->b) + "," + N(mode) + "," + fbits(scale) + ";";
}
extern "C" void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* w, const unsigned short* t, int a, int b, int c, int d, int color, unsigned flags) {
    trace += "TB" + R(w) + ",'" + u(t) + "'," + N(a) + "," + N(b) + "," + N(c) + "," + N(d) + "," + N(color) + "," + N(flags) + ";";
}
extern "C" void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* w, const unsigned short* t, int x, int y, int color, unsigned flags) {
    trace += "TX" + R(w) + ",'" + u(t) + "'," + N(x) + "," + N(y) + "," + N(color) + "," + N(flags) + ";";
}
static unsigned& W(unsigned off) { return hw::w(self_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    const std::string win = Rg(self_g + 784), self_r = Rg(self_g);
    const float scale = 0.625f;
    const unsigned starts[] = {0, 28, 29, 58, 59, 88, 89, 118, 119, 120, 500};
    for (int start : starts) for (int icon : {-1, 7, -5, 0}) for (int state = 0; state <= 11; ++state) for (int code : {0, -9999, 55}) for (int sub : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 40, -1})
    for (int lines : {1, 4}) {
        if ((state != 1 && state != 2 && state != 3 && state != 4) && sub != 0) continue;  // sub-state only matters for states 2-4
        hw::clear();
        std::memset(strings, 0, sizeof strings);
        _5VIWnd$mUI = ui_g;
        hw::w(ui_g + 48) = 55;
        hw::w(ctx_g + 19356) = raster_g; hw::w(ctx_g + 19352) = dict_g;
        W(776) = ctx_g; W(780) = static_cast<unsigned>(icon);
        W(1152) = static_cast<unsigned>(state); W(1156) = static_cast<unsigned>(code); W(1160) = static_cast<unsigned>(sub); W(1168) = 0x1234;
        W(784 + 20) = 700; W(784 + 12) = 100; W(784 + 16) = 60; W(784 + 24) = 300;
        std::memcpy(hw::at(self_g + 784 + 40), &scale, 4);
        norrath_dnas_ellipse_count = start;
        line_count = lines;
        unsigned char before[0x1000];
        std::memcpy(before, hw::at(self_g), sizeof before);
        trace.clear();
        OnDrawSelf__9VIWndDnas(hw::at(self_g));

        std::string want;
        int counter = start + 1;
        int exp_icon = icon;
        if (icon == -1) { want += "HI:dnas_64;FT" + Rg(dict_g) + "," + N(sum("dnas_64")) + ",1," + Rg(self_g + 780) + ";"; exp_icon = 4242; }
        want += "B;DF" + self_r + ";";
        std::string buf = "t803 ";
        want += "SY:1024,t803;SD:1024,' ';";
        bool show = false, error = false;
        int append = 0;
        auto cat = [&](int id) { want += "SC:1024,t" + N(id) + ";"; buf += "t" + N(id); };
        if (state == 0 && code == 0) { show = true; append = 777; }
        else {
            switch (static_cast<unsigned>(state - 1)) {
            case 0: append = 762; break;
            case 1: case 2: case 3: {
                show = true;
                if (static_cast<unsigned>(sub) >= 9) break;
                switch (sub) {
                case 0: append = 775; break;
                case 1: {
                    cat(758);
                    int c = counter;
                    if (c > 120) { counter = 0; c = 0; }
                    if (c < 30) want += "SNU:1024,same;";
                    else if (c < 60) { want += "SNC:1024,'%S.',same;"; buf += "."; }
                    else if (c < 90) { want += "SNC:1024,'%S..',same;"; buf += ".."; }
                    else { want += "SNC:1024,'%S...',same;"; buf += "..."; }
                    break;
                }
                case 2: append = 748; break;
                case 3: show = false; append = 749; break;
                case 5: append = 766; break;
                case 7: append = 765; break;
                case 8: append = 774; break;
                default: break;
                }
                break;
            }
            case 4: append = 764; break;
            case 5: case 6:
                error = true;
                want += "DE" + self_r + "," + Rg(self_g + 1152) + ";SY:1024,ERR;";
                buf = "ERR";
                break;
            default: break;
            }
        }
        if (append) cat(append);
        if (error) show = false;
        const int extra = show ? 150 : 40;
        want += "CL" + win + ",'" + buf + "'," + N(700 - extra - 40) + ";";
        want += "IN" + win + ",ui,0/0,320/246,470," + N(error ? lines + 3 : show ? 10 : lines + 2) + ",0;DW" + win + ";";
        if (exp_icon >= 0 && show) want += "BR" + Rg(raster_g) + "," + N(exp_icon) + ",0," + N(100 - 350 + 20) + "/" + N(60 - 150 + 13) + ",3," + fbits(scale) + ";";
        if (show) want += "TB" + win + ",'" + buf + "'," + N(extra) + ",3," + N(700 - extra - 40) + ",4,0,0;TB" + win + ",'t1857',40,8,620,2,0,8;";
        else want += "TB" + win + ",'" + buf + "'," + N(extra) + ",1," + N(700 - extra - 40) + "," + N(lines) + ",0,0;";
        if (error) {
            want += "S8:1024,'|x ';SC:1024," + std::string(code == -9999 ? "t786" : "t760") + ";";
            want += "TX" + win + ",'|x " + (code == -9999 ? "t786" : "t760") + "',40," + N(lines + 2) + ",0,0;";
        }
        want += "E;";
        if (trace != want) {
            std::fputs(("MISMATCH start=" + N(start) + " icon=" + N(icon) + " state=" + N(state) + " code=" + N(code) + " sub=" + N(sub) + " lines=" + N(lines) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            return 1;
        }
        if (norrath_dnas_ellipse_count != counter) ok = false;
        if (static_cast<int>(W(780)) != exp_icon || W(1168) != (error ? 1u : 0u)) ok = false;
        for (unsigned i = 0; i < sizeof before; ++i) {
            const bool known = (i >= 780 && i < 784) || (i >= 1168 && i < 1172);
            if (!known && hw::at(self_g)[i] != before[i]) ok = false;
        }
        ++scenarios;
        if (!ok) { std::fputs("state mismatch\n", stdout); return 1; }
    }
    std::puts(("ok (" + std::to_string(scenarios) + " scenarios)").c_str());
    return 0;
}
