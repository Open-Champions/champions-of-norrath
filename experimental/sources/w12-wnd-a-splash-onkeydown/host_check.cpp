// Host check only; not a build input. The expected call log is derived from the key table in the
// source comment by a separate model, for every key from 0 to 200 and the selection and gate values
// around each threshold; stores are compared as whole-object snapshots.
#include "onkeydown.cpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned _5VIWnd$mUI = 0x4242;
static unsigned char object[10000];
static std::string trace;
static std::string rel(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - object); }
extern "C" void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound) { trace += "S" + std::to_string(sound) + (ui == 0x4242 ? "" : "?") + ";"; }
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) { trace += "F" + rel(self) + "," + (window ? rel(window) : "0") + "," + (force ? "1" : "0") + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void* window) { trace += "R" + rel(window) + ";"; }
extern "C" void SelectPlay__11VIWndSplash(void* self) { trace += "P" + rel(self) + ";"; }

static int& at(int off) { return *reinterpret_cast<int*>(object + off); }

int main() {
    bool ok = true;
    for (int key = 0; key < 200; ++key)
        for (int selection : {-2, -1, 0, 1, 2, 3, 7})
            for (int gate : {0, 1, -1}) {
                std::memset(object, 0x33, sizeof object);
                at(408) = selection; at(9656) = gate; at(9664) = 0x33333333; at(2408 + 28) = 0x33333333;
                unsigned char before[sizeof object];
                std::memcpy(before, object, sizeof object);
                trace.clear();
                OnKeyDown__11VIWndSplashi(object, key);
                // Model from the documented table.
                std::string want;
                int sel = selection, flag9664 = 0x33333333, flag2436 = 0x33333333;
                if (key == 135) { sel = 1; flag9664 = 1; want = "S13;F0,0,1;"; }
                else if (key == 138 && gate != 0 && sel > 0) { sel -= 1; want = "R40;S11;"; }
                else if (key == 139 && gate != 0 && sel <= 0) { sel += 1; want = "R40;S11;"; }
                else if (key == 134 && gate != 0) {
                    if (sel == 1) want = "S13;F0,0,1;";
                    else if (sel == 0) want = "S15;P0;";
                    else if (sel == 2) { flag2436 = 1; want = "F0,2408,1;S15;"; }
                }
                ok = ok && trace == want && at(408) == sel && at(9664) == flag9664 && at(2436) == flag2436 && at(9656) == gate;
                // Nothing else may change.
                for (unsigned i = 0; i < sizeof object; ++i) {
                    const bool known = (i >= 408 && i < 412) || (i >= 9664 && i < 9668) || (i >= 2436 && i < 2440);
                    if (!known && object[i] != before[i]) ok = false;
                }
                if (!ok) { std::printf("key %d sel %d gate %d got '%s' want '%s'\n", key, selection, gate, trace.c_str(), want.c_str()); return 1; }
            }
    std::puts("ok");
    return 0;
}
