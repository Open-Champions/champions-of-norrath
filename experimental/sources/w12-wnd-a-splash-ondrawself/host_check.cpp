// Host check only; not a build input. The splash window, UI object, application block and focus word live in
// the arena with a target-layout class table (the message handler is the entry at byte offset 48). Callees
// log; the expected log comes from a separate model of the numbered step list in the source comment over
// pad, previous pad, focus, request words, selection and phase combinations. Hash values are the character sum.
#define NORRATH_HOST_ARENA 1
#include "ondrawself.cpp"
#include "../w12-wnd-a-splash-locals/last_pad_state.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned _5VIWnd$mUI;
unsigned _5VIWnd$mFocus;
static const unsigned self_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x8000, app_g = hw::BASE + 0xa000, mcerr_g = hw::BASE + 0xc000,
                      child_g = hw::BASE + 0xc400, table_g = hw::BASE + 0xc800, raster_g = hw::BASE + 0xcc00, dict_g = hw::BASE + 0xcd00;
static std::string trace;
static std::string R(const void* p) { return p ? std::to_string(hw::guest(p) - hw::BASE) : "0"; }
static std::string Rg(unsigned g) { return g ? std::to_string(g - hw::BASE) : "0"; }
static std::string u(const unsigned short* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static std::string fbits(float f) { unsigned v; std::memcpy(&v, &f, 4); return std::to_string(v); }
static unsigned short strings[3000][8];

extern "C" void* GetAppPtr__5VIWnd(void*) { return hw::at(app_g); }
extern "C" void EndConnect__12VIWndConnect(void* c) { trace += "EC" + R(c) + ";"; }
extern "C" void* McErrWnd__Fv() { return hw::at(mcerr_g); }
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    if (hw::guest(table) != ui_g || arg != 55) trace += "BADSTR;";
    if (!strings[id][0]) { std::string s = "t" + std::to_string(id); for (size_t i = 0; i < s.size(); ++i) strings[id][i] = static_cast<unsigned short>(s[i]); }
    return strings[id];
}
extern "C" void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* w, const unsigned short* a, const unsigned short* b, int prompt, const Pair* rect, int size) {
    trace += "SU" + R(w) + "," + u(a) + "," + u(b) + "," + std::to_string(prompt) + "," + std::to_string(rect->a) + "/" + std::to_string(rect->b) + "," + std::to_string(size) + ";";
}
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* w, bool force) { trace += "SF" + R(self) + "," + R(w) + "," + (force ? "1" : "0") + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void* w) { trace += "RT" + R(w) + ";"; }
extern "C" void RemoveChild__5VIWndP5VIWnd(void* self, void* c) { trace += "RC" + R(self) + "," + R(c) + ";"; }
extern "C" void Begin2D__4VIUI(unsigned ui) { trace += std::string("B") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void End2D__4VIUI(unsigned ui) { trace += std::string("E") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" unsigned VIHashResourceID(const char* name) {
    unsigned sum = 0;
    for (const char* c = name; *c; ++c) sum += static_cast<unsigned char>(*c);
    trace += std::string("H:") + name + ";";
    return sum;
}
extern "C" void Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int id, const Pair* p, int mode, float scale, unsigned flags) {
    trace += std::string("BL") + (ui == ui_g ? "" : "?") + std::to_string(id) + "," + std::to_string(p->a) + "/" + std::to_string(p->b) + "," + std::to_string(mode) + "," + fbits(scale) + "," + std::to_string(flags) + ";";
}
extern "C" void BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int id, const Pair* p, int mode, float scale, unsigned flags) {
    trace += std::string("BX") + (ui == ui_g ? "" : "?") + std::to_string(id) + "," + std::to_string(p->a) + "/" + std::to_string(p->b) + "," + std::to_string(mode) + "," + fbits(scale) + "," + std::to_string(flags) + ";";
}
extern "C" void SetTextColor__6VIFontRC9VIColor32(void* font, const Color32* c) {
    trace += "TC" + std::to_string(hw::guest(font) - ui_g) + "," + std::to_string(c->r) + "." + std::to_string(c->g) + "." + std::to_string(c->b) + "." + std::to_string(c->a) + ";";
}
extern "C" void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void* font, void* raster, void* dict, const unsigned short* text, int x, int y) {
    trace += "DS" + std::to_string(hw::guest(font) - ui_g) + "," + std::to_string(hw::guest(raster)) + "," + std::to_string(hw::guest(dict)) + "," + u(text) + "," + std::to_string(x) + "," + std::to_string(y) + ";";
}
extern "C" void str8to16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "S8:" + std::to_string(size) + ";";
    unsigned i = 0;
    for (; s[i]; ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}
