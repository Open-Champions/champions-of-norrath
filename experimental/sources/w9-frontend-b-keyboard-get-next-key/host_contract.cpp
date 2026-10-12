#include "keyboard_get_next_key.cpp"

#include <cstdio>
#include <initializer_list>

char vik[8];
int lastkey, lasttime, framedone = -7, drawFrameNm = 1;
struct Ev { int ok; unsigned char code, flag; };
static Ev evs[16];
static int nev, pos, reads;
extern "C" int Read__10VIKeyboardR12VIConsoleKey(void* kb, void* key) {
    ++reads;
    if (kb != vik) return 99;
    if (pos >= nev) return 0;
    Ev& e = evs[pos++];
    if (e.ok) { static_cast<unsigned char*>(key)[0] = e.code; static_cast<unsigned char*>(key)[1] = e.flag; }
    return e.ok;
}
static void feed(std::initializer_list<Ev> l) { nev = 0; pos = 0; reads = 0; for (auto e : l) evs[nev++] = e; }

static int press(int code) { feed({{1, static_cast<unsigned char>(code), 1}}); return keyboardGetNextKey__Fv(); }

int main() {
    // remapping on press
    static const int from[] = {19, 25, 26, 90, 91, 93, 96, 123, 124, 125, 126, 127, 5, 18, 200};
    static const int to[]   = {-1, -1, 26, 90, 138, 139, 39, 91, 140, 93, 96, 127, 5, 18, 200};
    for (unsigned i = 0; i < sizeof from / sizeof *from; ++i) {
        lastkey = 0; lasttime = 50;
        if (press(from[i]) != to[i]) return 1 + static_cast<int>(i);
        if (lastkey != to[i] || lasttime != 0) return 20;
    }
    // no event
    lastkey = 0; lasttime = 99; feed({});
    if (keyboardGetNextKey__Fv() != -1 || lasttime != 99) return 21;  // lastkey zero: no tick
    lastkey = 'a'; lasttime = 5; feed({});
    if (keyboardGetNextKey__Fv() != -1 || lasttime != 6) return 22;
    lastkey = 'a'; lasttime = 30; feed({});
    if (keyboardGetNextKey__Fv() != 'a' || framedone != drawFrameNm) return 23;  // 31 reached
    lasttime = 40; feed({});
    if (keyboardGetNextKey__Fv() != -1) return 24;  // same frame
    drawFrameNm = 2; feed({});
    if (keyboardGetNextKey__Fv() != 'a' || framedone != 2) return 25;
    // release of the held key, then a press in the same call
    lastkey = 'a'; lasttime = 3;
    feed({{1, 'a', 0}, {1, 'b', 1}});
    if (keyboardGetNextKey__Fv() != 'b' || lastkey != 'b' || lasttime != 0 || reads != 2) return 26;
    // release of another key keeps lastkey; the loop ends on no event
    lastkey = 'a'; lasttime = 3;
    feed({{1, 'z', 0}});
    if (keyboardGetNextKey__Fv() != -1 || lastkey != 'a' || reads != 2) return 27;
    // release is compared after remapping
    lastkey = 138; lasttime = 3;
    feed({{1, 91, 0}});
    keyboardGetNextKey__Fv();
    if (lastkey != 0) return 28;
    std::puts("keyboard get next key passed");
    return 0;
}
