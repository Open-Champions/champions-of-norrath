// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onkeydown.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static std::string trace;
extern "C" {
void ResetTickPulse__10VIUIWindow(void* w) { trace += "reset:" + std::to_string(reinterpret_cast<unsigned long>(w) % 1000) + ";"; }
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += "snd:" + std::to_string(reinterpret_cast<unsigned long>(ui)) + "," + std::to_string(s) + ";"; }
}
alignas(8) static unsigned char parent[64];
static VtEntry table[16];
static void* self_ptr;
static void entry_fn(void* p, void* from, int message, int n, void* last) {
    trace += "vcall:" + std::to_string(static_cast<unsigned char*>(p) - parent) + "," + (from == self_ptr ? "self" : "?") + "," +
             std::to_string(message) + "," + std::to_string(n) + "," + (last ? "?" : "0") + ";";
}

int main() {
    bool ok = true;
    alignas(8) static int words[300];
    self_ptr = words;
    *reinterpret_cast<unsigned*>(parent + 32) = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
    table[48 / sizeof(VtEntry)] = VtEntry{16, 5, entry_fn};  // the host entry is wider than the 8-byte EE entry
    for (int key : {0, 133, 134, 135, 136, 137, 138}) {
        for (int n : {-1, 0, 1, 2}) {
            for (int two : {0, 3}) {
                for (int& w : words) w = 9;
                words[5] = static_cast<int>(reinterpret_cast<unsigned long>(parent));
                words[246] = n; words[251] = two;
                trace.clear();
                OnKeyDown__12VIWndMessagei(words, key);
                const unsigned long self_mod = reinterpret_cast<unsigned long>(words + 10) % 1000;
                std::string want;
                int want_n = n, want_28 = 9;
                const std::string tick = "reset:" + std::to_string(self_mod) + ";snd:4660,11;";
                if (key == 136 && n > 0) { want = tick; want_n = n - 1; }
                if (key == 137 && (two ? n <= 0 : n < 0)) { want = tick; want_n = n + 1; }
                if (key == 134) {
                    want_28 = 0;
                    want = "vcall:16,self,307," + std::to_string(n) + ",0;snd:4660," + std::to_string(two ? (n == 0 ? 13 : 8) : 13) + ";";
                }
                ok = ok && trace == want && words[246] == want_n && words[7] == want_28;
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
