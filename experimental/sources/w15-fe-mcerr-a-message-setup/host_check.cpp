// Host check only; not a build input. The ui pointer word is compared by value (the host pointer is
// truncated to 32 bits by the object's word, so the check compares the word the routine read). Records every callee with its arguments (string and vector contents).
#include "setup.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned _5VIWnd$mUI;
static unsigned char object[1100];
static int ui_marker;
static std::string trace;
static std::string str(const unsigned short* s) {
    std::string r;
    while (*s) r += static_cast<char>(*s++);
    return r;
}
static std::string off(const void* p) { return std::to_string(static_cast<const unsigned char*>(p) - object); }
extern "C" void strcpy16__8VIStringPUsUiPCUs(unsigned short* dest, unsigned size, const unsigned short* source) {
    trace += "cp:" + off(dest) + "," + std::to_string(size) + "," + str(source) + ";";
}
extern "C" void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, void* ui, const int* position, const int* size,
                                                         int width, int a, int b) {
    trace += "init:" + off(window) + "," + (static_cast<unsigned>(reinterpret_cast<unsigned long>(ui)) == _5VIWnd$mUI ? "ui" : "?") + ",pos=" + std::to_string(position[0]) + "/" +
             std::to_string(position[1]) + ",size=" + off(size) + ":" + std::to_string(size[0]) + "/" + std::to_string(size[1]) +
             "," + std::to_string(width) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
extern "C" int CountLines__10VIUIWindowPCUsi(void* window, const unsigned short* text, int width) {
    trace += "lines:" + off(window) + "," + off(text) + "," + std::to_string(width) + ";";
    return 11;
}

int main() {
    bool ok = true;
    std::memset(object, 0x44, sizeof object);
    _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(&ui_marker));
    const unsigned short title[3] = {'T', 'i', 0};
    const unsigned short text[3] = {'B', 'o', 0};
    const int size[2] = {300, 120};
    Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(object, title, text, 3, size, 440);
    int words[275];
    std::memcpy(words, object, sizeof words);
    ok = ok && trace == "cp:408,64,Ti;cp:472,512,Bo;init:40,ui,pos=0/0,size=988:300/120,440,20,0;lines:40,472,360;";
    ok = ok && words[251] == 3 && words[247] == 300 && words[248] == 120 && words[249] == 440 && words[246] == 0 &&
         words[250] == 14;
    ok = ok && words[245] == 0x44444444 && words[252] == 0x44444444 && object[407] == 0x44;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
