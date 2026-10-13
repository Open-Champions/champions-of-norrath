// Host contract for msg_trigger__10CloudGianti; not an EE input. Includes the source.
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
char bossMeterName[128];
int bossMeterCurrent;
static std::string calls; static int roll;
void msg_trigger__8Creaturei(void* s, int w) { calls += "base(" + std::to_string(w) + ");"; (void)s; }
void soundResetStream__Fi(int v) { calls += "reset(" + std::to_string(v) + ");"; }
int eRand__10GameObject(void*) { calls += "rand;"; return roll; }
int sprintf(char* d, const char* f, ...) {
    va_list ap; va_start(ap, f); int n = std::vsnprintf(d, 32, f, ap); va_end(ap); calls += std::string("fmt(") + f + ");"; return n;
}
void soundStartMusic__FPci(char* n, int f) { calls += std::string("music(") + n + "," + std::to_string(f) + ");"; }
int main() {
    alignas(16) static unsigned char object[1100];
    int bad = 0;
    std::memset(object, 0, sizeof object);
    std::memset(bossMeterName, 'z', sizeof bossMeterName); bossMeterCurrent = 77; calls.clear();
    msg_trigger__10CloudGianti(object, 5);
    if (calls != "base(5);" || bossMeterName[0] != 'z' || bossMeterCurrent != 77) { std::puts("fail: inactive path"); bad = 1; }
    int flag = 1; float health = 250.9f; std::memcpy(object + 976, &flag, 4); std::memcpy(object + 560, &health, 4);
    for (int r : {0, 1, 2, 3, 4, 7, -1}) {
        std::memset(bossMeterName, 'z', sizeof bossMeterName); bossMeterCurrent = 77; calls.clear(); roll = r;
        msg_trigger__10CloudGianti(object, 5);
        std::string want = std::string("reset(0);rand;fmt(mx_boss_generic%d_l.vag);music(mx_boss_generic") + std::to_string((r & 3) + 1) + "_l.vag,259);";
        if (calls != want || std::strcmp(bossMeterName, "CloudGiantKing") != 0 || bossMeterName[15] != 'z' || bossMeterName[14] != 0 || bossMeterCurrent != 250) {
            std::printf("fail: active path roll %d: %s\n", r, calls.c_str()); bad = 1;
        }
    }
    return bad;
}
