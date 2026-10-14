// Host check only; not a build input. The window lives in the arena (the UI word and the text
// pointers are guest addresses). Compares the call log with a separate model of the step list in
// the source comment over input-block, recompute, repeat-counter, direction, scroll-top and
// record-list shapes; the text copies and terminators are checked through the Text stub.
#define NORRATH_HOST_ARENA 1
#include "ondrawself.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned _5VIWnd$mUI;
static const unsigned window_g = hw::BASE + 0x100, ui_g = hw::BASE + 0x8000, texts_g = hw::BASE + 0x10000;
static std::string trace;
static std::string rel(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - hw::at(window_g)); }
static std::string ascii(const unsigned short* s) { std::string r; for (; *s; ++s) r += static_cast<char>(*s); return r; }
static const unsigned short footer_text[] = {'F', 'T', 0};
static std::string float_bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return std::to_string(u); }

extern "C" void ResetTickPulse__10VIUIWindow(void* w) { trace += "R" + rel(w) + ";"; }
extern "C" void ComputeTextRows__9VIWndEula(void* self) { trace += "K" + rel(self) + ";"; }
extern "C" void Down__9VIWndEula(void* self) { trace += "D" + rel(self) + ";"; }
extern "C" void Up__9VIWndEula(void* self) { trace += "U" + rel(self) + ";"; }
extern "C" void Begin2D__4VIUI(unsigned ui) { trace += std::string("B") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void End2D__4VIUI(unsigned ui) { trace += std::string("N") + (ui == ui_g ? "" : "?") + ";"; }
extern "C" void Draw__10VIUIWindow(void* w) { trace += "W" + rel(w) + ";"; }
extern "C" void DrawFrameVScroll__10VIUIWindowiiiif(void* w, int a, int b, int c, int d, float fraction) {
    trace += "V" + rel(w) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + "," + float_bits(fraction) + ";";
}
extern "C" void strcpy16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source) {
    trace += "Y" + std::to_string(size) + ";";
    // Fill past the copy with junk so a missing terminator shows up.
    for (unsigned i = 0; i < size / 2 + 3; ++i) destination[i] = 0x5a5a;
    for (unsigned i = 0; i < size / 2; ++i) destination[i] = source[i];
}
extern "C" void Text__10VIUIWindowPCUsii9VIUICOLORUi(void* w, const unsigned short* text, int x, int y, int color, unsigned flags) {
    trace += "T" + rel(w) + ",'" + ascii(text) + "'," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(color) + "," + std::to_string(flags) + ";";
}
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    trace += std::string("G") + (hw::guest(table) == ui_g ? "" : "?") + std::to_string(id) + "," + std::to_string(arg) + ";";
    return footer_text;
}
extern "C" int snprintf16__8VIStringPUsiPCce(unsigned short* destination, int size, const char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const unsigned short* argument = __builtin_va_arg(args, const unsigned short*);
    __builtin_va_end(args);
    trace += "P" + std::to_string(size) + ",'" + format + "'," + (argument == footer_text ? "ft" : "?") + ";";
    destination[0] = 'P';
    destination[1] = 0;
    return 1;
}
static unsigned& W(int off) { return hw::w(window_g + static_cast<unsigned>(off)); }

int main() {
    bool ok = true;
    long scenarios = 0;
    // Records: strings "aN" of lengths 1..3 for the first `filled` records, then a zero pointer.
    for (int block : {0, 1})
      for (int recompute : {0, 1})
        for (int counter : {0, 1, 2, 7})
          for (int dir : {-3, 0, 1, 4})
            for (int top : {0, 2, 5})
              for (int filled : {0, 1, 5, 16, 20, 30})
                for (int selection : {0, 1})
                  for (int width : {400, 641}) {
                    hw::clear();
                    _5VIWnd$mUI = ui_g;
                    hw::w(ui_g + 48) = 77;
                    W(9764) = block; W(1548) = recompute; W(9756) = counter; W(9760) = static_cast<unsigned>(dir);
                    W(1556) = top; W(1544) = selection; W(9752) = 100; W(40 + 20) = width;
                    for (int r = 0; r < 40; ++r) {
                        const unsigned record = window_g + 1560 + 8 * r;
                        if (r < filled) {
                            const int length = 1 + (r % 3);
                            const unsigned text = texts_g + 32 * r;
                            for (int c = 0; c < length; ++c) { unsigned short u = static_cast<unsigned short>('a' + r + c); std::memcpy(hw::at(text) + 2 * c, &u, 2); }
                            hw::w(record) = text;
                            const short len = static_cast<short>(length);
                            std::memcpy(hw::at(record + 4), &len, 2);
                        }
                    }
                    unsigned char before[9900];
                    std::memcpy(before, hw::at(window_g), sizeof before);
                    trace.clear();
                    OnDrawSelf__9VIWndEula(hw::at(window_g));

                    std::string want;
                    int cur_top = top, cur_counter = counter, cur_recompute = recompute;
                    if (block) want += "R40;";
                    if (cur_recompute) { want += "K0;"; cur_recompute = 0; cur_top = 0; }
                    if (cur_counter) {
                        --cur_counter;
                        if (cur_counter == 0) { cur_counter = 2; want += dir > 0 ? "D0;" : "U0;"; }
                    }
                    want += "B;W40;";
                    want += "V40,40,0," + std::to_string(width - 80) + ",16," + float_bits(static_cast<float>(cur_top) / static_cast<float>(100 - 17)) + ";";
                    for (int row = 0; row < 16; ++row) {
                        const int r = cur_top + row;
                        if (r >= filled) break;
                        const int length = 1 + (r % 3);
                        std::string text;
                        for (int c = 0; c < length; ++c) text += static_cast<char>('a' + r + c);
                        want += "Y" + std::to_string(length * 2) + ";T40,'" + text + "',40," + std::to_string(row) + ",0,0;";
                    }
                    want += "G750,77;P1024,'|c %S',ft;";
                    want += "T40,'P'," + std::to_string(width / 2 - 40) + ",17," + std::to_string(selection == 0 ? 1 : 0) + ",0;N;";
                    if (trace != want) {
                        std::printf("block %d recompute %d counter %d dir %d top %d filled %d\n got '%s'\nwant '%s'\n", block, recompute, counter, dir, top, filled, trace.c_str(), want.c_str());
                        return 1;
                    }
                    ok = ok && static_cast<int>(W(9756)) == cur_counter && static_cast<int>(W(1556)) == cur_top && W(1548) == 0u;
                    for (unsigned i = 0; i < sizeof before; ++i) {
                        const bool known = (i >= 1548 && i < 1552) || (i >= 1556 && i < 1560) || (i >= 9756 && i < 9760);
                        if (!known && hw::at(window_g)[i] != before[i]) ok = false;
                    }
                    ++scenarios;
                    if (!ok) { std::printf("state mismatch at scenario %ld\n", scenarios); return 1; }
                  }
    std::printf("ok (%ld scenarios)\n", scenarios);
    return 0;
}
