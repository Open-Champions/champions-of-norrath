// Host check only; not a build input. The window lives in the arena (the UI pointer word is a guest
// address). The expected call log and stores come from a separate model of the key table in the
// source comment, for every key from 128 to 140 plus out-of-range keys, modes 0-8, selection values
// below, at and above zero, every combination of the three flag words and an empty/non-empty message.
#define NORRATH_HOST_ARENA 1
#include "onkeydown.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned _5VIWnd$mUI;
static const unsigned window = hw::BASE + 0x100, ui = hw::BASE + 0x6000;
static std::string trace;
static unsigned short message_text[2];
static unsigned short string_a[2] = {0x41, 0}, string_b[2] = {0x42, 0};
static std::string rel(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - hw::at(window)); }
extern "C" const unsigned short* Message__C16VIWndReadMessage(const void* w) { trace += "M" + rel(w) + ";"; return message_text; }
extern "C" void SoundPlay__4VIUI11VIUISoundId(unsigned u, int sound) { trace += "S" + std::to_string(sound) + (u == ui ? "" : "?") + ";"; }
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* w, bool force) { trace += "F" + rel(self) + "," + (w ? rel(w) : "0") + "," + (force ? "1" : "0") + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void* w) { trace += "R" + rel(w) + ";"; }
extern "C" const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg) {
    trace += "G" + std::string(hw::guest(table) == ui ? "ui" : "?") + "," + std::to_string(id) + "," + std::to_string(arg) + ";";
    return id == 785 ? string_a : string_b;
}
extern "C" void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* w, const unsigned short* first, const unsigned short* second, int prompt,
                                                                            const Pair* rect, int size) {
    trace += "U" + rel(w) + "," + (first == string_a ? "a" : "?") + (second == string_b ? "b" : "?") + "," + std::to_string(prompt) + "," +
             std::to_string(rect->a) + "/" + std::to_string(rect->b) + "," + std::to_string(size) + ";";
}
static unsigned& W(int off) { return hw::w(window + static_cast<unsigned>(off)); }

int main() {
    bool ok = true;
    long scenarios = 0;
    for (int key = 126; key < 142; ++key)
      for (int mode = 0; mode < 9; ++mode)
        for (int sel : {-1, 0, 1, 2})
          for (int bits = 0; bits < 8; ++bits)
            for (int text : {0, 1}) {
                hw::clear();
                _5VIWnd$mUI = ui;
                std::memset(hw::at(window), 0x11, 4000);
                W(1148) = mode; W(1144) = sel;
                W(1156) = (bits & 1) ? 7 : 0; W(1160) = (bits & 2) ? 8 : 0; W(1164) = (bits & 4) ? 9 : 0;
                W(2312 + 28) = 0x11111111; W(788) = 111; W(792) = 222;
                hw::w(ui + 48) = 31337;
                message_text[0] = text ? 0x7777 : 0;
                unsigned char before[4000];
                std::memcpy(before, hw::at(window), 4000);
                trace.clear();
                OnKeyDown__12VIWndPatcheri(hw::at(window), key);

                std::string want;
                int s = sel;
                unsigned f1156 = W(1156), f1160 = W(1160);
                bool set2340 = false, set1204 = false;
                if ((key == 138 || key == 139) && mode == 4) {
                    want = "M2312;";
                    if (text) {
                        if (key == 138) {
                            if (sel <= 0) want += "S9;"; else { s = sel - 1; want += "R776;S11;"; }
                        } else {
                            if (sel > 0) want += "S9;"; else { s = sel + 1; want += "R776;S11;"; }
                        }
                    }
                } else if (key == 134) {
                    if (mode == 4 && !(bits & 4)) {
                        if (sel == 0) { set2340 = true; want = "F0,2312,1;S15;"; }
                        else { if (!(bits & 1)) want += "S11;"; want += "F0,0,1;"; }
                    } else if (mode == 2 || mode == 7) { f1156 = 1; want = "S9;F0,0,1;"; }
                } else if (key == 132 && mode == 4 && !(bits & 4) && !(bits & 1)) {
                    f1160 = 1;
                    set1204 = true;
                    want = "S15;Gui,785,31337;Gui,810,31337;U1176,ab,2,111/222,400;F0,1176,1;";
                }
                if (trace != want) { ok = false; std::printf("key %d mode %d sel %d bits %d text %d got '%s' want '%s'\n", key, mode, sel, bits, text, trace.c_str(), want.c_str()); }
                if (static_cast<int>(W(1144)) != s) ok = false;
                if (W(1156) != f1156 || W(1160) != f1160) ok = false;
                if (W(2340) != (set2340 ? 1u : 0x11111111u)) ok = false;
                if (W(1204) != (set1204 ? 1u : 0x11111111u)) ok = false;
                for (int i = 0; i < 4000; ++i) {
                    const bool known = (i >= 1144 && i < 1148) || (i >= 1156 && i < 1164) || (i >= 2340 && i < 2344) || (i >= 1204 && i < 1208);
                    if (!known && hw::at(window)[i] != before[i]) { ok = false; std::printf("stray write at %d\n", i); break; }
                }
                ++scenarios;
                if (!ok) return 1;
            }
    std::printf("ok (%ld scenarios)\n", scenarios);
    return 0;
}
