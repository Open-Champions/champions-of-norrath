// Host comparison only. Not an EE build input. Every callee, the wired-register write and the
// TLB tables are scripted. An independent model of the documented sequence is compared with the
// recorded events for a spread of table sizes, including each overflow stop and the fill.
#include <csetjmp>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CHECK 1
static std::vector<std::string> events;
static void set_wired(int value) { events.push_back("wired " + std::to_string(value)); }
#include "init_tlb_32mb.cpp"
TlbInfoObject TLBInfo;
static std::jmp_buf stop;
extern "C" void kprintf(const char* f, ...) {
    char buf[200];
    va_list ap; va_start(ap, f);
    if (std::strncmp(f, "# TLB spad", 10) == 0) {
        int a = va_arg(ap, int), b = va_arg(ap, int), c = va_arg(ap, int), d = va_arg(ap, int), e = va_arg(ap, int);
        std::snprintf(buf, sizeof buf, "kprintf %d %d %d %d %d", a, b, c, d, e);
    } else {
        std::snprintf(buf, sizeof buf, "kprintf %s", f);
    }
    va_end(ap);
    events.push_back(buf);
}
extern "C" void Exit(int status) { events.push_back("exit " + std::to_string(status)); std::longjmp(stop, 1); }
extern "C" void _SetTLBEntry(int i, int a, int b, int c, int d) {
    events.push_back("set " + std::to_string(i) + " " + std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " " + std::to_string(d));
}
static TlbEntry tables[3][64];
static std::vector<std::string> model(int a, int b, int c, int* final_index) {
    std::vector<std::string> m;
    m.push_back("kprintf " + std::to_string(a - 1) + " " + std::to_string(a) + " " + std::to_string(a + b - 1) + " " + std::to_string(a + b) + " " + std::to_string(a + b + c - 1));
    m.push_back("wired 0");
    int idx = 0;
    const int counts[3] = {a, b, c};
    for (int t = 0; t < 3; ++t) {
        if (t == 2) {
            *final_index = idx;
            m.push_back("wired " + std::to_string(idx));
            if (c <= 0) continue;
        }
        if (idx + counts[t] >= 49) {
            m.push_back(std::string("kprintf # TLB over flow (") + char('1' + t) + ")");
            m.push_back("exit 1");
            return m;
        }
        for (int i = 0; i < counts[t]; ++i, ++idx) {
            const TlbEntry& e = tables[t][i];
            m.push_back("set " + std::to_string(idx) + " " + std::to_string(e.word0) + " " + std::to_string(e.word1) + " " + std::to_string(e.word2) + " " + std::to_string(e.word3));
        }
    }
    unsigned addr = static_cast<unsigned>(idx) * 8192u + 0xe0000000u;
    for (int i = idx; i < 48; ++i, addr += 8192u) m.push_back("set " + std::to_string(i) + " 0 " + std::to_string(static_cast<int>(addr)) + " 0 0");
    return m;
}
int main() {
    int failures = 0;
    for (int t = 0; t < 3; ++t) for (int i = 0; i < 64; ++i) tables[t][i] = {t * 1000 + i, i * 7, i * 13 + t, -i};
    const int cases[][3] = {{3, 4, 2}, {3, 4, 0}, {3, 4, -1}, {1, 1, 1}, {10, 20, 5}, {20, 28, 0}, {20, 29, 0}, {48, 0, 0}, {49, 0, 0}, {47, 1, 0},
                            {20, 27, 1}, {30, 19, 1}, {0, 0, 3}, {40, 0, 8}, {40, 0, 9}, {2, 0, 50}};
    for (const auto& k : cases) {
        TLBInfo = {k[0], k[1], k[2], 0x77, tables[0], tables[1], tables[2]};
        events.clear();
        if (setjmp(stop) == 0) InitTLB32MB();
        int fin = -1;
        std::vector<std::string> want = model(k[0], k[1], k[2], &fin);
        if (events != want) {
            std::printf("case %d/%d/%d differs (%zu vs %zu events)\n", k[0], k[1], k[2], events.size(), want.size());
            for (size_t i = 0; i < events.size() && i < want.size(); ++i) if (events[i] != want[i]) { std::printf("  %s | %s\n", events[i].c_str(), want[i].c_str()); break; }
            ++failures;
        }
        if (k[0] + k[1] < 49 && k[0] < 49 && TLBInfo.final_index != k[0] + k[1]) { std::printf("final index word\n"); ++failures; }
    }
    if (failures) return 2;
    std::printf("init_tlb_32mb cases passed\n");
    return 0;
}
