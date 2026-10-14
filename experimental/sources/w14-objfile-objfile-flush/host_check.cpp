// Host check only; not a build input.
#include "flush.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static std::string trace; static void* obj;
extern "C" int WriteFileHeader__9VIObjFilei(void* o, int f) { trace += (o == obj && f == 0) ? "hdr;" : "hdr?;"; return 3; }
extern "C" int Flush__6VIFile(void* o) { trace += o == obj ? "flush;" : "flush?;"; return 4; }
int main() {
    bool ok = true;
    alignas(8) unsigned char o[640];
    obj = o;
    for (int open = 0; open < 2; ++open) for (int busy = 0; busy < 2; ++busy) for (int dirty = 0; dirty < 2; ++dirty) {
        std::memset(o, 0, sizeof o);
        *reinterpret_cast<unsigned*>(o + 560) = open ? 0x100 : 0;
        *reinterpret_cast<unsigned*>(o + 572) = busy ? 5 : 0;
        *reinterpret_cast<unsigned*>(o + 588) = dirty ? 9 : 0;
        trace.clear();
        int r = Flush__9VIObjFile(o);
        bool go = open && !busy;
        std::string want = go ? (dirty ? "hdr;flush;" : "flush;") : "";
        if (r != (go ? 0 : -1) || trace != want) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
