// Host check only; not a build input. Build with the host C++ compiler. Drives every path with recording stubs and
// checks the callee, forwarded arguments, return value and an untouched object.
#include "read.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static std::string trace;
static int buffered_result;
extern "C" {
int BufferedRead__6VIFilePvi(void* s, void* b, int n) { trace += "B" + std::to_string(n) + ";"; (void)s; (void)b; return buffered_result; }
int MemoryRead__6VIFilePvi(void* s, void* b, int n) { trace += "M" + std::to_string(n) + ";"; (void)s; (void)b; return 111; }
int MemorySegmRead__6VIFilePvi(void* s, void* b, int n) { trace += "S" + std::to_string(n) + ";"; (void)s; (void)b; return 222; }
}

int main() {
    bool ok = true;
    const int firsts[] = {0, 5, 0x7fffffff, -1, (int)0x80000000};
    const int words[] = {0, 1, -1, 0x1000};
    const int counts[] = {0, 16, 4096};
    const int bresults[] = {-1, 0, 16, 4096, 15};
    for (int first : firsts)
        for (int w48 : words)
            for (int w56 : words)
                for (int count : counts)
                    for (int br : bresults) {
                        alignas(8) unsigned char object[128];
                        std::memset(object, 0x33, sizeof object);
                        std::memcpy(object, &first, 4);
                        std::memcpy(object + 48, &w48, 4);
                        std::memcpy(object + 56, &w56, 4);
                        unsigned char before[128];
                        std::memcpy(before, object, sizeof object);
                        buffered_result = br;
                        trace.clear();
                        int got = Read__6VIFilePvi(object, object + 100, count);
                        int want;
                        std::string wt;
                        if (first >= 0) { want = br == count ? 0 : -1; wt = "B" + std::to_string(count) + ";"; }
                        else if (w48 != 0) { want = 111; wt = "M" + std::to_string(count) + ";"; }
                        else if (w56 != 0) { want = 222; wt = "S" + std::to_string(count) + ";"; }
                        else { want = -1; }
                        ok = ok && got == want && trace == wt && std::memcmp(before, object, sizeof object) == 0;
                    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
