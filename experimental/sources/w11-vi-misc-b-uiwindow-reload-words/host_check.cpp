// Host check only; not a build input. Load results -5, -1, 0, 1 and 7: the call order, the path
// text, the cleared word, the message and the return value are checked.
#include "reload_words.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static int load_result;
static unsigned char object[128];

extern "C" {
void Unload__13VIStringTable(VIStringTable*) { trace += "unload;"; }
int Load__13VIStringTablePCc(VIStringTable*, const char* name) {
    unsigned w;
    std::memcpy(&w, object + 52, 4);
    trace += std::string("load:") + name + ":" + std::to_string(w) + ";";
    return load_result;
}
int printf(const char* format, ...) { trace += std::string("printf:") + format + ";"; return 0; }
}

int main() {
    bool ok = true;
    for (int result : {-5, -1, 0, 1, 7}) {
        std::memset(object, 0x5a, sizeof object);
        trace.clear();
        load_result = result;
        const int r = ReloadWords__4VIUI(object);
        std::string want = "unload;load:data\\words.txt:0;";
        if (result >= 0) want += "printf:WORDS.TXT read from disc.\n;";
        ok = ok && trace == want && r == (result < 0 ? -1 : 0) && object[51] == 0x5a && object[56] == 0x5a;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
