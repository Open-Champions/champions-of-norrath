// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "onkeydown.cpp"
#include <cstdio>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[512];
alignas(8) static unsigned char parent[64];
static VtEntry table[8];
unsigned _5VIWnd$mUI = 0x1234u;
unsigned char sKeyMap[66];
int sCompactKeyboard;
static unsigned short text[4];
static std::string trace;
static int insert_result, backspace_result, setcaret_result, up_result, down_result;

extern "C" {
int Insert__9VIWndEditUs(void*, unsigned short ch) { trace += "ins" + std::to_string(ch) + ";"; return insert_result; }
int Backspace__9VIWndEdit(void*) { trace += "bs;"; return backspace_result; }
int CaretOffset__9VIWndEdit(void*) { trace += "caret;"; return 7; }
int SetCaretOffset__9VIWndEditi(void*, int o) { trace += "set" + std::to_string(o) + ";"; return setcaret_result; }
int MoveCaretUp__9VIWndEdit(void*) { trace += "up;"; return up_result; }
int MoveCaretDown__9VIWndEdit(void*) { trace += "down;"; return down_result; }
void MoveCursor__9VIWndEditii(void*, int dx, int dy) { trace += "cur" + std::to_string(dx) + "," + std::to_string(dy) + ";"; }
void OnKeyDown__5VIWndi(void*, int k) { trace += "base" + std::to_string(k) + ";"; }
unsigned long long GetTime__7VITimer() { trace += "time;"; return 0x1122334455667788ull; }
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int s) { trace += std::string("snd") + (reinterpret_cast<unsigned long>(ui) == 0x1234 ? "" : "?") + std::to_string(s) + ";"; }
}
static void entry_fn(void* p, void* from, int m, int a, int b) {
    trace += "msg" + std::to_string(static_cast<unsigned char*>(p) - parent) + "," + (from == object ? "self" : "?") + "," + std::to_string(m) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
static int* words() { return reinterpret_cast<int*>(object); }
static std::string run(int key) {
    trace.clear();
    OnKeyDown__9VIWndEditi(object, key);
    return trace;
}
static void reset() {
    std::memset(object, 0, sizeof object);
    std::memset(sKeyMap, 0, sizeof sKeyMap);
    sCompactKeyboard = 0;
    insert_result = backspace_result = setcaret_result = up_result = down_result = 1;
    words()[5] = static_cast<int>(reinterpret_cast<unsigned long>(parent));
    words()[9] = 55; words()[10] = 66;
    text[0] = 'x'; text[1] = 0;
    words()[106] = static_cast<int>(reinterpret_cast<unsigned long>(text));
    *reinterpret_cast<unsigned*>(parent + 32) = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
    std::memset(table, 0, sizeof table);
    table[48 / sizeof(VtEntry) + 0] = VtEntry{24, 0, entry_fn};  // host entries are wider than the EE entries
    words()[96] = 99;
}

int main() {
    bool ok = true;
    reset(); ok = ok && run('A') == "ins65;snd11;" && words()[96] == 0;
    reset(); insert_result = 0; ok = ok && run('q') == "ins113;snd9;";
    reset(); ok = ok && run('7') == "ins55;snd11;";
    reset(); ok = ok && run(',') == "ins44;snd11;" && run(8) == "bs;snd11;" && run(0) == "" && run(500) == "" && run(5) == "";
    reset(); ok = ok && run(27) == "snd8;msg24,self,260,0,0;" && run(135) == "snd8;msg24,self,260,0,0;";
    reset(); ok = ok && run(4) == "msg24,self,259,55,66;" && run(143) == "snd11;msg24,self,259,55,66;" && run(13) == "msg24,self,259,55,66;";
    reset(); words()[110] = 1; ok = ok && run(13) == "ins10;snd11;" && run(141) == "ins10;snd11;";
    reset(); ok = ok && run(141) == "";
    reset(); setcaret_result = 0; ok = ok && run(17) == "time;caret;set6;snd9;" && run(18) == "time;caret;set8;snd9;";
    { unsigned long long v; std::memcpy(&v, object + 416, 8); ok = ok && v == 0x1122334455667788ull; }
    reset(); up_result = 0; ok = ok && run(19) == "up;snd9;" && run(20) == "down;snd11;";
    reset(); ok = ok && run(142) == "snd11;" && words()[12] == 1;
    reset(); sCompactKeyboard = 1; ok = ok && run(142) == "" && run(141) == "" && run(132) == "" && run(133) == "" && run(136) == "" && run(134) == "";
    reset(); ok = ok && run(136) == "cur-1,0;" && run(137) == "cur1,0;" && run(138) == "cur0,-1;" && run(139) == "cur0,1;" && words()[96] == 139;
    { float f; std::memcpy(&f, object + 388, 4); ok = ok && f == 0.45f; }
    reset(); ok = ok && run(132) == "ins32;snd11;" && run(146) == "base146;";
    reset(); ok = ok && run(133) == "bs;snd11;"; text[0] = 0; ok = ok && run(133) == "snd9;";
    // key 134: grid cell codes
    for (int code = 0; code < 20; ++code) {
        reset(); words()[82] = 3; words()[83] = 2; words()[0 + 11] = 0; sKeyMap[3 + 22] = static_cast<unsigned char>(code);
        const std::string got = run(134);
        const char* want = nullptr;
        switch (code) {
        case 1: want = "bs;snd11;"; break;
        case 2: want = "ins32;snd11;"; break;
        case 3: want = "snd11;"; break;
        case 5: want = "snd11;msg24,self,259,55,66;"; break;
        case 6: want = "ins10;snd11;"; break;
        case 7: want = "up;"; break;
        case 8: want = "down;"; break;
        case 9: want = "time;caret;set6;"; break;
        case 10: want = "time;caret;set8;"; break;
        case 11: want = "ins45;snd11;"; break;
        case 12: want = "ins126;snd11;"; break;
        case 13: want = "ins44;snd11;"; break;
        case 14: want = "ins34;snd11;"; break;
        case 15: want = "ins95;snd11;"; break;
        case 16: want = "ins39;snd11;"; break;
        default: want = nullptr;
        }
        const std::string expect = want ? want : "ins" + std::to_string(code) + ";snd11;";
        ok = ok && got == expect && words()[86] == 3 && words()[87] == 2 && words()[13] == (code == 3 ? 1 : 0);
        float f; std::memcpy(&f, object + 352, 4); ok = ok && f == 1.0f;
    }
    // letters in the grid: lowered unless shifted
    for (int caps : {0, 1, 2}) for (int a : {0, 1}) for (int b : {0, 1, 2}) {
        reset(); words()[82] = 0; words()[83] = 0; sKeyMap[0] = 'Q'; words()[11] = caps; words()[12] = a; words()[13] = b;
        const bool lower = (caps == 0 && a == 0 && b == 0) || (b == 1 && (caps == 1 || a == 1));
        ok = ok && run(134) == std::string("ins") + (lower ? "113" : "81") + ";snd11;";
    }
    reset(); words()[82] = 0; words()[83] = 0; sKeyMap[0] = 0xe9; ok = ok && run(134) == "ins65513;snd11;";  // sign-extended byte
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
