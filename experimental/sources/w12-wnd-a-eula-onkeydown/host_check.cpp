// Host check only; not a build input. Compares the call log and every changed byte with a separate
// model of the key table in the source comment, for keys 126-144 (and a few far values), both input
// block states and both selection values (plus others).
#include "onkeydown.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI = 0x4242;
static unsigned char object[10000];
static int tokens;
static std::string trace;
static std::string rel(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - object); }
extern "C" void* GetAppPtr__5VIWnd(void* self) { trace += "A" + rel(self) + ";"; return &tokens; }
extern "C" void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound) { trace += "S" + std::to_string(sound) + (ui == 0x4242 ? "" : "?") + ";"; }
extern "C" void EulaComplete__9VIStationUc(void* station, unsigned char accepted) { trace += std::string("C") + (station == &tokens ? "t" : "?") + std::to_string(accepted) + ";"; }
extern "C" void ResetTickPulse__10VIUIWindow(void* window) { trace += "R" + rel(window) + ";"; }
extern "C" void Up__9VIWndEula(void* self) { trace += "U" + rel(self) + ";"; }
extern "C" void Down__9VIWndEula(void* self) { trace += "D" + rel(self) + ";"; }
static int& at(int off) { return *reinterpret_cast<int*>(object + off); }

int main() {
    bool ok = true;
    int keys[40], n = 0;
    for (int k = 126; k <= 144; ++k) keys[n++] = k;
    for (int k : {0, 13, -1, 134 + 256, 1 << 20}) keys[n++] = k;
    for (int ki = 0; ki < n; ++ki)
      for (int block : {0, 1, -1})
        for (int selection : {-1, 0, 1, 2})
          for (int counter : {0, 5}) {
            const int key = keys[ki];
            std::memset(object, 0x44, sizeof object);
            at(9764) = block; at(1544) = selection; at(9756) = counter; at(9760) = 0x44444444;
            unsigned char before[sizeof object];
            std::memcpy(before, object, sizeof object);
            trace.clear();
            OnKeyDown__9VIWndEulai(object, key);
            std::string want = "A0;";
            int sel = selection, count = 0, dir = 0x44444444;
            switch (key) {
            case 134: case 135: want += "S15;Ct1;"; break;
            case 136: if (block == 0 && selection == 1) { want += "S11;R40;"; sel = 0; } break;
            case 137: if (block == 0 && selection == 0) { want += "S11;R40;"; sel = 1; } break;
            case 138: if (block == 0) { count = 10; dir = -1; want += "U0;"; } break;
            case 139: if (block == 0) { count = 10; dir = 1; want += "D0;"; } break;
            default: break;
            }
            ok = ok && trace == want && at(1544) == sel && at(9756) == count && at(9760) == dir && at(9764) == block;
            for (unsigned i = 0; i < sizeof object; ++i) {
                const bool known = (i >= 1544 && i < 1548) || (i >= 9756 && i < 9764);
                if (!known && object[i] != before[i]) ok = false;
            }
            if (!ok) { std::printf("key %d block %d sel %d got '%s' want '%s'\n", key, block, selection, trace.c_str(), want.c_str()); return 1; }
          }
    std::puts("ok");
    return 0;
}