extern "C" void strcat16__8VIStringPUsUiPCUs(unsigned short* d, unsigned size, const unsigned short* s) {
    trace += "SC:" + std::to_string(size) + ";";
    unsigned n = 0;
    while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = s[i]; if (!s[i]) break; }
}
extern "C" void strcat16__8VIStringPUsUiPCc(unsigned short* d, unsigned size, const char* s) {
    trace += "SD:" + std::to_string(size) + ";";
    unsigned n = 0;
    while (d[n]) ++n;
    for (unsigned i = 0;; ++i) { d[n + i] = static_cast<unsigned char>(s[i]); if (!s[i]) break; }
}
extern "C" void TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* w, const unsigned short* text, int a, int b, int c, int d, const Color32F* color, unsigned flags) {
    trace += "TB" + R(w) + ",'" + u(text) + "'," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," +
             fbits(color->r) + "." + fbits(color->g) + "." + fbits(color->b) + "." + fbits(color->a) + "," + std::to_string(flags) + ";";
}
extern "C" void SelectPlay__11VIWndSplash(void* self) { trace += "SP" + R(self) + ";"; }
static void on_message(void* self, void* from, int message, int a, int b) {
    trace += "MSG" + R(self) + "," + R(from) + "," + std::to_string(message) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
static void wrong(void*, void*, int, int, int) { trace += "WRONG;"; }
static unsigned& W(unsigned off) { return hw::w(self_g + off); }

int main() {
    bool ok = true;
    long scenarios = 0;
    const std::string self_r = Rg(self_g), msg_r = Rg(self_g + 4776), win_r = Rg(self_g + 40), conn_r = Rg(self_g + 416);
    for (int pad : {0, 1, 5}) for (int last : {0, 1}) for (int focus_kind : {0, 1, 2}) for (int q9648 : {0, 1}) for (int q9652 : {0, 1})
    for (int q9644 : {0, 1}) for (int sel : {0, 1, 2, -1}) for (int phase : {0, 1, 2, 3}) {
        hw::clear();
        std::memset(strings, 0, sizeof strings);
        for (int slot = 0; slot < 10; ++slot) hw::put_entry(table_g + 8 * slot, 4, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
        hw::put_entry(table_g + 48, 24, 6, reinterpret_cast<void*>(on_message));
        _5VIWnd$mUI = ui_g;
        _5VIWnd$mFocus = focus_kind == 0 ? self_g : focus_kind == 1 ? mcerr_g : child_g;
        norrath_splash_last_pad_state = last;
        hw::w(ui_g + 48) = 55; hw::w(ui_g + 72) = raster_g; hw::w(ui_g + 80) = dict_g;
        hw::w(app_g + 0xaeb0) = static_cast<unsigned>(pad);
        W(32) = table_g; W(408) = static_cast<unsigned>(sel);
        W(9648) = q9648 ? 3 : 0; W(9652) = q9652 ? 4 : 0; W(9644) = q9644 ? child_g : 0; W(9656) = 0x99; W(9660) = static_cast<unsigned>(phase);
        hw::w(self_g + 416 + 28) = 0x77; hw::w(self_g + 4776 + 28) = 0x66;
        unsigned char before[0x6000];
        std::memcpy(before, hw::at(self_g), sizeof before);
        trace.clear();
        OnDrawSelf__11VIWndSplash(hw::at(self_g));

        std::string want;
        auto show = [&](int a, int b, int width) {
            want += "SU" + msg_r + ",t" + std::to_string(a) + ",t" + std::to_string(b) + ",0,320/246," + std::to_string(width) + ";SF" + self_r + "," + msg_r + ",1;";
        };
        bool conn_cleared = false, msg_flag = false;
        int expect_last = pad;
        if (pad == 0 && last != 0) {
            want += "EC" + conn_r + ";";
            conn_cleared = true;
            if (focus_kind == 1) {
                want += "MSG" + Rg(self_g + 24) + "," + Rg(mcerr_g) + ",4354,0,0;";
                want += "MSG" + Rg(self_g + 24) + "," + Rg(mcerr_g) + ",4358,10,0;";
            }
            show(767, 1862, 400); msg_flag = true;
        }
        if (focus_kind != 0) want += "RT" + win_r + ";";
        unsigned expect9648 = W(9648), expect9652 = W(9652);
        if (q9648) { show(272, 809, 320); msg_flag = true; expect9648 = 0; expect9652 = 0; }
        else if (q9652) { show(272, 808, 320); msg_flag = true; expect9648 = 0; expect9652 = 0; }
        if (q9644) want += "RC" + self_r + "," + Rg(child_g) + ";";
        want += "B;";
        // The hash stub returns the character sum; compute it here independently.
        auto sum = [](const char* s) { unsigned v = 0; for (; *s; ++s) v += static_cast<unsigned char>(*s); return v; };
        want += "H:c:\\tmp\\srs\\conbk00.tga;BL" + std::to_string(sum("c:\\tmp\\srs\\conbk00.tga")) + ",0/0,0," + fbits(1.0f) + ",0;";
        want += "H:c:\\tmp\\srs\\conbk10.tga;BL" + std::to_string(sum("c:\\tmp\\srs\\conbk10.tga")) + ",320/0,0," + fbits(1.0f) + ",0;";
        want += "TC56,255.226.97.255;";
        want += "DS56," + std::to_string(raster_g) + "," + std::to_string(dict_g) + ",t217,265,378;";
        want += "BX176,237/380,0," + fbits(1.0f) + ",0;";
        want += "DS56," + std::to_string(raster_g) + "," + std::to_string(dict_g) + ",t212,356,378;";
        want += "BX173,328/380,0," + fbits(1.0f) + ",0;";
        auto line = [&](int row, int selected, int text_id) {
            std::string buffer;
            if (selected) { want += "S8:512;"; buffer = "|q"; }
            want += "SC:512;";
            buffer += "t" + std::to_string(text_id);
            if (selected) { want += "SD:512;"; buffer += "|w"; }
            const bool white = row == 0 ? sel != 0 : sel != 1;
            want += "TB" + win_r + ",'" + buffer + "',-320," + std::to_string(row) + ",640,1," + fbits(1.0f) + "." + fbits(1.0f) + "." + fbits(white ? 1.0f : 0.0f) + "." + fbits(1.0f) + ",9;";
        };
        line(0, sel == 0, 790);
        line(1, sel == 1, 1635);
        unsigned expect9660 = W(9660);
        if (phase == 2) { want += "SP" + self_r + ";"; expect9660 = 0; }
        want += "E;";
        if (trace != want) {
            std::fputs(("MISMATCH pad=" + std::to_string(pad) + " last=" + std::to_string(last) + " focus=" + std::to_string(focus_kind) + " q9648=" + std::to_string(q9648) + " q9652=" + std::to_string(q9652) +
                        " q9644=" + std::to_string(q9644) + " sel=" + std::to_string(sel) + " phase=" + std::to_string(phase) + "\n got  " + trace + "\n want " + want + "\n").c_str(), stdout);
            return 1;
        }
        if (norrath_splash_last_pad_state != expect_last) ok = false;
        if (W(9648) != expect9648 || W(9652) != expect9652 || W(9644) != 0 || W(9656) != 1 || W(9660) != expect9660) ok = false;
        if (hw::w(self_g + 416 + 28) != (conn_cleared ? 0u : 0x77u)) ok = false;
        if (hw::w(self_g + 4776 + 28) != (msg_flag ? 1u : 0x66u)) ok = false;
        for (unsigned i = 0; i < sizeof before; ++i) {
            const unsigned off = i;
            const bool known = (off >= 9644 && off < 9664) || (off >= 416 + 28 && off < 416 + 32) || (off >= 4776 + 28 && off < 4776 + 32);
            if (!known && hw::at(self_g)[i] != before[i]) ok = false;
        }
        ++scenarios;
        if (!ok) { std::fputs("state mismatch\n", stdout); return 1; }
    }
    std::puts(("ok (" + std::to_string(scenarios) + " scenarios)").c_str());
    return 0;
}
