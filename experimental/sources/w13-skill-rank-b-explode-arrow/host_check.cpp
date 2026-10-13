// Host check only; not a build input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW13_RANK_HOST host_check.cpp
// The Skill object is a byte array whose word at +96 is a handle for a table of 8-byte entries (delta 16 for
// the +88 entry, delta 24 for the +72 entry); the stubs mark the run as failed when called with any other
// adjusted pointer. The stubs return values from tables that the check steps through every combination of
// a value set and for four ranks, and write a trace of the calls (index, rank, key, the eight float
// arguments as bit patterns). That trace is compared with one built here from the stated rule, using
// lround for the rounding. The same routine was also compared with the original in a private interpreter run.
#include "get_rank_description.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

namespace {
unsigned char object[256];
unsigned char table[144];
std::string trace;
bool badPointer = false;
int evalValue[4];
int rampFirst[2], rampSecond[2];
unsigned short textBuffer[2];
unsigned short outBuffer[8];
char lastKey[64];

std::string bits(float f) {
    unsigned u;
    std::memcpy(&u, &f, sizeof u);
    char text[16];
    std::snprintf(text, sizeof text, "%08x", u);
    return text;
}
int hostEval(void* self, int index, int rank) {
    if (self != object + 16) badPointer = true;
    trace += "e" + std::to_string(index) + "," + std::to_string(rank) + ";";
    return evalValue[index];
}
void hostRamp(void* self, int index, int rank, float scale, int zero, int* first, int* second) {
    if (self != object + 24 || scale != 0.2f || zero != 0) badPointer = true;
    trace += "r" + std::to_string(index) + "," + std::to_string(rank) + ";";
    *first = rampFirst[index];
    *second = rampSecond[index];
}
[[maybe_unused]] int fifth(int value) {  // independent of the routine: lround rounds halves away from zero
    float x = static_cast<float>(value);
    return static_cast<int>(std::lround(x + x * 0.2f));
}
}

extern "C" const unsigned short* textGetString__FPc(const char* key) {
    std::snprintf(lastKey, sizeof lastKey, "%s", key);
    trace += std::string("k") + key + ";";
    return textBuffer;
}
extern "C" int getEnemyWord__Fi(int value) {
    trace += "n" + std::to_string(value) + ";";
    return value * 3 + 1;
}
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* to, const unsigned short* format, float a, float b, float c, float d,
                                        float e, float f, float g, float h) {
    trace += std::string("u") + (to == outBuffer ? "O" : "?") + (format == textBuffer ? "T" : "?") + ":" + bits(a) + bits(b) +
             bits(c) + bits(d) + bits(e) + bits(f) + bits(g) + bits(h) + ";";
}
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word == 0x1000u) return reinterpret_cast<__UINTPTR_TYPE__>(table);
    if (word == 0x2000u) return reinterpret_cast<__UINTPTR_TYPE__>(&hostEval);
    if (word == 0x3000u) return reinterpret_cast<__UINTPTR_TYPE__>(&hostRamp);
    return 0;
}

int main() {
    const int values[] = {0, 1, -3, 7, 100, -30, 25};
    const int ranks[] = {-2, 0, 3, 10};
    int* vars[] = {&evalValue[2], &evalValue[1], &rampFirst[0], &rampSecond[0]};
    const int count = 4;
    const unsigned handle = 0x1000u;
    std::memcpy(object + 96, &handle, 4);
    const short delta88 = 16, delta72 = 24;
    const unsigned fn88 = 0x2000u, fn72 = 0x3000u;
    std::memcpy(table + 88, &delta88, 2);
    std::memcpy(table + 92, &fn88, 4);
    std::memcpy(table + 72, &delta72, 2);
    std::memcpy(table + 76, &fn72, 4);
    long total = 0;
    int digit[8] = {0};
    for (;;) {
        for (int i = 0; i < count; ++i) *vars[i] = values[digit[i]];
        for (int rank : ranks) {
            trace.clear();
            badPointer = false;
            std::string expect;
            const int* ev = evalValue;
            const int* rf = rampFirst;
            const int* rs = rampSecond;
            (void)ev; (void)rf; (void)rs;
            std::string key;
            float a[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            int enemyArg = 0;
            (void)enemyArg;
            expect += "e2," + std::to_string(rank) + ";";
            expect += "e1," + std::to_string(rank) + ";";
            expect += "r0," + std::to_string(rank) + ";";
            key = "explodearrow_rankdesc";
            a[0] = float(rf[0]);
            a[1] = float(rs[0]);
            a[2] = float(ev[2] / 12);
            a[3] = float(ev[1] / 12);

            expect += "k" + key + ";";

            expect += std::string("uOT:");
            for (int i = 0; i < 8; ++i) expect += bits(a[i]);
            expect += ";";
            getRankDescription__17SkillExplodeArrowPUsi(object, outBuffer, rank);
            ++total;
            if (badPointer || trace != expect) {
                std::printf("FAIL rank %d\n got    %s\n expect %s\n", rank, trace.c_str(), expect.c_str());
                return 1;
            }
        }
        int pos = 0;
        while (pos < count && ++digit[pos] == 7) digit[pos++] = 0;
        if (pos == count) break;
    }
    std::printf("ok %ld runs\n", total);
    return 0;
}
