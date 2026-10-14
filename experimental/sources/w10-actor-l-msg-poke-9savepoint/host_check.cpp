// Host contract for msg_poke__9Savepointi; not an EE input. Both gates, the order of the calls
// and the word store before the browser call are checked, as is the untouched object on the
// early exits.
#include <cstring>
#include <initializer_list>
#include <string>
static std::string calls; static int busy, mutex, word_at_browser; static unsigned char* g_o;
extern "C" int MC_Busy__Fv() { calls += "busy;"; return busy; }
extern "C" int DLG_CheckMutex__Fv() { calls += "mutex;"; return mutex; }
extern "C" void MC_SaveGameBrowser__Fi(int a) { calls += "browser(" + std::to_string(a) + ");"; std::memcpy(&word_at_browser, g_o + 132, 4); }
extern "C" void lightActivate__FP5Light(void* l) { calls += "light" + std::to_string(static_cast<unsigned char*>(l) - g_o) + ";"; }
extern "C" void objectDoMyActions__FP10GameObjecti(void* o, int v) { calls += "actions(" + std::to_string(o == g_o) + "," + std::to_string(v) + ");"; }
#include "msg_poke.cpp"
int main() {
    alignas(16) static unsigned char o[256], ref[256];
    g_o = o; int bad = 0;
    for (int b : {0, 1, -1}) for (int m : {0, 1, 5}) for (int arg : {0, 3, -7}) {
        std::memset(o, 0x21, sizeof o); std::memcpy(ref, o, sizeof o); calls.clear(); busy = b; mutex = m; word_at_browser = -1;
        msg_poke__9Savepointi(o, arg);
        if (b) { if (calls != "busy;" || std::memcmp(o, ref, sizeof o)) bad = 1; continue; }
        if (m) { if (calls != "busy;mutex;" || std::memcmp(o, ref, sizeof o)) bad = 1; continue; }
        const int one = 1; std::memcpy(ref + 132, &one, 4); ref[196] = 1;
        if (calls != "busy;mutex;browser(" + std::to_string(arg) + ");light136;actions(1,1);" || word_at_browser != 1 || std::memcmp(o, ref, sizeof o)) bad = 1;
    }
    return bad;
}
