// Host check only; not a build input.
#include "saveandexit.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static int card, saved;
static int options;
extern "C" int VIMemCardStatus__FPi(int* p) { trace += p ? "card?;" : "card0;"; return card; }
extern "C" void McErr__FP5VIWndRC8VIVect2Ii(void*, const int* s, int m) { trace += "err(" + std::to_string(s[0]) + "," + std::to_string(s[1]) + "," + std::to_string(m) + ");"; }
extern "C" void McErrSetMode__Fi(int m) { trace += "mode" + std::to_string(m) + ";"; }
extern "C" void* VIGetGameOptions__Fv() { trace += "opts;"; return &options; }
extern "C" int Save__13VIGameOptions(void* o) { trace += o == &options ? "save;" : "savebad;"; return saved; }
extern "C" void SetFocus__5VIWndP5VIWndb(void*, void* t, bool f) { trace += "focus(" + std::to_string(reinterpret_cast<unsigned long>(t)) + "," + std::to_string(f) + ");"; }

int main() {
    static int w[400];
    bool ok = true;
    w[13] = 11; w[14] = 22; w[7] = 9; w[5] = 77; card = 1; trace.clear();
    ok = ok && SaveAndExit__12VIWndOptions(w) == -1 && w[296] == 1 && trace == "card0;err(11,22,4);mode1;";
    card = 0; saved = -3; w[296] = 0; trace.clear();
    ok = ok && SaveAndExit__12VIWndOptions(w) == -1 && w[298] == 1 && w[296] == 0 && w[7] == 9 && trace == "card0;opts;save;";
    saved = 0; trace.clear();
    ok = ok && SaveAndExit__12VIWndOptions(w) == 0 && w[7] == 0 && trace == "card0;opts;save;focus(77,1);";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
