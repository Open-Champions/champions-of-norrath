// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onkeydown.cpp"
#include <cstdio>
#include <string>

unsigned _5VIWnd$mUI = 0x1234u;
static std::string trace;
extern "C" {
void Prev__16VIWndReadMessage(void*) { trace += "prev;"; }
void Next__16VIWndReadMessage(void*) { trace += "next;"; }
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += "snd:" + std::to_string(reinterpret_cast<unsigned long>(ui)) + "," + std::to_string(s) + ";"; }
void SetFocus__5VIWndP5VIWndb(void*, void* t, bool f) { trace += "focus:" + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(f) + ";"; }
}

int main() {
    bool ok = true;
    int words[16];
    for (int key : {0, 131, 132, 133, 137, 138, 139, 140, 142}) {
        for (int& w : words) w = 9;
        words[5] = 0x2468;
        trace.clear();
        OnKeyDown__16VIWndReadMessagei(words, key);
        const char* want = key == 138 ? "prev;" : key == 139 ? "next;" : key == 132 ? "snd:4660,8;focus:9320,1;" : "";
        ok = ok && trace == want && words[7] == (key == 132 ? 0 : 9);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
