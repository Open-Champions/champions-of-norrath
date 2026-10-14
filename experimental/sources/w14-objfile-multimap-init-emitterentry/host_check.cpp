// Host check only; not a build input. Callees return scripted results and record order.
#include "init.cpp"
#include <cstdio>
#include <string>
static std::string trace;
static int clear_result, allocate_result, seen_count;
extern "C" int Clear__t10VIMultiMap2ZfZ14VIEmitterEntry(void*) { trace += "clear;"; return clear_result; }
extern "C" int Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry(void* self) { trace += "alloc;"; seen_count = static_cast<int*>(self)[1]; return allocate_result; }
int main() {
    bool ok = true;
    const int results[] = {-5, -1, 0, 1, 7};
    for (int c : results)
        for (int a : results) {
            trace.clear(); clear_result = c; allocate_result = a;
            int box[2] = {0x1111, 0x2222};
            int r = Init__t10VIMultiMap2ZfZ14VIEmitterEntryi(box, 42);
            int want = c < 0 ? -1 : (a < 0 ? -1 : 0);
            std::string want_trace = c < 0 ? "clear;" : "clear;alloc;";
            if (r != want || trace != want_trace) ok = false;
            if (c < 0 ? box[1] != 0x2222 : (box[1] != 43 || seen_count != 43)) ok = false;
            if (box[0] != 0x1111) ok = false;
        }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
