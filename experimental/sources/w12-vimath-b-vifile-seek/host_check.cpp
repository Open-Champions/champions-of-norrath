// Host check only; not a build input. Build with the host C++ compiler. Drives every dispatch path with recording
// stubs and checks the callee, forwarded offset/origin, return value and an untouched object.
#include "seek.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
extern "C" {
int BufferedSeek__6VIFilelQ26VIFile10SeekOrigin(void*, long o, int g) { trace += "B" + std::to_string(o) + "/" + std::to_string(g) + ";"; return 11; }
int MemorySeek__6VIFilelQ26VIFile10SeekOrigin(void*, long o, int g) { trace += "M" + std::to_string(o) + "/" + std::to_string(g) + ";"; return 22; }
int MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin(void*, long o, int g) { trace += "S" + std::to_string(o) + "/" + std::to_string(g) + ";"; return 33; }
}

int main() {
    bool ok = true;
    const int firsts[] = {0, 5, -1, (int)0x80000000};
    const int words[] = {0, 1, -1, 0x1000};
    const long offsets[] = {0, 7, -9, 0x7fffffff};
    const int origins[] = {0, 1, 2};
    for (int first : firsts)
        for (int w48 : words)
            for (int w56 : words)
                for (long off : offsets)
                    for (int org : origins) {
                        alignas(8) unsigned char object[128];
                        std::memset(object, 0x33, sizeof object);
                        std::memcpy(object, &first, 4);
                        std::memcpy(object + 48, &w48, 4);
                        std::memcpy(object + 56, &w56, 4);
                        unsigned char before[128];
                        std::memcpy(before, object, sizeof object);
                        trace.clear();
                        int got = Seek__6VIFilelQ26VIFile10SeekOrigin(object, off, org);
                        std::string tail = std::to_string(off) + "/" + std::to_string(org) + ";";
                        int want;
                        std::string wt;
                        if (first >= 0) { want = 11; wt = "B" + tail; }
                        else if (w48 != 0) { want = 22; wt = "M" + tail; }
                        else if (w56 != 0) { want = 33; wt = "S" + tail; }
                        else want = -1;
                        ok = ok && got == want && trace == wt && std::memcmp(before, object, sizeof object) == 0;
                    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
